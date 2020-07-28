#include "DisplayController.h"

const char *diseaseNames[] = {
  "TUBERCULOSIS", "MEASLES", "COVID-19", "SWINE FLU", "MERS 2015"
};
int diseaseIndex = 1;
float value = 0.0;

DisplayController displayController;


void setup() {

  Serial.begin(9600);

  displayController.setup();

}

void loop() {

  char buff[32];
  sprintf(buff, "%ld", long(value));
  
  displayController.setDisplayText( buff );
  displayController.update();

  value += 10;

}
