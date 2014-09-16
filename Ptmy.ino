#include <IRremote.h>
#include <LiquidCrystal.h>

int podstawowy = 0; // zmienna do kalibracji czujnika
decode_results results;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup()
{
  pinMode(52, OUTPUT); // beeper
  pinMode(6, OUTPUT); // R
  pinMode(8, OUTPUT); // G
  pinMode(7, OUTPUT); // B
  lcd.begin(16, 2);
  Serial.begin(9600);
  IRrecv irrecv(9);
  irrecv.enableIRIn();
  podstawowy = przygotowanie_do_pomiaru();
}
void wyswietl_tekst(char* a, char* b) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(a);
  lcd.setCursor(0, 1);
  lcd.print(b);
}
double pomiar()
{
  wyswietl_tekst("Start pomiaru", "Prosze dmuchac");
  int tab[10] = {0};
  for (int i = 0; i < 10; i++)
  {
    tab[i] = analogRead(A3);
    if (tab[i] - podstawowy > 100) {
      analogWrite(6, 255);
      analogWrite(7, 0);
    }
    else {
      analogWrite(6, 255);
      analogWrite(7, 0);
    }
    delay(1000);
  }
  double pom = 0;
  for (int i = 0; i < 10; i++)
  {
    pom += tab[i];
  }
  pom /= 10;
  wyswietl_tekst("Zakocznono", "pomiar");
  beep(500);
  delay(100);
  beep(500);
  delay(100);
  beep(500);
  delay(100);
  return pom;
}

int przygotowanie_do_pomiaru() {
  lcd.print(" Breathanalyser");
  lcd.setCursor(6, 1);
  lcd.print("2014");
  delay(2500);
  analogWrite(6, 100);
  wyswietl_tekst("Kalibracja", "prosze czekac");
  delay(1000);
  int tab[120] = {0};
  for (int i = 0; i < 30; i++)
  {
    tab[i] = analogRead(A3);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Pozostalo");
    lcd.setCursor(0, 1);
    lcd.print(tab[i]);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Pozostalo");
    lcd.setCursor(0, 1);
    lcd.print(30 - i);
    lcd.print(" sekund");
    delay(1000);
  }
  int wart_pocz = 0;
  for (int i = 0; i < 30; i++)
  {
    wart_pocz += tab[i];
  }
  wart_pocz /= 30;
  wyswietl_tekst("Zakonczono", "konfiguracje");
  delay(1000);
}
void beep(unsigned char delayms) {
  digitalWrite(52, 100);
  delay(delayms);
  digitalWrite(52, 0);
  delay(delayms);
}
void loop() {
  IRrecv irrecv(9);
  double wynik_pomiaru = 0;

  while (1) {
    //if (results.value == 16732535) // w lewo
    //{
    analogWrite(6, 0);
	analogWrite(8, 0);
	analogWrite(7, 0);
    wynik_pomiaru = (double)(pomiar());
	//0,0997ln(x) - 0,4432
    if (wynik_pomiaru > 0.2)
    {
      analogWrite(7, 255);
      beep(2500);
    }
    else
      analogWrite(8, 255);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Dmuchles:");
    lcd.setCursor(0, 1);
	lcd.print(wynik_pomiaru);
    lcd.print(" promila");
    delay(5000);
    wyslijdopc((double)wynik_pomiaru);
    //	}
    //else if(results.value == 16742535){ //w prawo
    //wyslijdopc(wynik_pomiaru);
    //}
    delay(1000);
    irrecv.resume(); // Receive the next value
  }
}

void wyslijdopc( double val){
	int precision = 100;
    Serial.print (int(val));  //prints the int part
    Serial.print("."); // print the decimal point
    unsigned int frac;
    if(val >= 0)
      frac = (val - int(val)) * 100;
    else
       frac = (int(val)- val ) * 100;
    int frac1 = frac;
    while( frac1 /= 10 )
        precision /= 10;
    precision /= 10;
    while(  precision /= 10)
        Serial.print("0");

    Serial.println(frac,DEC);
}
