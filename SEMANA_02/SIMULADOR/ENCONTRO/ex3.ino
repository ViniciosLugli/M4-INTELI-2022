class Led {
public:
	Led(int pin, byte heartz) {
		// armazena os atributos e inicializa o pino

		this->_pin = pin;
		this->_heartz = heartz;

		pinMode(pin, OUTPUT);
		digitalWrite(pin, LOW);
	}

	// inverte o estado do led
	void toggle() { digitalWrite(this->_pin, !digitalRead(this->_pin)); }

	void process() {
		// verifica se no tempo atual, o led deve ser alterado o estado.
		// é utilizado a divisão por 2, já que o ciclo do led contém o tempo desligado.
		if (millis() - this->_lastMillis > ((1000 / this->_heartz) / 2)) {
			this->_lastMillis = millis();
			this->toggle();
		}
	}

private:
	int _pin;
	byte _heartz;

	unsigned long _lastMillis;
};

Led led1(19, 6);
Led led2(18, 2);

void setup() {}

void loop() {
	led1.process();
	led2.process();
}