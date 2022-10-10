#include <iostream>
#include <string>
using namespace std;

char *direcoes[4] = {(char *)"Direita", (char *)"Esquerda", (char *)"Frente", (char *)"Tras"};

int converteSensor(int valor, int min, int max) {
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

int leSensor(char *sensor) {
	int valor;                                              // valor lido do sensor
	cout << "Digite o valor do sensor " << sensor << ": ";  // pede para o usuário digitar o valor do sensor
	cin >> valor;                                           // lê o valor do sensor(entrada do usuário)
	return valor;                                           // retorna o valor lido
}

bool leComando() {
	char resposta;                                         // resposta do usuário
	std::cout << "Deseja continuar o mapeamento? (s/n) ";  // mensagem para o usuário
	std::cin >> resposta;                                  // leitura da resposta do usuário
	return resposta == 's';                                // retorna verdadeiro se a resposta for 's'
}

int armazenaValor(int valor, int *vetor, int max, int posicao) {
	if (posicao < max) {         // verifica se a posição é válida
		vetor[posicao] = valor;  // armazena o valor no vetor
		return posicao + 1;      // retorna a próxima posição
	}
	return posicao;  // retorna a posição atual (não alterada)
}

char *direcaoMenorCaminho(int *vetor, int *distancia) {
	int index = 0;         // indice do vetor
	int menor = vetor[0];  // menor valor do vetor, inicialmente o primeiro valor para comparar

	for (int i = 1; i < 4; i++) {  // percorre o vetor
		if (vetor[i] < menor) {    // verifica se o valor atual é menor que o menor valor
			menor = vetor[i];      // atualiza o menor valor
			index = i;             // atualiza o indice do menor valor
		}
	}

	*distancia = menor;  // atualiza a variável distancia com o menor valor

	return index < 4 ? direcoes[index] : (char *)"Nao encontrado";  // retorna a direção do menor valor
}

int dirige(int *v, int maxv) {
	int maxVetor = maxv;    // tamanho máximo do vetor
	int *vetorMov = v;      // vetor de movimentos
	int posAtualVetor = 0;  // posição atual do vetor de movimentos
	int dirigindo = 1;      // flag para saber se o robô está dirigindo

	while (dirigindo) {                                                                // enquanto o robô estiver dirigindo
		printf("Mapeamento #%d\n", posAtualVetor + 1);                                 // imprime o número do mapeamento
		for (int i = 0; i < 4; i++) {                                                  // percorre as direções
			int medida = leSensor(direcoes[i]);                                        // le o sensor
			medida = converteSensor(medida, 0, 100);                                   // converte o valor do sensor para o intervalo de 0 a 100
			posAtualVetor = armazenaValor(medida, vetorMov, maxVetor, posAtualVetor);  // armazena o valor no vetor
		}
		dirigindo = leComando();  // verifica se o usuário deseja continuar
	}
	return posAtualVetor;  // retorna a posição atual do vetor
}

// O trecho abaixo irá utilizar as funções acima para ler os sensores e o
// movimento do robô e no final percorrer o vetor e mostrar o movimento a cada
// direção baseado na maior distância a cada movimento
void percorre(int *v, int tamPercorrido) {
	int *vetorMov = v;  // vetor de movimentos
	int maiorDir = 0;   // maior valor da direção

	for (int i = 0; i < tamPercorrido; i += 4) {                         // percorre o vetor de movimentos
		char *direcao = direcaoMenorCaminho(&(vetorMov[i]), &maiorDir);  // calcula a direção do menor caminho

		// foi utilizado a variavel global "direcoes" para mostrar a direção de forma mais amigável
		// o numero do mapeamento é calculado dividindo o indice do vetor pelo tamanho de cada mapeamento
		printf("#%d - Direcao: %s, Distancia: %d\n", i / 4 + 1, direcao, maiorDir);  // imprime a direção e a maior distância
	}
}

int main(int argc, char **argv) {
	int maxVetor = 100;          // tamanho máximo do vetor
	int vetorMov[maxVetor * 4];  // vetor de movimentos
	int posAtualVet = 0;         // posição atual do vetor de movimentos

	posAtualVet = dirige(vetorMov, maxVetor);  // dirige o robô e retorna a posição atual do vetor de movimentos
	percorre(vetorMov, posAtualVet);           // percorre o vetor de movimentos e mostra o movimento a cada direção

	return 0;  // fim do programa
}