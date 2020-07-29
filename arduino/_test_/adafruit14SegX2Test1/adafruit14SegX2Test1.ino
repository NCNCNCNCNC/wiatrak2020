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

  displayController.writeText( "HELLO" );
  //displayController.writeNumber( long(value) );
  displayController.update();

  value += 0.1;

}
