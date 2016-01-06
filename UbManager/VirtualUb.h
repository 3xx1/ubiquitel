#ifndef VirtualUb_h
#define VirtualUb_h

#include "ub.h"

class VirtualUb {
  public:
  volatile int active;

  VirtualUb(){active = 0; startServer(); mfound = false;};
  ~VirtualUb(){stopServer();};
  void docked();
  void undocked();
  void stopServer();
  

  private:
  char ubmip[16];
  bool mfound;
  pthread_t thread;
  static void *threadFunction(void *data);
  void startServer();
  void sendData(void *d, int size);

};
#endif
