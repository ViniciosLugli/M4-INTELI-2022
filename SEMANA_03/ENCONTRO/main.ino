#define PORT_LDR 2
#define PORT_BUZZER 13
#define PORT_LED 38

class LDR {
public:
	LDR(int pin) { this->_pin = pin; }

	int read() { return analogRead(this->_pin); }

private:
	int _pin;
};

class Buzzer {
public:
	Buzzer(int pin) {
		this->_pin = pin;
		ledcSetup(0, 5000, 8);  // channel, freq, resolution default
		ledcAttachPin(this->_pin, 0);
	}

	void play(int duty, int duration) {
		ledcWrite(0, duty);
		delay(duration);
		ledcWrite(0, 0);
	}

private:
	int _pin;
};

class Led {
public:
	Led(int pin) {
		this->_pin = pin;
		pinMode(this->_pin, OUTPUT);
	}

	void toggle() { digitalWrite(this->_pin, !digitalRead(this->_pin)); }

private:
	int _pin;
};

LDR ldr(PORT_LDR);
Buzzer buzzer(PORT_BUZZER);
Led led(PORT_LED);

void setup() { Serial.begin(115200); }

void loop() {
	int buffer = ldr.read();
	Serial.println("Readed value:" + buffer);
	buzzer.play(buffer / 4, 150);
	led.toggle();
	delay(100);
}