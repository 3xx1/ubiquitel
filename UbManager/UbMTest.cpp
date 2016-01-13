#include "UbManager.h"

//UbManagerを持つアプリケーション
class TestApp {
public:
    UbManager ubm;
    int n;
    
    TestApp() {
        //コールバック関数の登録
        n = -1;
        ubm.setCallback(this, &TestApp::ubCallback);
        //子機たちのIPアドレス取得
        ubm.search();
    };
    ~TestApp(){};
    
    void play() { if(n >= 0) ubm.play(n); printf("ub%d play!\n", n);}
    void pause() { if(n >= 0) ubm.pause(n); printf("ub%d pause!\n", n);}
    void stop() { if(n >= 0) ubm.stop(n); printf("ub%d stop!\n", n);}
    void sync() {ubm.sync(); printf("sync ub!\n");}
    void search() {ubm.search(); printf("searching ub...\n");}

    //ユビが見つかった時，ドッキングした時，ドッキング解除した時に呼ばれるコールバック関数
    void ubCallback(CallbackType cbt, int ubID){//ユビ状況，ユビID
        switch(cbt) {
            case UB_FOUND://見つかった時
                printf("ub%d found!\n", ubID);
                n = ubID;
                break;
              
            case UB_DOCKED://ドッキングした時
                printf("ub%d docked!\n", ubID);
              
                //過去に覚えこませたノートをリセット
                ubm.resetNote();
              
                //タイムスタンプ(ms),強さ(0~10)をいれて，覚えさせるノートを追加
                ubm.addNote(0, 10);
              
                //Note構造体を使ってもOK
                Note note;
                note.timeStamp = 250;
                note.intensity = 9;
                ubm.addNote(note);
              
                //追加順は時系列じゃなくてもOK
                ubm.addNote(250*2, 8);
                ubm.addNote(250*3, 7);
                ubm.addNote(250*4, 6);
                ubm.addNote(250*5, 5);
                ubm.addNote(250*6, 4);
                ubm.addNote(250*7, 3);
                ubm.addNote(250*8, 2);
                ubm.addNote(250*9, 1);

                //追加したノートをユビに送信
                ubm.sendNotes();
              
                //ループ周期をユビに送信
                ubm.sendLoop(2500);
              
                //以上のメソッドはドッキングしているユビにのみおこなわれるメソッド
                break;

            case UB_UNDOCKED://ドッキング解除した時
                printf("ub%d undocked!\n", ubID);
                break;
    }
  };
};

int main() {
    TestApp app;
    int i;
    bool q = false;

    while(q == false) {
        scanf("%d", &i);
        switch(i) {
            case 1://ユビプレイ
                app.play();
                break;
            case 2://ユビポーズ
                app.pause();
                break;
            case 3://ユビストップ
                app.stop();
                break;
            case 4://ユビシンク
                app.sync();
                break;
            case 5://ユビサーチ
                app.search();
                break;
            case 9:
                q = true;
                break;
            default:
                break;
        }
    }
    return 0;
}