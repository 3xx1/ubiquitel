void parsePacket() {
  packetSize = udp.parsePacket();
  //親機から受信
  if (packetSize){
      udp.read((char *)packet, sizeof(long)*1000);
    
      switch(packet[0]) {
        case SYNC_UB:
        gtime = 0;
        break;
        case PLAY_UB:
        if(!isPlaying) isPlaying =true;
        break;
        case PAUSE_UB:
        if(isPlaying) isPlaying =false;
        break;
        case STOP_UB:
        if(isPlaying) isPlaying =false;
        stopMotor();
        now = notes[0].sp;
        next = 0;
        break;
        case SET_LOOP:
        setLoop();
        break;
        case SET_NOTE:
        setNote();
        break;
        case RESET_NOTE:
        resetNote();
        break;
        case SEARCH_UB:
        ubmip = udp.remoteIP();
        sendData(UB_FOUND);
        break;
    }
  }
}

void sendData(const CallbackType cbt) {
    CallbackType ubf = cbt;
    udp.beginPacket(ubmip, 6340);
    udp.write((char *)&ubf, sizeof(ubf));
    udp.endPacket();
}
