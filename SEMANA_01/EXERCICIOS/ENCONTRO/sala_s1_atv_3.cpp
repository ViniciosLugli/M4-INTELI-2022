#include <iostream>

using namespace std;

bool divisivel_por_11(int numero) {
	int soma_par = 0;
	int soma_impar = 0;
	string numero_str = to_string(numero);

	for (int i = 0; i < numero_str.length(); i++) {
		if (i % 2 == 0) {
			soma_par += (int)numero_str[i];
		} else {
			soma_impar += (int)numero_str[i];
		}
	}

	return (soma_par - soma_impar) % 11 == 0;
}

int main(void) {
	int numero;
	printf("Digite um numero: ");
	scanf("%d", &numero);

	printf("O numero %d %sdivisivel por 11\n", numero, divisivel_por_11(numero) ? "é " : "nao é ");
	return 0;
}