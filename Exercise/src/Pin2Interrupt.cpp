#include "Pin2Interrupt.h"
#include "Arduino.h"
using namespace std;

int Get_Interrupt_Of_Pin(int pin)
{
  switch(pin)
  {
    case 2:return 0;
    case 3:return 1;
    case 18:return 5;
    case 19:return 4;
    case 20:return 3;
    case 21:return 2;
    default:return -1;
  }
}
