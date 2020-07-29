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
  displayController.setTextAlign( ALIGN_FREE );


}

void loop() {

  //char buff[16];
  //sprintf(buff, "%ld", long(value));

  //displayController.writeText( buff );
  displayController.writeNumber( value, 2 );
  displayController.update();

  value += 0.1;

}
