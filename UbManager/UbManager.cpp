#include "UbManager.h"

UbManager::UbManager() {
    active = 0;
    dockedUbID = -1;
    destUbID = -1;
    isDocking = false;
    looptime = 0;
    loopCount = 0;
	start = std::chrono::system_clock::now();
    startServer();
}

void UbManager::setTimer(int lt) {
    looptime = lt;
}

int UbManager::getTimestamp() {
  std::chrono::duration<double> sec = std::chrono::system_clock::now() - start;
  int duration = (double)sec.count()*1001-300;
  return duration;
}

UbManager::~UbManager() {
    stopServer();
}

void UbManager::addLoop(int loop, int repeat) {//ループ周期の送信
    if(destUbID == -1) return;
    int data[3];
    ubs[destUbID].loop = loop;
    ubs[destUbID].repeat = repeat;

    //printf("add loop %d\n",loop);
}

void UbManager::addNote(int ts, int intensity) {//ノートをユビに追加
    Note note;
    note.timeStamp = ts;
    note.intensity = intensity;
    addNote(note);
    //printf("add note %d, %d\n",ts, intensity);
}

void UbManager::addNote(Note note) {//ノートをユビに追加
    if(destUbID == -1) {printf("**NO UB!**\n"); return;}
    bool identical = false;
    auto it = ubs[destUbID].notes.begin();
    while(it != ubs[destUbID].notes.end()) {
        Note *n = (Note *) &(*it);
        if(n->timeStamp == note.timeStamp) {
            n->intensity = note.intensity;
            identical = true;
        }
        ++it;
    }
    if(!identical) {
        ubs[destUbID].notes.push_back(note);
        ubs[destUbID].notes.sort();
    }
}

void UbManager::sendNotes() {//複数ノートをユビに送信
    if(destUbID == -1) {printf("**NO UB!**\n"); return;}

    ubs[destUbID].notes.sort();
    int size = 3+2*ubs[destUbID].notes.size();
    int *data = (int *)calloc(size, sizeof(int));
    int *dp = data;
    *dp++ = SET_NOTE;
    *dp++ = ubs[destUbID].loop;
    *dp++ = ubs[destUbID].repeat;
    auto it = ubs[destUbID].notes.begin();
    while(it != ubs[destUbID].notes.end()) {
        Note note = (Note)*it;
        *dp++ = note.timeStamp;
        *dp++ = note.intensity;
        ++it;
    }
    sendData(data, size*sizeof(int), destUbID);
}

void UbManager::sendNotes(int from, int to) {//特定時間分のノートをユビに送信
    if(destUbID == -1) {printf("**NO UB!**\n"); return;}
    
    ubs[destUbID].notes.sort();
    int num = 0;
    auto it = ubs[destUbID].notes.begin();
    while(it != ubs[destUbID].notes.end()) {
        Note note = (Note)*it;
        if(from <= note.timeStamp && note.timeStamp < to)
            num++;
        ++it;
    }
    if(num == 0) {
        addNote(to-1,0);
        num++;
    }
    int size = 3+2*num;
    int *data = (int *)calloc(size, sizeof(int));
    int *dp = data;
    *dp++ = SET_NOTE;
    *dp++ = to - from;
    *dp++ = ubs[destUbID].repeat;
    it = ubs[destUbID].notes.begin();
    while(it != ubs[destUbID].notes.end()) {
        Note note = (Note)*it;
        if(from <= note.timeStamp && note.timeStamp < to) {
            *dp++ = note.timeStamp - from;
            *dp++ = note.intensity;
            printf("add note %d %d\n", note.timeStamp - from, note.intensity);

        }
        ++it;
    }
    sendData(data, size*sizeof(int), destUbID);
    printf("send %d data, loop:%d\n", num, to - from);
}

void UbManager::resetNotes() {//全てのノートをリセット
    if(destUbID == -1) {printf("**NO UB!**\n"); return;}
    ubs[destUbID].notes.clear();
    int data = RESET_NOTE;
    sendData(&data, sizeof(data), destUbID);
    //printf("reset notes\n");
}

void UbManager::resetAll() {//全てのユビのノートをリセット
    for(int i=0;i<ubs.size();i++) {
        ubs[i].notes.clear();
    }
    int data = RESET_NOTE;
    broadcast(&data, sizeof(int));
    printf("reset All\n");
}

void UbManager::play() {
    if(destUbID == -1) {printf("**NO UB!**\n"); return;}
    int data[2] = {PLAY_UB, 0};
    sendData(data, sizeof(data), destUbID);
    printf("play!\n");
}

void UbManager::playAll() {
    int data[2] = {PLAY_UB, 0};
    broadcast(data, sizeof(data));
}

void UbManager::playAt(int time) {
    if(destUbID == -1) {printf("**NO UB!**\n"); return;}
    int data[2] = {PLAY_UB, time};
    sendData(data, sizeof(data), destUbID);
}

void UbManager::pause() {
    if(destUbID == -1) {printf("**NO UB!**\n"); return;}
    int data = PAUSE_UB;
    sendData(&data, sizeof(data), destUbID);
}

void UbManager::stop() {
    if(destUbID == -1) {printf("**NO UB!**\n"); return;}
    int data = STOP_UB;
    sendData(&data, sizeof(data), destUbID);
}

void UbManager::stopAll() {
    int data = STOP_UB;
    broadcast(&data, sizeof(int));
}

int UbManager::getDockedUbID() {
    return dockedUbID;
}

int UbManager::getDestUbID() {
    return dockedUbID;
}

int UbManager::getUbSize() {
    return ubs.size();
}
void UbManager::setDestUbID(int destID) {
    destUbID = destID;
}

int UbManager::getNoteSize(int ubID) {
    if(ubs.size() > ubID)
        return ubs[ubID].notes.size();
    else
        return -1;
}

void UbManager::startServer() {
    int result;
    
    if (!active) {
        active = 1;
        result = pthread_create(&thread, NULL, threadFunction, (void *)this);
    }
    
}

void UbManager::stopServer() {
    int result;
    
    if (active) {
        active = 0;
        result = pthread_join( thread, NULL );
    }
}

void UbManager::sync() {//ユビクロックの同期
    int data = SYNC_UB;
    broadcast(&data, sizeof(int));
	start = std::chrono::system_clock::now();
    loopCount = 0;
}

void UbManager::search() {//ユビ検索用一斉送信
    int data = SEARCH_UB;
    broadcast(&data, sizeof(int));
}

void UbManager::broadcast(void *d, int size) {
    int sock;
    struct sockaddr_in addr;
    int yes = 1;
    
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(6341);
    addr.sin_addr.s_addr = inet_addr("255.255.255.255");
    
    setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *)&yes, sizeof(yes));
    sendto(sock, d, size, 0, (struct sockaddr *)&addr, sizeof(addr));
    close(sock);
}

void UbManager::sendData(void *d, int size, int ubID) {
    int sock;
    struct sockaddr_in addr;
    
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(6341);
    addr.sin_addr.s_addr = inet_addr(ubs[ubID].ip);

    sendto(sock, d, size, 0, (struct sockaddr *)&addr, sizeof(addr));
    close(sock);
}

void UbManager::confirm(CallbackType cbt, int ubID) {
    int data[2] = {CONFIRM, cbt};
    sendData(data, sizeof(int)*2, ubID);
}

//IPアドレス待受
void *UbManager::threadFunction(void *data) {
    UbManager *ubm = (UbManager *)data;
    int sock, val;
    struct sockaddr_in addr, senderinfo;
    
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(6340);
    addr.sin_addr.s_addr = INADDR_ANY;
    bind(sock, (struct sockaddr *)&addr, sizeof(addr));
    val =1;
    ioctl(sock, FIONBIO, &val);
    
    while (ubm->active) {
        int n;
        CallbackType type;
        socklen_t addrlen;
        addrlen = sizeof(senderinfo);
        
        //printf("!%d\n",ubm->getTimestamp());
        if(ubm->looptime != 0 && ubm->getTimestamp()/ubm->looptime == ubm->loopCount) {
            ubm->callback(UB_TIMER, NULL);
            ubm->loopCount++;
        }
        
        n = recvfrom(sock, &type, sizeof(type), 0,
                     (struct sockaddr *)&senderinfo, &addrlen);
        if(n == -1) {
            usleep(1000);
            continue;
        }
        
        if(type==UB_FOUND) {//IPアドレス
            Ub ub;
            ub.loop = 0;
            bool onList = false;
            strcpy(ub.ip, inet_ntoa(senderinfo.sin_addr));
            //すでにユビリストに追加してないかチェック
            for (int i=0; i<ubm->ubs.size(); i++) {
                //追加してたらとりあえず返事だけする
                if (strcmp(ubm->ubs[i].ip,ub.ip)==0) {
                    onList= true;
                    ubm->confirm(UB_FOUND, i);
                    ubm->callback(UB_FOUND, i);
                }
            }
            //リストにない時は追加
            if(!onList) {
                ubm->ubs.push_back(ub);
                ubm->confirm(UB_FOUND, ubm->ubs.size()-1);
                ubm->callback(UB_FOUND, ubm->ubs.size()-1);
            }
        }
        else if(type==UB_DOCKED) {//ドッキング
            //dockedUb探し
            char ip[16];
            strcpy(ip, inet_ntoa(senderinfo.sin_addr));
            for (int i=0; i<ubm->ubs.size(); i++) {
                if (strcmp(ubm->ubs[i].ip,ip)==0) {
                    //発見したらドッキングしているユビIDを記録し、返事する
                    ubm->confirm(UB_DOCKED, i);
                    ubm->dockedUbID = i;
                    ubm->destUbID = i;
                    ubm->isDocking = true;
                    ubm->callback(UB_DOCKED, i);
                }
            }
        }
        else if(type==UB_UNDOCKED) {//ドッキング解除
            //すでにドッキングしている時
            if (ubm->dockedUbID != -1) {
                ubm->confirm(UB_UNDOCKED, ubm->dockedUbID);
                ubm->isDocking = false;
                ubm->callback(UB_UNDOCKED, ubm->dockedUbID);
                ubm->dockedUbID = -1;
            }
            else {//ドッキングしているものがない時
                Ub ub;
                ub.loop = 0;
                strcpy(ub.ip, inet_ntoa(senderinfo.sin_addr));
                
                //ユビリストからドッキングしていたと思われるユビ探し
                for (int i=0; i<ubm->ubs.size(); i++) {
                    if (strcmp(ubm->ubs[i].ip,ub.ip)==0) {
                        ubm->confirm(UB_UNDOCKED, i);
                        ubm->callback(UB_UNDOCKED, i);
                    }
                }
            }
        }
        else if(type==UB_PLAYED) {
            Ub ub;
            strcpy(ub.ip, inet_ntoa(senderinfo.sin_addr));
            for (int i=0; i<ubm->ubs.size(); i++) {
                if (strcmp(ubm->ubs[i].ip,ub.ip)==0) {
                    ubm->confirm(UB_PLAYED, i);
                    ubm->callback(UB_PLAYED, i);
                }
            }
        }
        else if(type==UB_STOPPED) {
            Ub ub;
            strcpy(ub.ip, inet_ntoa(senderinfo.sin_addr));
            for (int i=0; i<ubm->ubs.size(); i++) {
                if (strcmp(ubm->ubs[i].ip,ub.ip)==0) {
                    ubm->confirm(UB_STOPPED, i);
                    ubm->callback(UB_STOPPED, i);
                }
            }
        }
    }
    close(sock);
    pthread_exit(NULL);
}
