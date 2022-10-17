#include <iostream>
int sala01[100];
int ultimaPos01 = 0;
int sala02[100];
int ultimaPos02 = 0;
int codigoItem = 0;

void lerLoop(){
	int sala = 0;
	int resp = 1;	
	while(resp){
		printf("Por favor digite o codigo do item:\n");
		scanf("%i",&codigoItem);
		
		printf("Digite para que sala vai esse item:\n");
		scanf("%i",&sala);
		
		switch(sala){
			case 1: sala01[ultimaPos01] = codigoItem; 
					ultimaPos01++;
			        break;
			case 2: sala02[ultimaPos02] = codigoItem; 
					ultimaPos02++;
					break;
		}
		printf("Deseja inserir mais algum item ? (0 - Nao/  1 - Sim)\n");
		scanf("%i",&resp);
	}	
}

int vetorResposta[5] = {0,0,0,0,0};
void buscaItem(int *vetor){
	puts("Digite o numero que deseja:\n");
	scanf("%i",&codigoItem);
	for(int i=0 ; i < ultimaPos01; i++){
		if(sala01[i]==codigoItem){
			vetor[0]= 1;
		}		
	}
	for(int i=0 ; i < ultimaPos02; i++){
		if(sala02[i]==codigoItem){
			vetor[1]= 1;
		}		
	}
}
int main(int argc, char** argv) {
	
	
	lerLoop();
	buscaItem(vetorResposta);
	
	
	for(int i=0; i<5 ; i++){
		printf("Numeros encontrados na sala %i = %i\n",i,vetorResposta[i]);
	}
	return 0;
}