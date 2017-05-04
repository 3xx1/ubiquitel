#ifndef UbManager_h
#define UbManager_h

#include "ub.h"
#include <chrono>
#include <iostream>
// メソッドのポインタをイベントリスナとするためのテンプレートクラス
class ListenerBase {
public:
    virtual void callback(CallbackType cbt, int ubID) = 0;
};

template <class T>
class MemFuncListener : public ListenerBase{
public:
    MemFuncListener(T* obj, void (T::*cbf)(CallbackType, int)){
        m_obj = obj;
        m_cbf = cbf;
    };
    
    virtual void callback(CallbackType cbt, int ubID){
        (m_obj->*m_cbf)(cbt, ubID);
    };
private:
    T* m_obj;
    void (T::*m_cbf)(CallbackType, int);
};

class UbManager {
public:
    bool isDocking;//ドッキングしてるユビがあるかを示すフラグ
    volatile int active;//サーバ起動中を示すフラグ
    int loopCount;
    UbManager();
    ~UbManager();
	int getTimestamp();//同期後の累積時間
    void addLoop(int loop, int repeat);//ループ周期、ループ回数をユビに送信
    void addNote(int ts, int intensity);//覚えさせたいノートを追加
    void addNote(Note note);//覚えさせたいノートを追加]
    void addNotes(std::list<Note> notes);
    void sendNotes();//覚えさせたノートをユビに送信
    void sendNotes(int from, int to);//覚えさせたノートのうち、特定の時間領域をユビに送信
    void sendEmptyLoop();//空ループを送信
    void resetNotes();//全てのノートをリセット
    void resetAll();//全てのユビのノートをリセット
    void sync();//ユビクロックの同期
    void playAll();//全てのユビ同時再生
    void play();
    void playAt(int time);
    void pause();
    void stop();
    void stopAll();
    int getDockedUbID();//ドッキングしているユビIDゲッタ．ドッキングしていない時は-1を返す
    int getDestUbID();//データ送信先ユビIDゲッタ．
    int getUbSize();//ユビの個数
    int getNoteSize(int ubID);
    void setDestUbID(int destID);//データ送信先ユビIDセッタ．
    void setTimer(int lt);
    void search();
    
    //コールバック用
    template <class T>
    void setCallback(T* obj, void (T::*cbf)(CallbackType, int)){
        m_listener = new MemFuncListener<T>(obj, cbf);
    };
    void callback(CallbackType cbt, int ubID){
        m_listener->callback(cbt, ubID);
    }

private:
    int dockedUbID, destUbID;
    std::vector<Ub> ubs;
    std::chrono::system_clock::time_point start;
    int looptime;
    ListenerBase *m_listener;

    //サーバ用スレッド
    pthread_t thread;
    static void *threadFunction(void *data);
    void startServer();
    void stopServer();
    
    //データ送信用内部メソッド
    void sendData(void *d, int size, int ubID);
    void broadcast(void *d, int size);
    void confirm(CallbackType cbt, int ubID);
    
    //その他
    int getMaxIntensity(int ts, int prets);
};
#endif /* UbManager_h */
