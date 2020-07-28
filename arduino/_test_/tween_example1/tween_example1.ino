
// import biblioteki
// link do biblioteki:https://github.com/WesWedding/TweenDuino
// link do dodatkowej bibiloteki kotrej potrzebuje TweenDuino: https://drive.google.com/uc?export=download&id=0B9Zobp2aWUKzR0FXYnFMTkQwWHM
#include <TweenDuino.h>

// wartosc ktora animujemy
float value = 20.0;

// wskaznik do obiektu typu Tween - to nam "animuje"
TweenDuino::Tween *tween;

void setup() {
    Serial.begin(9600);
    
    // Animacja od 20 (value) do 255 przez 10 sek (10 tys. ms) - to UL na koncu mowi 
    // Arduino że ma traktować ta wartośc jako unsigned long - czyli taki int ale wiekszy :)
    tween = TweenDuino::Tween::to(value, 10000UL, 255);
    
    Serial.print("Values: ");
}
void loop() {

    // trzeba zawsze update w loop robic
    tween->update(millis());
    
    if (tween->isActive()) { // ta dziwna "->" strzałka to tak jak "." nie wnikajmy... ;)
      // to sie wykonuje podczas animacji
      // tu mozna ustawiac predkosc wiatraczka / to co na displayu etc
      Serial.println(value);
    }else if(tween->isComplete()){
      // to sie wykonuje po zakonczeniu animacji
      // to miejsce do tego by np. odaplic kolejna animacje do kolejnej wartosci
      // czyli zwikszamy licznik, wybieramy niowa wartosc z tablicy i do niej animujemy
      // i tak az nie doejdziemy do konca tablicy, i od poczatku...
      // analogicznie jak to robilismy ostatnio
      Serial.println("DONE!");
      // kolejna animacja od obecnej wartosci value do 10 przez 1 sek..
      // tylko oczywiscie to bedzie musialo isc z tablicy i sie zmieniac dla kolejnych krokow
      tween = TweenDuino::Tween::to(value, 1000UL, 10);
    }

    
}
