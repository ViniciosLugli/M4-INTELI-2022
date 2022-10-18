class Led {
public:
	Led(int pin) {
		// armazena os atributos e inicializa o pino
		this->_pin = pin;

		pinMode(pin, OUTPUT);
		digitalWrite(pin, LOW);
	}

	void on() { digitalWrite(this->_pin, HIGH); }

	void off() { digitalWrite(this->_pin, LOW); }

private:
	int _pin;
};

class Counter {
public:
	static void increment() { Counter::count++; }

	// para retornar o valor da unidade, basta pegar o resto da divisão por 10
	static int getUnit() { return Counter::count % 10; }
	static int getTen() { return (Counter::count / 10) % 10; }

	static int getCount() { return Counter::count; }

	static int count;
};

Led ledRed(19);    // unidade impar
Led ledBlue(18);   // unidade par
Led ledGreen(5);   // dezena impar
Led ledYellow(4);  // dezena par

int Counter::count = 0;  // inicializa o contador

void setup() { Serial.begin(115200); }

void loop() {
	// incrementa o contador
	Counter::increment();
	Serial.println("Counter: " + String(Counter::getCount()));

	// verifica se a unidade é par ou impar
	if (Counter::getUnit() % 2 == 0) {
		ledRed.off();
		ledBlue.on();
	} else {
		ledRed.on();
		ledBlue.off();
	}

	// verifica se a dezena é par ou impar
	if (Counter::getTen() % 2 == 0) {
		ledGreen.off();
		ledYellow.on();
	} else {
		ledGreen.on();
		ledYellow.off();
	}

	delay(1000);
}