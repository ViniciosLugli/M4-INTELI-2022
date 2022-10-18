#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

DHT_Unified dht(2, DHT22);              // instância o dht22
sensors_event_t dht_event_temperature;  // cria a variavel que armazena os eventos de temperatura
sensors_event_t dht_event_humity;       // cria a variavel que armazena os eventos de humidade

enum AirQuality { IDEAL, OKAY, ALERT };
enum Temperature { VERY_COLD, COLD, NORMAL, HOT, VERY_HOT };

struct LifeQualityData {
	AirQuality air_quality;
	Temperature temperature;
};

class LifeQuality {
public:
	static LifeQualityData check(float temperature, float humity) {
		LifeQualityData data;

		if (temperature <= 18) {
			data.temperature = Temperature::VERY_COLD;
		} else if (temperature < 25) {
			data.temperature = Temperature::COLD;
		} else if (temperature < 30) {
			data.temperature = Temperature::NORMAL;
		} else if (temperature < 34) {
			data.temperature = Temperature::HOT;
		} else {
			data.temperature = Temperature::VERY_HOT;
		}

		if (humity > 50) {
			data.air_quality = AirQuality::IDEAL;
		} else if (humity > 30) {
			data.air_quality = AirQuality::OKAY;
		} else {
			data.air_quality = AirQuality::ALERT;
		}

		return data;
	}
};

void setup() {
	Serial.begin(115200);
	dht.begin();  // inicia o dht22
}

void loop() {
	dht.temperature().getEvent(&dht_event_temperature);  // lê a temperatura e armazena o evento de resposta
	dht.humidity().getEvent(&dht_event_humity);          // lê a umidade e armazena o evento de resposta

	if (isnan(dht_event_temperature.temperature) || isnan(dht_event_humity.relative_humidity)) {  // verifica se o valor é valido
		Serial.println("Não foi possível resgatar a informação de temperatura ou umidade");
	} else {
		LifeQualityData data = LifeQuality::check(dht_event_temperature.temperature, dht_event_humity.relative_humidity);

		Serial.println("Umidade: " + String(dht_event_humity.relative_humidity) + "%");

		switch (data.air_quality) {
			case AirQuality::IDEAL:
				Serial.println("Qualidade do ar: Ideal");
				break;
			case AirQuality::OKAY:
				Serial.println("Qualidade do ar: Ok");
				break;
			case AirQuality::ALERT:
				Serial.println("Qualidade do ar: Alerta");
				break;
		}

		Serial.println("Temperatura: " + String(dht_event_temperature.temperature) + "ºC");

		switch (data.temperature) {
			case Temperature::VERY_COLD:
				Serial.println("Temperatura: Muito frio");
				break;
			case Temperature::COLD:
				Serial.println("Temperatura: Frio");
				break;
			case Temperature::NORMAL:
				Serial.println("Temperatura: Normal");
				break;
			case Temperature::HOT:
				Serial.println("Temperatura: Quente");
				break;
			case Temperature::VERY_HOT:
				Serial.println("Temperatura: Muito quente");
				break;
		}
	}

	Serial.println("\n");

	delay(1000);
}