//セットアップモードには、ホームルーターに接続できない時、またはボタン長押しで切り替わる。
//セットアップモードでは、ホームルーターのSSID、パスワードを設定できる（PCのブラウザで192.168.4.1にアクセス）。
//また、ボタンで指のテスト動作ができる。
void checkTestTap()
{
  if (!digitalRead(testButton) && !testTap && setupMode) {
    testTap = true;
  }
  if (testTap) {
    if (stepCount < 32) {
      stepMotor();
      stepCount++;
    }
    else if (stepCount < 45) {
      stopMotor();
      stepCount++;
    } else {
      stepCount = 0;
      testTap = false;
    }
  }
}

void checkSetupMode()
{
  if (!digitalRead(testButton) && !setupMode) {
    if (!pushed) {
      pushedTime = millis();
      pushed = true;
    } else if ((millis() - pushedTime) > 2000) {
      setupMode = true;
      setupServer();
    }
  } else if (setupMode) {
    loopServer();
  } else {
    pushed = false;
  }
}
