#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

DHT_Unified dht(2, DHT22);  // instância o dht22
sensors_event_t dht_event;  // cria a variavel que armazena os eventos

void setup() {
	Serial.begin(115200);
	dht.begin();  // inicia o dht22
}

void loop() {
	dht.temperature().getEvent(&dht_event);  // lê a temperatura e armazena o evento de resposta

	if (isnan(dht_event.temperature)) {  // verifica se o valor é valido
		Serial.println("Não foi possível resgatar a informação de temperatura");
	} else {
		Serial.println("Temperatura: " + String(dht_event.temperature) + "ºC");
	}

	dht.humidity().getEvent(&dht_event);  // lê a umidade e armazena o evento de resposta

	if (isnan(dht_event.relative_humidity)) {  // verifica se o valor é valido
		Serial.println("Não foi possível resgatar a informação de umidade");
	} else {
		Serial.println("Umidade: " + String(dht_event.relative_humidity) + "%");
	}

	Serial.println("\n");
	delay(1000);
}