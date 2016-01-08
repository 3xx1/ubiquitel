#ifndef UbManager_h
#define UbManager_h

#include "ub.h"

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

  UbManager();
  ~UbManager();
  void sendLoop(int loop);//ループ周期をユビに送信
  void addNote(int ts, int intensity);//覚えさせたいノートを追加
  void addNote(Note note);//覚えさせたいノートを追加
  void sendNotes();//覚えさせたノートをユビに送信
  void resetNote();//全てのノートをリセット
  void sync();//ユビの同期，未実装
  void play(int ubID);
  void playAll();//全てのユビ同時再生
  void pause(int ubID);
  void stop(int ubID);
  int getDockedUbID();//ドッキングしているユビIDゲッタ．ドッキングしていない時は-1を返す
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
  int dockedUbID;
  std::vector<Ub> ubs;
  ListenerBase *m_listener;
  //サーバ用スレッド
  pthread_t thread;
  static void *threadFunction(void *data);
  void startServer();
  void stopServer();
  
  //データ送信用内部メソッド
  void sendData(void *d, int size, int ubID);
  void broadcast(DataType d, int size);
};
#endif /* UbManager_h */
