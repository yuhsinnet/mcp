////////////////////////////////PIN_IO_CONTROL//////////////////////////////////////////
#define LOW 0
#define HIGH 1

#define INPUT(port,pin) DDR ## port &= ~(1<<pin)
#define OUTPUT(port,pin) DDR ## port |= (1<<pin)
#define CLEAR(port,pin) PORT ## port &= ~(1<<pin)
#define SET(port,pin) PORT ## port |= (1<<pin)
#define TOGGLE(port,pin) PORT ## port ^= (1<<pin)
#define READ(port,pin) ((PIN ## port & (1<<pin))>>pin)
////////////////////////////////////////////////////////////////////////////////////////
 #define SETp(port,pin) port |=(1<<pin)
 #define INPUTp(port,pin) port &= ~(1<<pin)
 #define OUTPUTp(port,pin) port |= (1<<pin)
 #define CLEARp(port,pin) port &= ~(1<<pin)
 #define TOGGLEp(port,pin) port ^= (1<<pin)
 #define READp(port,pin) ((port & (1<<pin))>>pin)