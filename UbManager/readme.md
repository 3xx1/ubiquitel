#ユビマネージャクラスとサンプルコード
一応C++が得意先なのとoFでアプリを作ってるということなので、C++のソースコードです。もし気になる点があれば言ってください。
##サンプルコードの使い方
MacのターミナルでUbManagerディレクトリに移動してmake。ubiquitelフォルダをデスクトップに置いているなら、<br>
```
cd /Users/YOURNAME/Desktop/ubiquitel/UbManager/
make
```
と打ち込めば、
- UbMTest
- VUbTest

の2種類のサンプルコードの実行ファイルが生成されます。
その後、ターミナルのウィンドウをもうひとつ開き、片方でVUbTestを実行```./VUbTest```した後、もう片方でUbMTestを実行```./UbMTest```してください。

##ユビマネージャクラス(UbManager.h, UbManager.cpp)
このクラスは以下の機能を持っています。
- アプリケーションに対してネットワーク上に存在するユビの子機、また子機のドッキング状況の通知
- ユビの子機に対して再生、停止、やリズムパターンなどの指示

細かいメソッドの内容は、ソースコード(UbManager.h, UbManager.cpp, UbMTest.cpp)のコメントに書いておきました。

##バーチャルユビクラス(VirtualUb.h, VirtualUb.cpp)
このクラスはユビマネージャクラスの動作確認用に用意した仮想ユビ子機です。今のところユビマネージャとの通信部分しか実装していません。ちゃんとしたのはESP WROOM 02 上に実装予定です。

##oFで使うときは
以下の3つのファイルをXCodeプロジェクトに追加すれば十分だと思います。（多分）
- ub.h
- UbManager.h
- UbManager.cpp