#include <iostream>

using namespace std;

int main(void) {
	int valores[4];

	for (int i = 0; i < 4; i++) {
		printf("Digite o %dº valor: ", i + 1);

		scanf("%d", &valores[i]);
	}

	for (int i = 0; i < 4; i++) {
		printf("O %dº valor é: %s\n", i + 1, valores[i] % 2 == 0 ? "par" : "ímpar");
	}

	return 0;
}