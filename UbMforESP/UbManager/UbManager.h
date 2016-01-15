#ifndef UbManager_h
#define UbManager_h
#include "ub.h"

typedef void (*Callback)(CallbackType cbt, int ubID);

class UbManager {
public:
    bool isDocking;//ドッキングしてるユビがあるかを示すフラグ
    volatile long active;//サーバ起動中を示すフラグ
    
    UbManager();
    ~UbManager();
    void sendLoop(long loop);//ループ周期をユビに送信
    void addNote(long ts, long longensity);//覚えさせたいノートを追加
    void addNote(Note note);//覚えさせたいノートを追加
    void sendNotes();//覚えさせたノートをユビに送信
    void resetNote();//全てのノートをリセット
    void sync();//全てのユビ同時再生
    void play(long ubID);
    void pause(long ubID);
    void stop(long ubID);
    long getDockedUbID();//ドッキングしているユビIDゲッタ．ドッキングしていない時は-1を返す
    void search();
    void sort(int ubID);
    //コールバック用
    Callback *callback;
    void setCallback(Callback cb) { callback = &cb; };

private:
    int dockedUbID;
    Ub ubs[32];
    int numUbs;
    WiFiUDP udp;
    long packet[2];
    int packetSize = 0;
    void parsePacket();
    void begin();
    void stopServer();
    
    //データ送信用内部メソッド
    void sendData(void *d, long size, long ubID);
    void broadcast(DataType d, long size);
    void confirm(CallbackType cbt, long ubID);
};
#endif /* UbManager_h */
