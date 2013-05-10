#define SVECKA 11
#define STRASI 7
#define ZVOK 5
#define SENSOR 0
#define RAZDALJA 80  //cm
#define BERI_X_KRAT 3
#define ZVOK_F 38
#define ZAKASNITEV 25

long previousMillis = 0;
long closeReadings = 0;
int val = 0; 
int distance = 0;

void setup() {
  pinMode(SVECKA, OUTPUT);
	pinMode(STRASI, OUTPUT);
	pinMode(ZVOK, OUTPUT);
//	pinMode(SENSOR, INPUT);
        Serial.begin(9600);  
	delay(1000); // Allow the proximity sensor to initialize
}

int irvalue()
  {
      int value;
      int i;

      value = 0;
      for (i=0; i<BERI_X_KRAT; i++) 
        {
          value += analogRead(SENSOR);
          delay(50);
        }
    return value/(BERI_X_KRAT); 
  }

void loop() {
//        val = irvalue();
        val = analogRead(SENSOR);
        
        // send analog value out:
        // Calculate linear slope of reading (thanks, Acroname!): za GP2D120
        //distance = ((2914 / (val + 5)) - 1);
        Serial.print("Razdalja v vrednosti =\t");
        Serial.print(val);
        Serial.print("\t");
        distance = 9462 / (val - 17);
        //distance = 9462 / (val - 16.92);
        Serial.print("Razdalja v cm =\t");
        Serial.println(distance);
        // wait 10ms for ADC to reset before next reading:
        delay(10);
	if (distance < RAZDALJA) // Is someone close?
	{
		closeReadings++;
		if (closeReadings >= BERI_X_KRAT) // require n consecutive "close" readings before going into EVIL mode. This prevents little blips of the buzzer.
			{
				digitalWrite(SVECKA, LOW);
				digitalWrite(STRASI, HIGH);
				tone(ZVOK, ZVOK_F);
			}
                 Serial.print("buuuu   ");
	}
	else
	{ //regular candle flicker. Based on Arduino example BlinkWithoutDelay.
		noTone(ZVOK);
		digitalWrite(STRASI, LOW);
		closeReadings = 0;
		unsigned long currentMillis = millis();
                Serial.print("čakam ...   ");
		if(currentMillis - previousMillis > ZAKASNITEV)
		{
			previousMillis = currentMillis;
			analogWrite(SVECKA, random(0, 256));
		}
	}
                Serial.println(distance);

}
