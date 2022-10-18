


class Leds{
	//private: Implicito se nao disser nada
		int pinLed1 = 0;
		int pinLed2 = 0;
	public:
		Leds(int l1,int l2){
			pinLed1 = l1;
			pinLed2 = l2;
		  	pinMode(pinLed1,OUTPUT);
	  		pinMode(pinLed2,OUTPUT);		
		}
		void acende(){
			digitalWrite(pinLed1,HIGH);
			digitalWrite(pinLed2,HIGH);
			
		}
		void apaga(){
			digitalWrite(pinLed1,LOW);
			digitalWrite(pinLed2,LOW);
		}
};

Leds *l1 = NULL;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Hello, ESP32-S3!");
  l1 = new Leds(1,2);
}

void loop() {
  // put your main code here, to run repeatedly:
  l1->acende();
  delay(500); // this speeds up the simulation
  l1->apaga();
  delay(500);
}
