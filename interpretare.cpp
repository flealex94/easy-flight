#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>
FILE* log;
void echo(char* msg) {
	printf(msg);
	fprintf(log, msg);
}
struct picker {
	char* comenzi[25];
	void(*functii[26])(char*);
	int getpoz(char* com, char** comenzi) {
		int poz = 0;
		while (comenzi[poz] != '\0' && strcmp(com, comenzi[poz]) != 0 && poz < 25)
			poz++;
		if (comenzi[poz] == '\0') poz = 25;//
		return poz;
	}
	void(*getfunk(char* com))(char*) {
		return functii[getpoz(com, comenzi)];
	}
}picker;

void notfound(char* somestring) {
	printf("404: nu exista functia!\n");
}
void somefunk(char* somestring) {
	printf("somefunk: %s\n", somestring);
}
void otherfunk(char* somestring) {
	printf("otherfunk: %s\n", somestring);
}
void thefunk(char* somestring) {
	printf("thefunk: %s\n", somestring);
}
void initpicker() { //aici se initializeaza pickerul cu comenzi si functiile aferente
	for (int i = 0; i < 25; i++) {
		picker.comenzi[i] = nullptr; //(char*)malloc(7);
		picker.functii[i] = nullptr;
	}
	picker.functii[25] = notfound;
	//am adaugat 3 functii dummy la vector, cu denumiri random
	picker.comenzi[0] = new char[7];
	strcpy(picker.comenzi[0], "asdf");
	picker.functii[0] = somefunk;

	picker.comenzi[1] = new char[7];
	strcpy(picker.comenzi[1], "qwer");
	picker.functii[1] = otherfunk;

	picker.comenzi[2] = new char[7];
	strcpy(picker.comenzi[2], "zxcv");
	picker.functii[2] = thefunk;
	//pe masura ce vor fi gata, trebuie adaugate functiile!

}
char* upcase(char* string) {
	char* upcase = (char*)malloc(strlen(string) + 1);
	char* ptr = upcase;
	while (*string)
		*(ptr++) = toupper(*(string++));
	*ptr = '\0';
	return upcase;
}
char* copiaza(char* string) {
	char* nou = (char*)malloc(sizeof(char)*(strlen(string) + 1));
	strcpy(nou, string);
	return nou;
}
//interpretator de comenzi
void interp(char* comline) {
	//primeste un linia cu intreaga comanda ca si string
	char* aux = (char*)malloc(strlen(comline) + 1); strcpy(aux, comline);
	//concateneaza numele comenzii si cel al modulului pentru a putea cauta functia 
	char* com = upcase(strtok(aux, " \t")); aux = upcase(strtok(nullptr, " \t"));
	strcat(com, aux);
	char* data = strtok(nullptr, ""); //ce mai ramane din string reprezinta date de intrare pentru functie
	picker.getfunk(com)(data);
}

int main() {
	initpicker();

	interp("qw er omg");
	return 0;
}