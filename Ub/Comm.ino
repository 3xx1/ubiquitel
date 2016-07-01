void parsePacket() {
  packetSize = udp.parsePacket();
  //親機から受信
  if (packetSize){
      udp.read((char *)packet, sizeof(long)*1000);
      switch(packet[0]) {
        case SYNC_UB:
        syncUb();
        break;
        case PLAY_UB:
        playUb();
        break;
        case PAUSE_UB:
        pauseUb();
        break;
        case STOP_UB:
        stopUb();
        break;
        case SET_LOOP:
        setLoop();
        break;
        case SET_NOTE:
        setNote();
        break;
        case RESET_NOTE:
        stopUb();
        resetNote();
        break;
        case SEARCH_UB:
        notifyUb();
        break;
        case CONFIRM:
        waiting[packet[1]] = 0;
    }
  }
}

void sendData(const CallbackType cbt) {
    CallbackType ubf = cbt;
    udp.beginPacket(ubmip, 6340);
    udp.write((char *)&ubf, sizeof(ubf));
    udp.endPacket();
    waiting[cbt] = waitingTime;
}

void waitForConfirmation() {
  for(int i=0;i<3;i++) {
    if(waiting[i] > 1) waiting[i]--;
    if((waiting[i]%resendPeriod) == 1) {
      sendData((CallbackType)i);
    }
  }
}

