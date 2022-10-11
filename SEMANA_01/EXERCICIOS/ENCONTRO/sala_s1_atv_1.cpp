#include <iostream>

using namespace std;

int main(void) {
	int numero;

	printf("Digite um numero: ");

	scanf("%d", &numero);

	printf("o número inserido é %s\n", numero % 2 == 0 ? "par" : "impar");

	return 0;
}