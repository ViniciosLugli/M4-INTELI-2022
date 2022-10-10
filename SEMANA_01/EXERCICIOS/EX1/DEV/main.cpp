#include <iostream>
#include <string>

// Definindo o tipo de execução
//	#define DEBUG

// define funções para debug
#ifdef DEBUG
	#define DBG_INFO(...) printf("INFO: " __VA_ARGS__)
#else
	#define DBG_INFO(...)
#endif

// usa o namespace std para não precisar acessar o namespace em cada chamada
using namespace std;

// variavel global
char *direcoes[4] = {(char *)"Direita", (char *)"Esquerda", (char *)"Frente", (char *)"Tras"};

class Utils {
public:
	static int percentual(int valor, int min, int max) {
		/*
		exemplo: converteSensor(30, 0, 100) = 30
		calculo para normalizar e converter para o intervalo:
		(30 - 0) * 100 / (100 - 0) = percentual
		(30 - 0) * 100 / 100 = percentual
		30 * 100 / 100 = percentual
		30 = percentual
		*/
		return (valor - min) * 100 / (max - min);
	}

	static int armazenaValorVetor(int valor, int *vetor, int max, int posicao) {
		if (posicao < max) {         // verifica se a posição é válida
			vetor[posicao] = valor;  // armazena o valor no vetor
			return posicao + 1;      // retorna a próxima posição
		}
		return posicao;  // retorna a posição atual (não alterada)
	}

	static char *direcaoMaiorCaminho(int *vetor, int *distancia) {
		int index = 0;         // indice do vetor
		int maior = vetor[0];  // maior valor do vetor, inicialmente o primeiro valor para comparar

		// percorre o vetor
		for (int i = 1; i < 4; i++) {
			if (vetor[i] > maior) {  // verifica se o valor atual é maior que o maior valor
				maior = vetor[i];    // atualiza o maior valor
				index = i;           // atualiza o indice do maior valor
			}
		}

		*distancia = maior;                                             // armazena o maior valor no ponteiro
		return index < 4 ? direcoes[index] : (char *)"Nao encontrado";  // retorna a direção do maior va
	}
};

template <typename TipoEntrada>
class Entrada {
public:
	char *texto;
	Entrada(char *texto) {
		// inicializa o texto da entrada
		this->texto = texto;
	}

	TipoEntrada ler() {
		TipoEntrada valor;      // variavel para armazenar o valor lido
		printf("%s: ", texto);  // imprime o texto
		cin >> valor;           // lê o valor
		return valor;           // retorna o valor lido
	}
};

struct Leitura {
	int valor;
	int percentual;
	int min, max;

	Leitura(int valor, int min, int max) {
		// inicializa as variáveis
		this->valor = valor;
		this->min = min;
		this->max = max;
		this->percentual = Utils::percentual(valor, min, max);  // calcula o percentual
	}
};

class Sensor {
public:
	int min;
	int max;
	char *direcao;

	Sensor(int min, int max, char *direcao) {
		// inicializa os atributos
		this->min = min;
		this->max = max;
		this->direcao = direcao;
	}

	Leitura ler() {
		char texto[50];                                                // define o texto a ser exibido na entrada
		sprintf(texto, "Digite o valor do sensor %s", this->direcao);  // concatena o texto com a direção do sensor

		Entrada<int> entradaSensor(texto);  // cria o objeto para ler o valor do sensor
		int valor = entradaSensor.ler();    // faz a leitura do valor

		return Leitura(valor, min, max);  // retorna a estrutura de dados
	}
};

class Robo {
public:
	int *vetorDistancias;
	int posicao;
	bool dirigindo;

	Robo() {
		this->vetorDistancias = new int[4];  // aloca o vetor de distancias
		this->posicao = 0;                   // inicializa a posição do vetor
		this->dirigindo = true;              // inicializa o estado do robô
	}

	void lerSensores() {
		this->posicao = 0;  // inicializa a posição do vetor

		for (int i = 0; i < 4; i++) {            // percorre os sensores
			Sensor sensor(0, 100, direcoes[i]);  // cria o objeto sensor
			Leitura leitura = sensor.ler();      // faz a leitura do sensor

			// armazena o valor do sensor no vetor de distancias
			this->posicao = Utils::armazenaValorVetor(leitura.valor, this->vetorDistancias, 4, this->posicao);
		}
	}

	void verificaContinuar() {
	verificar_continuar_repetir:
		Entrada<char> entrada_continuar((char *)"Deseja continuar o mapeamento? (s/n)");  // cria o objeto para ler a resposta
		char resposta = entrada_continuar.ler();                                          // faz a leitura da resposta

		switch (resposta) {  // verifica a resposta
			case 's':        // se for sim
				this->dirigindo = true;
				break;
			case 'n':  // se for não
				this->dirigindo = false;
				break;
			default:  // se for qualquer outra coisa
				printf("Resposta inválida, digite novamente.\n");
				goto verificar_continuar_repetir;
				break;
		}
	}

	int dirigir(int *vetorSaida, int maxVetor) {
		int posAtualVetor = 0;  // posição atual do vetor de movimentos

		while (this->dirigindo) {                               // enquanto o robô estiver dirigindo
			printf("Mapeamento #%d\n", posAtualVetor / 4 + 1);  // exibe o número do mapeamento
			this->lerSensores();                                // le o sensor

			for (int i = 0; i < 4; i++) {  // percorre o vetor de distancias
				posAtualVetor = Utils::armazenaValorVetor(this->vetorDistancias[i], vetorSaida, maxVetor,
				                                          posAtualVetor);  // armazena o valor no vetor de movimentos
			}

			this->verificaContinuar();  // verifica se o usuário deseja continuar
		}

		return posAtualVetor;  // retorna a posição atual do vetor
	}

	void percorre(int *v, int tamPercorrido) {
		int *vetorMov = v;  // vetor de movimentos

		for (int i = 0; i < tamPercorrido; i += 4) {  // percorre o vetor de movimentos
			// exibe os valores do vetor de movimentos em debug
			DBG_INFO("i: %d, vetorMov[i]: %d, vetorMov[i + 1]: %d, vetorMov[i + 2]: %d, vetorMov[i + 3]: %d\n", i, vetorMov[i], vetorMov[i + 1],
			         vetorMov[i + 2], vetorMov[i + 3]);

			int maiorDir;                                                         // variavel para armazenar maior valor do sensor
			char *direcao = Utils::direcaoMaiorCaminho(&vetorMov[i], &maiorDir);  // pega a direção do maior valor

			// O numero do mapeamento atual é dado
			// pela divisão da posição atual do vetor de movimentos pelo tamanho do vetor de distancias
			printf("#%d - Direcao: %s, Distancia: %d\n", i / 4 + 1, direcao, maiorDir);  // imprime a direção e a maior distância
		}
	}
};

int main(int argc, char **argv) {
	int maxVetor = 100;          // tamanho máximo do vetor
	int vetorMov[maxVetor * 4];  // vetor de movimentos
	int posAtualVet = 0;         // posição atual do vetor de movimentos

	// Mock de um robô que faz o mapeamento
	Robo robo;  // cria o objeto robo

	posAtualVet = robo.dirigir(vetorMov, maxVetor);  // faz o mapeamento
	DBG_INFO("Tamanho do vetor de movimentos: %d\n", posAtualVet);
	robo.percorre(vetorMov, posAtualVet);  // percorre o mapeamento

	return 0;  // fim do programa
}