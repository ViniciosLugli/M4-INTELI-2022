#include <iostream>

using namespace std;

float multiplicar(float a, float b) { return a * b; }

int main(void) {
	int numero1, numero2;

	for (int i = 0; i < 2; i++) {
		printf("Digite o %dº número: ", i + 1);

		scanf("%d", !i ? &numero1 : &numero2);
	}

	printf("O resultado da multiplicação é: %f\n", multiplicar(numero1, numero2));

	return 0;
}