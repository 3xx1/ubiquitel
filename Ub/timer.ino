//タイマ割り込み
void timer()
{
  if (isPlaying)
    stepTime();
  gtime++;
  if (gtime == playtime)
  {
    packet[1] = -1;
    playUb();
  }
  checkTestTap();
}
