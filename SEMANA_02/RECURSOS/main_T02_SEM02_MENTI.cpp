#include <iostream>
#define HIGH 1
#define LOW 0
#define OUTPUT 1

void pinMode(int pino,int modo){
	printf("Definindo pino %i como %i",pino,modo);
}

void digitalWrite(int pino, int valor){
	printf("pino %i escrevendo %i",pino,valor);
	
}

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

int main(int argc, char** argv) {
	
	Leds *l1 = new Leds(1,2);
	l1->acende();
	
	return 0;
}