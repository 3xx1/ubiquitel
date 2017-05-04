void checkDock() {
  int tmpState = digitalRead(dock);
  if(dockState != tmpState) {
    dockState = tmpState;
    switch(dockState) {
      case LOW:
      sendData(UB_DOCKED);
      break;
      case HIGH:
      sendData(UB_UNDOCKED);
      break;
    }
  }
}

