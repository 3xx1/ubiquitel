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
    
    void play() { if(n >= 0) ubm.playAt(n, 5000); printf("ub%d play!\n", n);}
    void pause() { if(n >= 0) ubm.pause(n); printf("ub%d pause!\n", n);}
    void stop() { if(n >= 0) ubm.stop(n); printf("ub%d stop!\n", n);}
    void sync() {ubm.sync(); printf("sync ub!\n");}
    void search() {ubm.search(); printf("searching ub...\n");}

    //ユビが見つかった時，ドッキングした時，ドッキング解除した時に呼ばれるコールバック関数
    void ubCallback(CallbackType cbt, int ubID){//ユビ状況，ユビID
        int a = 400;
        switch(cbt) {
            case UB_FOUND://見つかった時
                printf("ub%d found!\n", ubID);
                n = ubID;
                break;
              
            case UB_DOCKED://ドッキングした時
                printf("ub%d docked!\n", ubID);
              
                //過去に覚えこませたノートをリセット
                ubm.resetNotes();
              
                //タイムスタンプ(ms),強さ(0~10)をいれて，覚えさせるノートを追加
                ubm.addNote(0, 7);
                //Note構造体を使ってもOK
                Note note;
                note.timeStamp = 200;
                note.intensity = 3;
                ubm.addNote(note);
                
                //追加順は時系列じゃなくてもOK
                ubm.addNote(200*2, 1);
                ubm.addNote(200*3, 5);
                ubm.addNote(200*4, 8);
                ubm.addNote(200*5, 1);
                ubm.addNote(200*6, 3);
                ubm.addNote(200*7, 1);
                ubm.addNote(200*8, 3);
                ubm.addNote(200*9, 7);
                ubm.addNote(200*10, 1);
                ubm.addNote(200*11, 6);
                ubm.addNote(200*12, 9);
                ubm.addNote(200*13, 1);
                ubm.addNote(200*14, 4);
                ubm.addNote(200*15, 5);
                
                //追加したノートをユビに送信
                ubm.sendNotes();
              
                //ループ周期をユビに送信
                ubm.sendLoop(200*16);
              
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