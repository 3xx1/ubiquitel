#include "UbManager.h"

UbManager::UbManager() {
    active = 0;
    dockedUbID = -1;
    isDocking = false;
    begin();
}

UbManager::~UbManager() {
    stopServer();
}

void UbManager::sendLoop(long loop) {//ループ周期の送信
    if(!isDocking) return;
    long data[2];
    ubs[dockedUbID].loop = loop;
    data[0] = SET_LOOP;
    data[1] = loop;
    
    sendData(data, sizeof(data), dockedUbID);
}

void UbManager::addNote(long ts, long intensity) {//ノートをユビに追加
    Note note;
    note.timeStamp = ts;
    note.intensity = intensity;
    addNote(note);
}

void UbManager::addNote(Note note) {//ノートをユビに追加
    if(!isDocking) return;
    ubs[dockedUbID].notes[ubs[dockedUbID].numNotes].timeStamp = note.timeStamp;
    ubs[dockedUbID].notes[ubs[dockedUbID].numNotes].intensity = note.intensity;
    ubs[dockedUbID].numNotes++;
    sort(dockedUbID);
}

void UbManager::sendNotes() {//複数ノートをユビに送信
    if(!isDocking) return;
    long data[128];
    
    data[0] = SET_NOTE;
    for(int i=0;i<ubs[dockedUbID].numNotes;i++) {
        data[2*i+1] = ubs[dockedUbID].notes[i].timeStamp;
        data[2*i+2] = ubs[dockedUbID].notes[i].intensity;
    }
    sendData(data, 1+2*ubs[dockedUbID].numNotes*sizeof(long), dockedUbID);
}

void UbManager::resetNote() {//全てのノートをリセット
    if(!isDocking) return;
    ubs[dockedUbID].numNotes = 0;
    long data = RESET_NOTE;
    sendData(&data, sizeof(data), dockedUbID);
}

void UbManager::play(long ubID) {
    long data = PLAY_UB;
    sendData(&data, sizeof(data), ubID);
}

void UbManager::pause(long ubID) {
    long data = PAUSE_UB;
    sendData(&data, sizeof(data), ubID);
}

void UbManager::stop(long ubID) {
    long data = STOP_UB;
    sendData(&data, sizeof(data), ubID);
}

long UbManager::getDockedUbID() {
    return dockedUbID;
}

void UbManager::begin() {
    udp.begin(6340);
}

void UbManager::stopServer() {
}

void UbManager::sync() {//ユビの同時再生
    broadcast(SYNC_UB, sizeof(long));
}

void UbManager::search() {//ユビ検索用一斉送信
    broadcast(SEARCH_UB, sizeof(long));
}

void UbManager::broadcast(DataType d, long size) {
    udp.beginPacket("255.255.255.255", 6341);
    udp.write((char *)&d, size);
    udp.endPacket();
}

void UbManager::sendData(void *d, long size, long ubID) {
    udp.beginPacket(ubs[ubID].ip, 6341);
    udp.write((char *)d, size);
    udp.endPacket();
}

void UbManager::confirm(CallbackType cbt, long ubID) {
    long data[2] = {CONFIRM, cbt};
    sendData(data, sizeof(long)*2, ubID);
}

//IPアドレス待受
void UbManager::parsePacket() {
    packetSize = udp.parsePacket();
    
    if(packetSize) {
        //親機から受信
        udp.read((char *)packet, sizeof(long)*1000);
        if(packet[0]==UB_FOUND) {//IPアドレス
            bool onList = false;
            for (int i=0; i<numUbs; i++) {
                if (ubs[i].ip == udp.remoteIP())
                    onList= true;
            }
            if(!onList) {
                ubs[numUbs].loop = 0;
                ubs[numUbs].ip = udp.remoteIP();
                numUbs++;
                confirm(UB_FOUND, numUbs-1);
                (*callback)(UB_FOUND, numUbs-1);
            }
        }else if(packet[0]==UB_DOCKED) {//ドッキング
            //dockedUb探し
            for (int i=0; i<numUbs; i++) {
                if (ubs[i].ip == udp.remoteIP()) {
                    if (dockedUbID == -1) {
                        confirm(UB_DOCKED, i);
                        dockedUbID = i;
                        isDocking = true;
                        (*callback)(UB_DOCKED, i);
                    }
                }
            }
        }else if(packet[0]==UB_UNDOCKED) {//ドッキング解除
            if (dockedUbID != -1) {
                confirm(UB_UNDOCKED, dockedUbID);
                isDocking = false;
                (*callback)(UB_UNDOCKED, dockedUbID);
                dockedUbID = -1;
            }
        }
    }
}

void UbManager::sort(int ubID) {
    for(int i=0; i<(ubs[ubID].numNotes-1); i++) {
        for(int o=0; o<(ubs[ubID].numNotes-(i+1)); o++) {
            if(ubs[ubID].notes[o].timeStamp > ubs[ubID].notes[o+1].timeStamp) {
                Note t;
                t.timeStamp = ubs[ubID].notes[o].timeStamp;
                t.intensity = ubs[ubID].notes[o].intensity;
                ubs[ubID].notes[o].timeStamp = ubs[ubID].notes[o+1].timeStamp;
                ubs[ubID].notes[o].intensity = ubs[ubID].notes[o+1].intensity;
                ubs[ubID].notes[o+1].timeStamp = t.timeStamp;
                ubs[ubID].notes[o+1].intensity = t.intensity;
            }
        }
    }
}
