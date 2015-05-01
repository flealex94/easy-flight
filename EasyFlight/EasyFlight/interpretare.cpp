#include <stdio.h>
#include <string.h>
#include <malloc.h>
struct picker {
	char* comenzi[25];
	void(*functii[26])(char*);
	int getPoz(char* com, char** comenzi){
		int poz = 0;
		while (comenzi[poz]!='\0' && strcmp(com, comenzi[poz]) != 0 && poz < 25)
			poz++;
		if (comenzi[poz] == '\0') poz = 25;//
		return poz;
	}
	void(*getFunk(char* com))(char*){
		return functii[getPoz(com, comenzi)];
	}
}picker;

void someFunk(char* somestring){
	printf("someFunk: %s\n", somestring);
}
void otherFunk(char* somestring){
	printf("otherFunk: %s\n", somestring);
}
void theFunk(char* somestring){
	printf("theFunk: %s\n", somestring);
}
void notFound(char* somestring){
	printf("404: nu exista functia!\n");
}

void initPicker(){ //aici se initializeaza pickerul cu comenzi si functiile aferente
	for (int i = 0; i < 25; i++){
		picker.comenzi[i] = nullptr; //(char*)malloc(7);
		picker.functii[i] = nullptr;
	}
	picker.functii[25] = notFound;
	//am adaugat 3 functii dummy la vector, cu denumiri random
		picker.comenzi[0] = new char[7];
		strcpy(picker.comenzi[0], "asdf");
		picker.functii[0] = someFunk;

		picker.comenzi[1] = new char[7];
		strcpy(picker.comenzi[1], "QWER");
		picker.functii[1] = otherFunk;

		picker.comenzi[2] = new char[7];
		strcpy(picker.comenzi[2], "zxcv");
		picker.functii[2] = theFunk;
	//pe masura ce vor fi gata, trebuie adaugate functiile!

}
 //interpretator de comenzi
void interp(char* comLine){
	//primeste un linia cu intreaga comanda ca si string
	char* aux=(char*)malloc(strlen(comLine)+1);strcpy(aux, comLine); 
	//concateneaza numele comenzii si cel al modulului pentru a putea cauta functia 
	char* com = strtok(aux, " \t"); aux = strtok(nullptr, " \t");
	strcat(com, aux); 
	char* data = strtok(nullptr,""); //ce mai ramane din string reprezinta date de intrare pentru functie
	picker.getFunk(com)(data);
}

int main(){
	initPicker();
	interp("QW		 ER asdf");
	//interp("o functie care nu exista mesaj test");
	
	return 0;
}