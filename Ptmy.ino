#include <LiquidCrystal.h>
#include <IRremote.h>

int podstawowy=0; // zmienna do kalibracji czujnika
decode_results results;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup()
{
	pinMode(52, OUTPUT); // beeper
	pinMode(A13, OUTPUT); // R
	pinMode(A12, OUTPUT); // G 
	pinMode(A11, OUTPUT); // B
	lcd.begin(16, 2);
	Serial.begin(9600);
	IRrecv irrecv(9);
	irrecv.enableIRIn();
	podstawowy=przygotowanie_do_pomiaru();
}
void wyswietl_tekst(char* a, char* b){
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print(a);
	lcd.setCursor(0, 1);
	lcd.print(b);
}
double pomiar()
{
	Serial.println("Rozpoczeto Pomiar");
	wyswietl_tekst("Start pomiaru", "Prosze dmuchac");
	int tab[10]={0}; 
	for (int i = 0; i < 10; i++)
	{
		tab[i]= analogRead(A3);
		wyslijdopc(tab[i]);
		if(tab[i]-podstawowy>100){
			analogWrite(A11,255);
			analogWrite(A13,0);
		}
		else{
			analogWrite(A13,255);
			analogWrite(A11,0);
		}
		delay(1000);
	}
	double pom=0;
	for (int i = 0; i < 10; i++)
	{
		pom+=tab[i];
	}
	pom/=10;
	Serial.print("Zakonczono Pomiar");
	wyswietl_tekst("Zakocznono", "pomiar");
	beep(500);
	delay(100);
	beep(500);
	delay(100);
	beep(500);
	delay(100);
	return pom;
}

int przygotowanie_do_pomiaru(){
	lcd.print(" Breathanalyser");
	lcd.setCursor(6, 1);
	lcd.print("2014");
	delay(2500);
	analogWrite(A11, 255);
	Serial.print("Kalibracja Alkomatu prosze czekac");
	wyswietl_tekst("Kalibracja","prosze czekac");
	delay(1000);
	int tab[120]={0};
	for (int i=0;i<30;i++) 
	{
		tab[i]=analogRead(A3);
		wyslijdopc(tab[i]);
				lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("Pozostalo");
		lcd.setCursor(0, 1);
		lcd.print(tab[i]);
		
		Serial.print("Pozostalo czasu do kalibracji: ");
		Serial.print(30-i);
		Serial.print(" sekund\n");
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("Pozostalo");
		lcd.setCursor(0, 1);
		lcd.print(30 - i);
		lcd.print(" sekund");
		delay(1000);
	}
	int wart_pocz=0;
	for (int i = 0; i < 30; i++)
	{
		wart_pocz+=tab[i];
	}
	wart_pocz/=30;
	wyswietl_tekst("Zakonczono", "konfiguracje");
	delay(1000);
}
void beep(unsigned char delayms){
	digitalWrite(52, 100);      // Almost any value can be used except 0 and 255
	// experiment to get the best tone
	delay(delayms);          // wait for a delayms ms
	digitalWrite(52, 0);       // 0 turns it off
	delay(delayms);          // wait for a delayms ms   
}  
void loop() {
	IRrecv irrecv(9);
	double wynik_pomiaru=0;
	if (irrecv.decode(&results)) {
			Serial.println(results.value);
		}
	while (1) {
		//if (results.value == 16732535) // w lewo
		//{
			analogWrite(A11, 0);
			int temp = pomiar();
			podstawowy *=3;
			wynik_pomiaru = (temp - podstawowy)/ 100;
			if(wynik_pomiaru > 0.2)
			{
				analogWrite(A13, 255);
				beep(2500);
			}
			else
				analogWrite(A12, 255);

			lcd.clear();
			lcd.setCursor(0,0);
			lcd.print("Dmuchles:");
			lcd.setCursor(0,1);
			if (wynik_pomiaru < 0)	wynik_pomiaru = 0;
			lcd.print(wynik_pomiaru);
			lcd.print(" promila");

			delay(5000);
			wyslijdopc(wynik_pomiaru);
	//	}
		//else if(results.value == 16742535){ //w prawo
			wyslijdopc(wynik_pomiaru);
		//}
		delay(1000);
		irrecv.resume(); // Receive the next value
	}
}

void wyslijdopc(int wynik_pomiaru ){
	Serial.write(wynik_pomiaru);
}

