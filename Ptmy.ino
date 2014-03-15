#include <LiquidCrystal.h>
#include <IRremote.h>

int podstawowy=0; // zmienna do kalibracji czujnika
IRrecv irrecv(9);
decode_results results;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup()
{
	pinMode(52, OUTPUT); // beeper
	lcd.begin(16, 2);
	Serial.begin(9600);
	irrecv.enableIRIn();
	podstawowy=przygotowanie_do_pomiaru();
}

int pomiar()
{
		Serial.println("Rozpoczêto Pomiar\n");
		int tab[10]={0}; 
		for (int i = 0; i < 10; i++)
		{
			tab[i]= analogRead(A3);
			if(tab[i]-podstawowy>100){
				analogWrite(A8,255);
				analogWrite(A10,0);
			}
			else{
				analogWrite(A10,255);
				analogWrite(A8,0);
			}
			Serial.print("X");
			delay(500);
		}
		int pom=0;
		for (int i = 0; i < 10; i++)
		{
			pom+=tab[i];
		}
		pom/=10;
		Serial.print("\nZakonczono Pomiar\n");
		digitalWrite(52,1);
		delay(500);
		digitalWrite(52,1);
		delay(500);
		digitalWrite(52,1);
		delay(500);
		pom *= 0.21;
		return pom;

}

int przygotowanie_do_pomiaru(){
	Serial.print("Kalibracja Alkomatu prosze czekac\n");
	int tab[120]={0};
	for (int i=0;i<120;i++)
	{
		tab[i]=analogRead(A3);
		Serial.print("Pozosta³o czasu do kalibracji: ");
		Serial.print(120-i);
		Serial.print(" sekund\n"); 
		delay(1000);
	}
	int wart_pocz=0;
	for (int i = 0; i < 120; i++)
	{
		wart_pocz+=tab[i];
	}
	wart_pocz/=120;
}
void beep(unsigned char delayms){
  analogWrite(50, 100);      // Almost any value can be used except 0 and 255
                           // experiment to get the best tone
  delay(delayms);          // wait for a delayms ms
  analogWrite(50, 0);       // 0 turns it off
  delay(delayms);          // wait for a delayms ms   
}  
void loop() {
	
	int wynik_pomiaru=0;
	
	while (!irrecv.decode(&results)) {
		if (results.value == 123456)
		{
			wynik_pomiaru = pomiar();
		}
		else if(results.value == 1234567){
			wyslijdopc(wynik_pomiaru);
		}
		//Serial.println(results.value, HEX);
		delay(1000);
		lcd.print(results.value);
		irrecv.resume(); // Receive the next value
	}

	//Serial.println(analogRead(A3));
	delay(1000);
	//Serial.println("wypisz");
}

void wyslijdopc(int wynik_pomiaru ){
	Serial.write(wynik_pomiaru);
}

