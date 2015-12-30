#include "UbManager.h"

//UbManagerを持つアプリケーション
class TestApp {
public:
  UbManager ubm;

  TestApp() {
    //コールバック関数の登録
    ubm.setCallback(this, &TestApp::ubCallback);
    //子機たちのIPアドレス取得
    ubm.search();
  };
  ~TestApp(){};
  
  //ユビが見つかった時，ドッキングした時，ドッキング解除した時に呼ばれるコールバック関数
  void ubCallback(CallbackType cbt, int ubID){//ユビ状況，ユビID
    switch(cbt) {
      case UB_FOUND://見つかった時
        printf("ub%d found!\n", ubID);
        break;
        
      case UB_DOCKED://ドッキングした時
        printf("ub%d docked!\n", ubID);
        
        //過去に覚えこませたノートをリセット
        ubm.resetNote();
        
        //タイムスタンプ(ms),強さ(0~10)をいれて，覚えさせるノートを追加
        ubm.addNote(0, 8);
        
        //Note構造体を使ってもOK
        Note note;
        note.timeStamp = 250;
        note.intensity = 3;
        ubm.addNote(note);
        
        //追加順は時系列じゃなくてもOK
        ubm.addNote(750, 3);
        ubm.addNote(500, 10);
        
        //追加したノートをユビに送信
        ubm.sendNotes();
        
        //ループ周期をユビに送信
        ubm.sendLoop(1000);
        
        //以上のメソッドはドッキングしているユビにのみおこなわれるメソッド
        //以降はユビIDを指定することで，任意のユビに対して実行可能
        
        //再生，一時停止，停止（0に戻る）
        ubm.play(ubID);
        usleep(1500000);
        ubm.pause(ubID);
        usleep(1000000);
        ubm.play(ubID);
        usleep(500000);
        ubm.stop(ubID);
        
        break;
        
      case UB_UNDOCKED://ドッキング解除した時
        printf("ub%d undocked!\n", ubID);
        break;
    }
  };
};

int main() {
  TestApp app;
  //任意のキーを入力すると終了
  getchar();
  return 0;
}