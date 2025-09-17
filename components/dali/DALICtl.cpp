#include "DALICtl.h"

Dali dali;
uint8_t DALI_Addr[64] = {0};
uint8_t DALI_NUM = 0;

uint8_t bus_is_high() {
  return digitalRead(RX_PIN); //slow version
}

//use bus
void bus_set_low() {
  digitalWrite(TX_PIN,LOW); //opto slow version
}

//release bus
void bus_set_high() {
  digitalWrite(TX_PIN,HIGH); //opto slow version
}

void ARDUINO_ISR_ATTR onTimer() {
  dali.timer();
}

hw_timer_t *timer = NULL;

// Forward declarations for C++ wrapper functions
void DALI_Init_CPP();
void Blinking_ALL_CPP();
void Luminaire_Brightness_CPP(uint8_t Light, uint8_t addr);
void Lighten_ALL_CPP();
void Extinguish_ALL_CPP();
void Scan_DALI_addr_ALL_CPP();
void Assign_new_address_ALL_CPP();

// C++ wrapper functions
void DALI_Init_CPP() {
  printf("Initializing DALI bus \r\n");
  //setup RX/TX pin
  pinMode(RX_PIN, INPUT);
  pinMode(TX_PIN, OUTPUT);
  
  timer = timerBegin(9600000);
  timerAttachInterrupt(timer, &onTimer);
  timerAlarm(timer, 1000, true, 0);

  dali.begin(bus_is_high, bus_set_high, bus_set_low);
  Scan_DALI_addr_ALL_CPP();
  if(DALI_NUM == 0)
    Assign_new_address_ALL_CPP();
}

void Blinking_ALL_CPP() {                                                        
  printf("Running: Blinking all lamps\r\n");
  dali.set_level(254);
  delay(500);
  dali.set_level(0);
  delay(500);
}

void Luminaire_Brightness_CPP(uint8_t Light, uint8_t addr) {                     
  printf("Running: Set the brightness of the fixture at address %d to %d %%\r\n", addr, Light);
  dali.set_level(Light, addr);
}

void Lighten_ALL_CPP() {                                                          
  printf("Running: Light all lamps\r\n");
  dali.set_level(254);
}

void Extinguish_ALL_CPP() {                                                       
  printf("Running: Extinguish all lamps\r\n");
  dali.set_level(0);
}

int16_t DALICheckStatus(uint8_t addr)
{
  return dali.cmd(0x11, addr);
}

void Scan_DALI_addr_ALL_CPP() {                                                   
  printf("Running: Scan all devices\r\n");
  DALI_NUM = 0;
  for (uint8_t addr = 0; addr < 64; addr++) {
    dali.cmd(0x11, addr); // Query Status
    delay(100);
    if (dali.cmd(0x90, addr) == 0xFF) { // Query Ballast Failure
      DALI_Addr[DALI_NUM] = addr;
      DALI_NUM ++;
      printf("Found device at address %d\r\n", addr);
      dali.set_level(100, addr);
      delay(100);
      dali.set_level(0, addr);
    }
  }
  printf("End scan,%d devices were scanned\r\n",DALI_NUM);
}

void Assign_new_address_ALL_CPP(){                                                  
  printf("Running: Assign new addresses to all devices\r\n");   
  printf("Might need a couple of runs to find all lamps ...\r\n");
  printf("Be patient, this takes a while ...\r\n");
  uint8_t cnt = dali.commission(0xff); //init_arg=0b11111111 : all without short address  
  printf("DONE, assigned %d new short addresses\r\n",cnt);
  Scan_DALI_addr_ALL_CPP();
}

#ifdef __cplusplus
extern "C" {
#endif

void DALI_Init() {
  DALI_Init_CPP();
}

void Blinking_ALL() {
  Blinking_ALL_CPP();
}
void Luminaire_Brightness(uint8_t Light, uint8_t addr) {
  Luminaire_Brightness_CPP(Light, addr);
}

void Lighten_ALL() {
  Lighten_ALL_CPP();
}
void Extinguish_ALL() {
  Extinguish_ALL_CPP();
}

void Scan_DALI_addr_ALL() {
  Scan_DALI_addr_ALL_CPP();
}
void Delete_DALI_addr_ALL() {
  // Implementation needed - can be added later
  printf("Delete function not implemented yet\r\n");
}

void Assign_new_address_ALL() {
  Assign_new_address_ALL_CPP();
}

#ifdef __cplusplus
}
#endif