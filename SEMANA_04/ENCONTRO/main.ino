#define HC_SR04_ECHO_PIN 10
#define HC_SR04_TRIGGER_PIN 18
#define SERVO_MOTOR_PWM_PIN 3

class HC_SR04 {
public:
	HC_SR04(int echoPin, int triggerPin) {
		this->m_echoPin = echoPin;
		this->m_triggerPin = triggerPin;

		pinMode(this->m_echoPin, INPUT);
		pinMode(this->m_triggerPin, OUTPUT);
	}

	float getDistance() {
		// Faz o pulse de 10us no pino de trigger
		digitalWrite(this->m_triggerPin, LOW);
		delayMicroseconds(2);
		digitalWrite(this->m_triggerPin, HIGH);
		delayMicroseconds(10);
		digitalWrite(this->m_triggerPin, LOW);

		// Utiliza a função pulseIn para medir o tempo que o pino de echo ficou em HIGH ( função nativa arduino)
		float duration = pulseIn(this->m_echoPin, HIGH, 10000);

		// Calcula a distância em centimetros
		float distance = duration * 0.034 / 2;

		return distance;  // Retorna em centimetros a distância
	}

	float meanDistance(int n) {
		float sum = 0;
		for (int i = 0; i < n; i++) {
			sum += this->getDistance();
		}
		return sum / n;
	}

private:
	int m_echoPin;
	int m_triggerPin;
};

class Servo {
public:
	Servo(int pwmPin) {
		this->m_pwmPin = pwmPin;
		ledcSetup(0, 50, 16);  // 50 Hz, 8 bits

		ledcAttachPin(this->m_pwmPin, 0);  // Pino do servo no canal 0
	}

	void setAngle(int angle) {
		// Converte o ângulo para o valor de duty cycle
		int dutyCycle = map(angle, 0, 180, 1638, 7864);
		ledcWrite(0, dutyCycle);
	}

private:
	int m_pwmPin;
};

HC_SR04 hc_sr04(HC_SR04_ECHO_PIN, HC_SR04_TRIGGER_PIN);
Servo servo(SERVO_MOTOR_PWM_PIN);

void setup() { Serial.begin(115200); }

void loop() {
	float distance = hc_sr04.getDistance();

	Serial.println("Distância: " + String(distance) + "cm");

	servo.setAngle(map(distance, 30, 100, 0, 180));

	delay(500);
}