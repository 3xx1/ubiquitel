#include "VirtualUb.h"

int main() {
  VirtualUb vub;
  char i;
  bool q = false;
  
  while(q == false) {
    scanf("%c", &i);
    switch(i) {
    case 'd':
      vub.docked();
      break;
    case 'u':
      vub.undocked();
      break;
    case 'q':
      q = true;
      break;
    default:
      break;
    }
  }
  return 0;
}
