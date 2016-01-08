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
}

void UbManager::addNote(int ts, int intensity) {//ノートをユビに追加
    Note note;
    note.timeStamp = ts;
    note.intensity = intensity;
    addNote(note);
}

void UbManager::addNote(Note note) {//ノートをユビに追加
    if(!isDocking) return;
    ubs[dockedUbID].notes.push_back(note);
    ubs[dockedUbID].notes.sort();
}

void UbManager::sendNotes() {//複数ノートをユビに送信
    if(!isDocking) return;
    
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
    sendData(data, sizeof(data), dockedUbID);
}

void UbManager::resetNote() {//全てのノートをリセット
    if(!isDocking) return;
    ubs[dockedUbID].notes.clear();
    int data = RESET_NOTE;
    sendData(&data, sizeof(data), dockedUbID);
}

void UbManager::play(int ubID) {
    int data = PLAY_UB;
    sendData(&data, sizeof(data), ubID);
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

void UbManager::sync() {//ユビの同期
    broadcast(SYNC_UB, sizeof(int));
}

void UbManager::search() {//ユビ検索用一斉送信
    broadcast(SEARCH_UB, sizeof(int));
}

void UbManager::broadcast(DataType d, int size) {
    int sock;
    struct sockaddr_in addr;
    int yes = 1;
    
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(6341);
    addr.sin_addr.s_addr = inet_addr("255.255.255.255");
    
    setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *)&yes, sizeof(yes));
    sendto(sock, &d, size, 0, (struct sockaddr *)&addr, sizeof(addr));
    close(sock);
    printf("sent!\n");
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

//IPアドレス待受
//ノートデータ送信

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
            strcpy(ub.ip, inet_ntoa(senderinfo.sin_addr));
            ubm->ubs.push_back(ub);
            ubm->callback(UB_FOUND, ubm->ubs.size()-1);
            
        }else if(type==UB_DOCKED) {//ドッキング
            //dockedUb探し
            char ip[16];
            strcpy(ip, inet_ntoa(senderinfo.sin_addr));
            for (int i=0; i<ubm->ubs.size(); i++) {
                if (strcmp(ubm->ubs[i].ip,ip)==0) {
                    ubm->dockedUbID = i;
                    ubm->isDocking = true;
                    ubm->callback(UB_DOCKED, i);
                }
            }
            
        }else if(type==UB_UNDOCKED) {//ドッキング解除
            ubm->isDocking = false;
            ubm->callback(UB_UNDOCKED, ubm->dockedUbID);
            ubm->dockedUbID = -1;
        }
    }
    close(sock);
    pthread_exit(NULL);
}
