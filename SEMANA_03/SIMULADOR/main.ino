
/*
Construa um programa que seja capaz de ler os dados de luz de
um sensor LDR, converter o valor lido em úm número de 0 a 15 e use esse valor para acender 4 LEDs, cada um para um dígito binário que irá corresponder
a conversão do número (0 a 15). Além disso:
1 - Faça uma função que, a cada novo número medido, faça a contagem binária nos leds, pausadamente, de 0
até o número medido.
2 - Acrescente "toque" usando um Buzzer que corresponda a uma nota diferente para cada número. Ao contar de 0 até o número, o
buzzer irá tocando todas as notas correspondentes.
3 - Acrescente um "push button", tal que, ao ser pressionado, armazene o valor medido em um vetor.
4 - Acrescente um SEGUNDO push button que, ao ser pressionado, toque todos os tons correspondentes ao número armazenado.
5 - Ponto de desafio: Acrescente um display ao seu sistema que vá indicando as medidas que vao ocorrendo, assim como as notas tocadas
*/

#include <Adafruit_SSD1306.h>

#define OLED_ENDERECO 0x3c   // OLED display endereço
#define LARGURA_DISPLAY 128  // OLED display largura, em pixels
#define ALTURA_DISPLAY 64    // OLED display altura, em pixels
#define MAX_VALOR_VETOR 12   // Tamanho máximo do vetor

class Console {
public:
	static void escreverFormatado(const char* texto, ...) {
		char buffer[128];  // buffer para armazenar o texto formatado

		va_list args;                   // lista de argumentos
		va_start(args, texto);          // inicia a lista de argumentos
		vsprintf(buffer, texto, args);  // formata o texto
		va_end(args);                   // finaliza a lista de argumentos

		/*
		está sendo utilizado o ets_printf para escrever no console
		pois não é seguro o uso do serial.Print durante interrupções,
		é causado um crash(abort) no ESP32, já que é uma função
		que utiliza de um buffer interno fechado para escrita.

		como pode ser visto na referência:
		https://esp32.com/viewtopic.php?f=13&t=3748&p=17131
		*/
		ets_printf(F(buffer));  // escreve o texto formatado no buffer
	}
};

class LDR {
public:
	LDR(int pin) { this->_pin = pin; }             // construtor
	int read() { return analogRead(this->_pin); }  // leitura do sensor

private:
	int _pin;  // pino de leitura do sensor
};

class Buzzer {
public:
	Buzzer(int porta, int canal) {
		this->_porta = porta;
		this->_canal = canal;

		ledcAttachPin(this->_porta, this->_canal);  // associa o pino ao canal
	}

	void tocarNota(int nota, int duracao) {
		Console::escreverFormatado("\ttocando nota %d por %d ms\n", nota, duracao);
		ledcWriteTone(this->_canal, nota);  // toca a nota
		delay(duracao);                     // aguarda a duração da nota
		ledcWriteTone(this->_canal, 0);     // desliga o buzzer
	}

private:
	int _porta;  // pino do buzzer
	int _canal;  // canal do buzzer
};

class Botao {
public:
	Botao(int pin) {
		this->_porta = pin;  // pino do botão

		// configura o pino como entrada com pull-up, para usar o resistor interno
		pinMode(this->_porta, INPUT_PULLUP);
	}

	bool pressionado() { return digitalRead(this->_porta) == LOW; }  // verifica se o botão está pressionado
	void adicionarInterrupcao(void (*callback)()) { attachInterrupt(digitalPinToInterrupt(this->_porta), callback, FALLING); }

private:
	int _porta;  // pino do botão
};

class Led {
public:
	Led(int porta) {                    // construtor
		this->_porta = porta;           // armazena o pino do LED
		pinMode(this->_porta, OUTPUT);  // configura o pino como saída
	}

	void ligar() { digitalWrite(this->_porta, HIGH); }    // acende o led
	void desligar() { digitalWrite(this->_porta, LOW); }  // apaga o led

private:
	int _porta;  // pino do led
};

class BitLeds {
public:
	BitLeds(Led* leds) {
		this->_leds = leds;  // armazena os leds
	}

	void definirValor(int valorDecimal) {
		Console::escreverFormatado("\tValor decimal para binario: %d\n", valorDecimal);
		// a repetição do for é dado pelo tamanho da lista de leds
		for (int i = 0; i < sizeof(this->_leds); i++) {  // percorre todos os leds
			/*
			verifica se o bit está ligado, usando o operador de deslocamento de bits
			o operador de deslocamento de bits é o operador <<, que desloca os bits para a esquerda
			logo o valor 1 deslocado para a esquerda 3 vezes, resulta no valor 8, que é 2 elevado a 3
			com isso, é possível verificar se o bit está ligado, usando o operador de AND ( &
			o operador & faz a comparação bit a bit, se o resultado for diferente de zero, o bit está ligado

			exemplo: 8 & 5 = 0, pois 1000 & 0101 = 0000

			processo completo:
			valorDecimal = 5

			led 0:
			5 & (1 << 0) = 5 & 1 = 1, pois 0101 & 0001 = 0001

			led 1:
			5 & (1 << 1) = 5 & 2 = 0, pois 0101 & 0010 = 0000

			led 2:
			5 & (1 << 2) = 5 & 4 = 4, pois 0101 & 0100 = 0100

			led 3:
			5 & (1 << 3) = 5 & 8 = 0, pois 0101 & 1000 = 0000

			o if verifica se o resultado é diferente de zero, se for diferente de zero, o led é ligado
			*/
			if (bitRead(valorDecimal, i) == 1) {
				this->_leds[i].ligar();  // liga o led
			} else {
				this->_leds[i].desligar();  // desliga o led
			}
		}
	}

private:
	Led* _leds;  // vetor de leds
};

class Vetor {
public:
	static int armazenaValor(int valor, int* vetor, int max, int posicao) {
		if (posicao < max) {         // verifica se a posição é válida
			vetor[posicao] = valor;  // armazena o valor no vetor
			return posicao + 1;      // retorna a próxima posição
		}
		return posicao;  // retorna a posição atual (não alterada)
	}
};

byte valorLDR = 0;  // valor do sensor LDR

LDR ldr(15);          // cria o objeto do sensor LDR
Buzzer buzzer(2, 0);  // cria o objeto do buzzer
BitLeds bitLeds(new Led[4]{
	// cria o objeto dos leds
	Led(27),  // led 0
	Led(14),  // led 1
	Led(12),  // led 2
	Led(13)   // led 3
});
Botao botaoSalvar(26);                                      // cria o objeto do botão de salvar
Botao botaoLer(25);                                         // cria o objeto do botão de ler os valores
Adafruit_SSD1306 display(LARGURA_DISPLAY, ALTURA_DISPLAY);  // cria o objeto do display

int* valoresLDR;            // cria o vetor de valores ldr
int posicaoValoresLDR = 0;  // posição atual do vetor

bool fazerLeitura = false;   // flag para fazer a leitura dos valores do LDR
bool salvarLeitura = false;  // flag para salvar ovalor do LDR

void callbackSalvarValor() {
	salvarLeitura = true;  // ativa a flag para salvar o valor do LDR
}

void callbackLerValores() {
	fazerLeitura = true;  // ativa a flag para fazer a leitura dos valores do LDR
}

void setup() {
	Serial.begin(115200);                                   // inicia terminal com baud rate 115200
	valoresLDR = new int[MAX_VALOR_VETOR];                  // cria o vetor de valores
	botaoSalvar.adicionarInterrupcao(callbackSalvarValor);  // adiciona a interrupção do botão salvar
	botaoLer.adicionarInterrupcao(callbackLerValores);      // adiciona a interrupção do botão ler

	if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // inicia o display
		Serial.println(F("Falha ao iniciar o display"));
		for (;;)
			;  // loop infinito
	} else {
		display.clearDisplay();              // limpa o display
		display.setTextColor(WHITE, BLACK);  // define a cor do texto, e a cor do fundo para sobrepor
		display.setTextSize(1);              // define o tamanho da fonte
		display.setCursor(0, 0);             // define a posição do cursor
		display.println(F("Iniciando..."));  // escreve na tela
		display.display();                   // atualiza o display
	}
	delay(2000);             // aguarda 2 segundos
	display.clearDisplay();  // limpa o display
}

void loop() {
	display.clearDisplay();  // limpa o display

	/*lê o valor do sensor LDR e converte para um valor de 0 a 15
	o valor do sensor LDR varia de 4095 a 0, que é o valor da resolução do ADC
	para converter para um valor de 0 a 15, é necessário dividir o valor do sensor por 273,4375
	4095 / 15 = 273,4375
	ou seja, o valor do sensor é dividido por 273,4375, e o resultado é arredondado para o inteiro mais próximo
	*/
	// Arredonda o valor e subtrai o valor do LDR do valor máximo para inverter o valor

	valorLDR = 15 - round(ldr.read() / 273.4375);
	Console::escreverFormatado("Valor LDR: %i\n", valorLDR);
	display.setCursor(LARGURA_DISPLAY / 2 - 40, ALTURA_DISPLAY / 2 - 15);  // define a posição do cursor
	display.print(F("Valor LDR: "));                                       // escreve o valor do LDR no display
	display.print(String(valorLDR));
	display.display();                     // atualiza o display
	for (int i = 0; i <= valorLDR; i++) {  // percorre todos os leds
		bitLeds.definirValor(i);           // define o valor do led

		int nota = (i * 100) + 100;
		display.setCursor(LARGURA_DISPLAY / 2 - 52, ALTURA_DISPLAY / 2 + 5);  // define a posição do cursor
		display.print(F("Tocando nota: "));                                   // escreve o valor da nota no display
		display.print(String(nota));
		display.display();            // atualiza o display
		buzzer.tocarNota(nota, 200);  // toca a nota por 200ms, valores de tonalidade de 100 a 1600

		delay(300);  // aguarda 300ms

		Console::escreverFormatado("\n");  // pula uma linha
	}

	if (salvarLeitura) {
		salvarLeitura = false;
		// armazena o valor do LDR no vetor de valores
		posicaoValoresLDR = Vetor::armazenaValor(valorLDR, valoresLDR, MAX_VALOR_VETOR, posicaoValoresLDR);
		Console::escreverFormatado("-> Valor armazenado: %i\n", (int)valorLDR);
		display.clearDisplay();
		display.setCursor(LARGURA_DISPLAY / 2 - 60, ALTURA_DISPLAY / 2);  // define a posição do cursor
		display.print(F("Valor armazenado: "));                           // escreve o valor do LDR no display
		display.print(String(valorLDR));
		display.display();  // atualiza o display
		delay(500);
	}

	if (fazerLeitura) {        // verifica se deve fazer a leitura dos valores
		fazerLeitura = false;  // reseta a flag
		display.clearDisplay();
		display.setCursor(LARGURA_DISPLAY / 2 - 40, ALTURA_DISPLAY / 2 - 20);  // define a posição do cursor
		display.print(F("Lendo valores..."));                                  // escreve o valor do LDR no display
		display.display();                                                     // atualiza o display
		delay(500);
		if (posicaoValoresLDR > 0) {  // verifica se há valores armazenados
			Console::escreverFormatado("-> Valores armazenados: ");
			for (int i = 0; i < posicaoValoresLDR; i++) {          // percorre o vetor
				Console::escreverFormatado("%i ", valoresLDR[i]);  // imprime o valor
			}
			Console::escreverFormatado("\nTocando notas dos valores salvos...\n");
			// toca o buzzer com base no valor do vetor
			for (int i = 0; i < posicaoValoresLDR; i++) {  // percorre o vetor
				int nota = (valoresLDR[i] * 100) + 100;
				display.setCursor(LARGURA_DISPLAY / 2 - 52, ALTURA_DISPLAY / 2);  // define a posição do cursor
				display.print(F("Tocando nota: "));                               // escreve o valor da nota no display
				display.print(String(nota));
				display.setCursor(LARGURA_DISPLAY / 2 - 52, ALTURA_DISPLAY / 2 + 10);  // define a posição do cursor
				display.print(F("Valor LDR: "));                                       // escreve o valor do LDR no display
				display.print(String(valorLDR));
				display.display();            // atualiza o display
				buzzer.tocarNota(nota, 200);  // toca o buzzer por 200ms
				delay(300);                   // aguarda 300ms
			}

			posicaoValoresLDR = 0;  // reseta a posição do vetor para armazenar novos valores
		} else {
			Console::escreverFormatado("Nenhum valor armazenado\n");
		}
	}

	delay(10);  // delay para não sobrecarregar o processador do simulador
}
