#include "UbManager.h"

UbManager::UbManager() {
    active = 0;
    dockedUbID = -1;
    isDocking = false;
    startServer();
}

UbManager::~UbManager() {
    stopServer();
}

void UbManager::sendLoop(int loop) {//ループ周期の送信
    if(!isDocking) return;
    int data[2];
    ubs[dockedUbID].loop = loop;
    data[0] = SET_LOOP;
    data[1] = loop;
    
    sendData(data, sizeof(data), dockedUbID);
    printf("send loop %d\n",loop);
}

void UbManager::addNote(int ts, int intensity) {//ノートをユビに追加
    Note note;
    note.timeStamp = ts;
    note.intensity = intensity;
    addNote(note);
    printf("add note %d, %d\n",ts, intensity);
}

void UbManager::addNote(Note note) {//ノートをユビに追加
    if(!isDocking) {printf("**NO UB!**\n"); return;}
    ubs[dockedUbID].notes.push_back(note);
    ubs[dockedUbID].notes.sort();
}

void UbManager::sendNotes() {//複数ノートをユビに送信
    if(!isDocking) {printf("**NO UB!**\n"); return;}
    
    ubs[dockedUbID].notes.sort();
    int *data = (int *)calloc(1+2*ubs[dockedUbID].notes.size(), sizeof(int));
    int *dp = data;
    
    *dp++ = SET_NOTE;
    auto it = ubs[dockedUbID].notes.begin();
    while(it != ubs[dockedUbID].notes.end()) {
        Note note = (Note)*it;
        *dp++ = note.timeStamp;
        *dp++ = note.intensity;
        ++it;
    }
    sendData(data, 1+2*ubs[dockedUbID].notes.size()*sizeof(int), dockedUbID);
    printf("send notes\n");
}

void UbManager::resetNotes() {//全てのノートをリセット
    if(!isDocking) {printf("**NO UB!**\n"); return;}
    ubs[dockedUbID].notes.clear();
    int data = RESET_NOTE;
    sendData(&data, sizeof(data), dockedUbID);
    printf("reset notes\n");
}

void UbManager::play(int ubID) {
    int data[2] = {PLAY_UB, 0};
    sendData(data, sizeof(data), ubID);
    printf("play!\n");
}

void UbManager::playAll() {
    int data[2] = {PLAY_UB, 0};
    broadcast(data, sizeof(data));
}

void UbManager::playAt(int ubID, int time) {
    int data[2] = {PLAY_UB, time};
    sendData(data, sizeof(data), ubID);
}

void UbManager::pause(int ubID) {
    int data = PAUSE_UB;
    sendData(&data, sizeof(data), ubID);
}

void UbManager::stop(int ubID) {
    int data = STOP_UB;
    sendData(&data, sizeof(data), ubID);
}

int UbManager::getDockedUbID() {
    return dockedUbID;
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
            bool onList = false;
            strcpy(ip, inet_ntoa(senderinfo.sin_addr));
            for (int i=0; i<ubm->ubs.size(); i++) {
                if (strcmp(ubm->ubs[i].ip,ip)==0) {
                    //発見したらドッキングしているユビIDを記録し、返事する
                    ubm->confirm(UB_DOCKED, i);
                    ubm->dockedUbID = i;
                    ubm->isDocking = true;
                    ubm->callback(UB_DOCKED, i);
                    onList = true;
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
                bool onList = false;
                strcpy(ub.ip, inet_ntoa(senderinfo.sin_addr));
                
                //ユビリストからドッキングしていたと思われるユビ探し
                for (int i=0; i<ubm->ubs.size(); i++) {
                    if (strcmp(ubm->ubs[i].ip,ub.ip)==0) {
                        onList= true;
                        ubm->confirm(UB_UNDOCKED, i);
                        ubm->callback(UB_UNDOCKED, i);
                    }
                }
            }
        }
    }
    close(sock);
    pthread_exit(NULL);
}
