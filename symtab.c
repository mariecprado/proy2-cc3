#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_LEN 100
#define MAX_LABEL_LEN 100
#define LONG_LABEL "012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678"

char **symtab_s;
long *symtab_n;
int label_count = 0;

void initialize(){

	symtab_s = (char**)malloc(TABLE_LEN*sizeof(char*));
	int i = 0;
	for(i=0; i < TABLE_LEN; i++){
		symtab_s[i] = (char*)malloc(MAX_LABEL_LEN*sizeof(char));
	}
	symtab_n = (long*)malloc(TABLE_LEN*sizeof(long));
}

void release(){

	free(symtab_s);
	free(symtab_n);
}

void add(char *string, long dir){
	//TODO: Implemente el metodo add agregando <string,dir> a la tabla de sinbolos en
	//la posicion que corresponda
	symtab_s[label_count] = string;
	symtab_n[label_count] = dir;
	label_count++;
}

long search(char *string){
	//TODO: Implemente el metodo search, si existe la etiqueta en la tabla, devuelva
	//su direccion, si no, deben devolver 0
	long ret = 0;
	int i = 0;
	while(i < TABLE_LEN){
		ret = strcmp(symtab_s[i], string);
		if(ret == 0){
			ret = symtab_n[i];
			i = TABLE_LEN;
		} else {
			i++;
			ret = 0;
		}
	}
	return ret;
}

int get_offset(long dir, char *string){
	//TODO: Implementen el metodo get_offset. Deben devolver a cuantas instrucciones
	//de la direccion dir, se encuentra la etiqueta string.
	//Si la etiqueta esta "arriba", este deberia ser un numero negativo.
	int offset = 0;
	int contador = 0;
	long ret = 0;
	int i = 0;
	while(i < TABLE_LEN){
		ret = strcmp(symtab_s[i], string);
		if(ret == 0){
			offset = (symtab_n[i] - dir);
			offset = offset/4;
			i = TABLE_LEN;
		} else {
			i++;
			offset = 0;
		}
	}
	return offset;
}


/*NO TOQUEN NADA A PARTIR DE AQUI*/
/*int check_add(int flag){
	char *l[] = {"main","label1","mensaje","cond","loopcito_"};
	long d[]  = {0x1122334455667788,0x0000000002244423,0x4000000000000000,
		 		0x4000000000000004,0x0000000002200423};
	int i = 0;
	int score = 0;
	do{
		add(l[i],d[i]);

	}while( ++i < 5);
	while(--i >= 0){
		int cmp_s = strcmp(l[i],symtab_s[i]);
		int cmp_n = (d[i] == symtab_n[i]);
		int cmp = 0;
		if(!cmp_s && cmp_n){
			cmp = 5;
		}
		if(flag){
			printf("%s debe ser igual a %s\n",l[i],symtab_s[i]);
			printf("0x%016lx debe ser igual a 0x%016lx (+%d)\n..........\n",d[i],symtab_n[i],cmp);
		}
		score+=cmp;
	}
	return score;
}

int check_search(int flag){
	char *l[] = {"main","label1","mensaje","cond","loopcito_","branc_ne_case","funcion",
				 "symtab","label2"};
	long d[]  = {0x40000000,0x40000004,0x40001010,0x40000100,0x48100000,
		 		 0x88500000,0x800B00D4,0x80104000,0x40001004};
	int i = 0;
	int score = 0;
	long search_value;
	int t_score;
	do{
		add(l[i],d[i]);
	}while( ++i < 9);
	while(--i >= 0){
		search_value = search(l[i]);
		t_score = 0;
		if(search_value == d[i]){
			t_score = 3;
		}
		if(flag){
			printf("Obtenido: 0x%08lx, esperado 0x%08lx (+%d)\n..........\n",search_value,d[i],t_score);
		}
		score+=t_score;
	}
	search_value = search("esta_label_no");
	if(search_value){
		t_score = 0;
	}else{
		t_score = 8;
	}
	score+=t_score;
	if(flag){
		printf("Buscando una label que no existe, devolvio %lx (+%d)\n",search_value,t_score);
	}
	return score;
}

int check_get_offset(int flag){
	char *l[] = {"main","label1","label2","label3","label4"};
	long d[]  = {0x40000000,0x4000000C,0x40000020,0x40000030,0x4000004C};
	int i = 0;
	int score = 0;
	do{
		add(l[i],d[i]);

	}while( ++i < 5);
	int offset = get_offset(0x40000000,"label1");
	int sc = 10;
	if(offset-3){
		sc = 0;
	}
	score+=sc;
	if(flag){
		printf("Respuesta esperada: 3, Respuesta obtenida: %d (+%d)\n..........\n",offset,sc);
	}
	sc = 10;
	offset = get_offset(0x40000600,"label4");
	if(offset+365){
		sc = 0;
	}
	score+=sc;
	if(flag){
		printf("Respuesta esperada: -365, Respuesta obtenida: %d (+%d)\n..........\n",offset,sc);
	}
	sc = 5;
	offset = get_offset(0x40000030,"label3");
	if(offset){
		sc = 0;
	}
	score+=sc;
	if(flag){
		printf("Respuesta esperada: 0, Respuesta obtenida: %d (+%d)\n..........\n",offset,sc);
	}
	return score;
}

void check_init(){
	int i = MAX_LABEL_LEN;
	do{
		strcpy(symtab_s[--i],LONG_LABEL);
	}while(i);
}

void main(int argc, char *argv[]){
	if(argc == 2){
		initialize();
		if(!strcmp(argv[1],"check_ej1")){
			puts("Revisando ejercicio 1...");
			check_init();
			puts("No segmentation fault... Todo Bien...!\nTotal: 15/15");
		}else if(!strcmp(argv[1],"check_ej2")){
			puts("Revisando ejercicio 2...");
			printf("Total: %d/25\n",check_add(1));
		}else if(! strcmp(argv[1],"check_ej3")){
			puts("Revisando ejercicio 3...");
			printf("Total: %d/35\n",check_search(1));
		}else if(! strcmp(argv[1],"check_ej4")){
			puts("Revisando ejercicio 4...");
			printf("Total: %d/25\n",check_get_offset(1));
		}else if(! strcmp(argv[1],"check_all")){
			int total = 15;
			check_init();
			puts("Revisando ejercicio 1... (+15)");
			release();
			initialize();
			int aux = check_add(0);
			printf("Revisando Ejercicio 2... (+%d)\n",aux);
			total+=aux;
			release();
			initialize();
			aux = check_search(0);
			printf("Revisando Ejercicio 3... (+%d)\n",aux);
			total+=aux;
			release();
			initialize();
			aux = check_get_offset(0);
			printf("Revisando Ejercicio 4... (+%d)\n",aux);
			total+=aux;
			printf("Total: %d/100\n",total);
		}else{
			puts("Comando no encontrado...");
		}
		release();
	}else{
		puts("Necesitas ingregar un parametro de ejecucion");
	}
}
*/