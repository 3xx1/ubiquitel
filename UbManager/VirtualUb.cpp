#include "VirtualUb.h"

void VirtualUb::startServer() {
    int result;
    printf("start server\n");
    if (!active) {
        active = 1;
        result = pthread_create(&thread, NULL, threadFunction, (void *)this);
    }
  
}

void VirtualUb::stopServer() {
    int result;
    
    if (active) {
        active = 0;
        result = pthread_join( thread, NULL );
    }
}

void VirtualUb::docked() {
  CallbackType docked = UB_DOCKED;
  sendData(&docked, sizeof(int));
}

void VirtualUb::undocked() {
  CallbackType undocked = UB_UNDOCKED;
  sendData(&undocked, sizeof(int));
}

void VirtualUb::sendData(void *d, int size) {
  if(!mfound) return;
  int sock;
  struct sockaddr_in addr;

  sock = socket(AF_INET, SOCK_DGRAM, 0);
  addr.sin_family = AF_INET;
  addr.sin_port = htons(6340);
  addr.sin_addr.s_addr = inet_addr(ubmip);

  sendto(sock, d, size, 0, (struct sockaddr *)&addr, sizeof(addr));
  close(sock);
}

void *VirtualUb::threadFunction(void *data) {
  VirtualUb *vub = (VirtualUb *)data;
  int sock, val;
  struct sockaddr_in addr, senderinfo;

  sock = socket(AF_INET, SOCK_DGRAM, 0);
  addr.sin_family = AF_INET;
  addr.sin_port = htons(6341);
  addr.sin_addr.s_addr = INADDR_ANY;
  bind(sock, (struct sockaddr *)&addr, sizeof(addr));
  val =1;
  ioctl(sock, FIONBIO, &val);
  
  while (vub->active) {
    int n, type;
    socklen_t addrlen;
    addrlen = sizeof(senderinfo);
    n = recvfrom(sock, &type, sizeof(type), 0,
    		 (struct sockaddr *)&senderinfo, &addrlen);
    if(n == -1) {
      usleep(1000);
      continue;
    }

    switch(type) {
      case SYNC_UB:
        printf("ub syncronized!\n");
        break;
      case PLAY_UB:
        printf("ub play!\n");
        break;
      case PAUSE_UB:
        printf("ub pause!\n");
        break;
      case STOP_UB:
        printf("ub stop!\n");
        break;
      case SET_LOOP:
        printf("ub loop period!\n");
        break;
      case SET_NOTE:
        printf("ub note set!\n");
        break;
      case RESET_NOTE:
        printf("ub note reset!\n");
        break;
      case SEARCH_UB:
        vub->mfound = true;
        CallbackType ubf = UB_FOUND;
        strcpy(vub->ubmip, inet_ntoa(senderinfo.sin_addr));
        printf("ubm address:%s!\n",vub->ubmip);
        vub->sendData(&ubf, sizeof(int));
        break;
    }
  }
  close(sock);
  pthread_exit(NULL);
  printf("no active!\n");
}
