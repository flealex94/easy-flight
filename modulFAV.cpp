#include "stdio.h"
#include "string.h"
#include "malloc.h"
#include "stdlib.h"
#include <time.h>
#include <ctype.h>
FILE* log;
FILE* flaero;
FILE* marfuri;
FILE* batch;
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
char* copiaza(char* string) {
	char* nou = (char*)malloc(sizeof(char)*(strlen(string) + 1));
	strcpy(nou, string);
	return nou;
}
void ftimestamp() {
	time_t ltime; /* calendar time */
	ltime = time(NULL); /* get current cal time */
	char* time;
	time = copiaza(asctime(localtime(&ltime)));
	time[strlen(time) - 1] = '\0';
	fprintf(log, "%s: ", time);
}
void echo(char* msg) {
	printf(msg);
	ftimestamp();
	fprintf(log, msg);
}
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
void initPicker() { //aici se initializeaza pickerul cu comenzi si functiile aferente
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
//interpretor de comenzi
void interp(char* comline) {
	//primeste un linia cu intreaga comanda ca si string
	char* aux = (char*)malloc(strlen(comline) + 1); strcpy(aux, comline);
	//concateneaza numele comenzii si cel al modulului pentru a putea cauta functia 
	char* com = upcase(strtok(aux, " \t")); aux = upcase(strtok(nullptr, " \t"));
	strcat(com, aux);
	char* data = strtok(nullptr, ""); //ce mai ramane din string reprezinta date de intrare pentru functie
	picker.getfunk(com)(data);
}

struct linieFAV {
	char* idAeronava;
	enum { PAS, CAR }tipAero;
	short int nrLocuri;
	double greutateMaxima;
};
struct NodFAV {
	linieFAV* infoUtil;
	NodFAV* next;
	NodFAV* prev;
};
NodFAV* listaDublaCirculara = nullptr;

linieFAV* creareFAV(char* idA, int tipA, int nrL, float grM) {
	linieFAV* fav = (linieFAV*)malloc(sizeof(linieFAV));
	fav->idAeronava = copiaza(idA);
	switch (tipA) {
		case 0: fav->tipAero = fav->PAS; break;
		case 1: fav->tipAero = fav->CAR; break;
	}
	fav->nrLocuri = nrL;
	fav->greutateMaxima = grM;
	return fav;
}
NodFAV* creareNodFAV(linieFAV* fav) {
	NodFAV* nou = nullptr;
	nou = (NodFAV*)malloc(sizeof(NodFAV));
	nou->infoUtil = fav;
	nou->next = nou->prev = nullptr;
	return nou;
}
void inserareFAV(NodFAV* nod) {
	if (listaDublaCirculara == nullptr) {
		listaDublaCirculara = nod;
		listaDublaCirculara->next = listaDublaCirculara->prev = listaDublaCirculara;
	} else {
		nod->next = listaDublaCirculara;
		listaDublaCirculara->prev->next = nod;
		nod->prev = listaDublaCirculara->prev;
		listaDublaCirculara->prev = nod;
	}
}
void initializareFAV() {
	flaero = fopen("flaero.txt", "r");
	if (!flaero) {
		printf("Nu se poate deschide fisierul!\n");
	} else {
		int tipAero, nrLocuri; float greutateMaxima;
		char idAeronava[20];
		fscanf(flaero, "%s", idAeronava);
		while (!feof(flaero)) {
			fscanf(flaero, "%d", &tipAero);
			fscanf(flaero, "%d", &nrLocuri);
			fscanf(flaero, "%f", &greutateMaxima);
			linieFAV* fav = creareFAV(idAeronava, tipAero, nrLocuri, greutateMaxima);
			NodFAV* nod = creareNodFAV(fav);
			inserareFAV(nod);

			fscanf(flaero, "%s", idAeronava);
		}
	}
}
NodFAV* cautaElementFAV(char* id) {
	if (listaDublaCirculara != nullptr) {
		NodFAV* origine = listaDublaCirculara;
		do {
			if (strcmp(listaDublaCirculara->infoUtil->idAeronava, id) == 0)
				return listaDublaCirculara;
			listaDublaCirculara = listaDublaCirculara->next;
		} while (listaDublaCirculara != origine);
	}

	return nullptr;
}
void afisareFAV() {
	if (listaDublaCirculara != nullptr) {
		NodFAV* origine = listaDublaCirculara;
		do {
			linieFAV* linie = listaDublaCirculara->infoUtil;
			printf(" %s %lf %i %d\n", linie->idAeronava, linie->greutateMaxima, linie->tipAero, linie->nrLocuri);
			listaDublaCirculara = listaDublaCirculara->next;
		} while (listaDublaCirculara != origine);
	} else printf("\nLista goala\n");
}
void afisareInversFAV() {
	if (listaDublaCirculara != nullptr) {
		NodFAV* origine = listaDublaCirculara;
		do {
			linieFAV* linie = listaDublaCirculara->infoUtil;
			printf(" %s %lf %i %d\n", linie->idAeronava, linie->greutateMaxima, linie->tipAero, linie->nrLocuri);
			listaDublaCirculara = listaDublaCirculara->prev;
		} while (listaDublaCirculara != origine);
	} else printf("\nLista goala\n");
}
void findFAV(char* id) {
	NodFAV* elem = cautaElementFAV(id);
	if (elem != nullptr) {
		linieFAV* linie = elem->infoUtil;
		char msg[100];
		sprintf(msg, "\t  %s %lf %i %d\n", linie->idAeronava, linie->greutateMaxima, linie->tipAero, linie->nrLocuri);
		echo(msg);
	} else echo("\nNu exista elementul\n");
}

void addFAV(char* DATE) {
	char* dat = copiaza(DATE);
	char* aux;
	char* idA = strtok(dat, " \t");;
	int tipA; int nrL; float grM;
	aux = strtok(NULL, " \t");
	tipA = atoi(aux);
	aux = strtok(NULL, " \t");
	nrL = atoi(aux);
	aux = strtok(NULL, " \t");
	grM = atof(aux);

	linieFAV* fav = creareFAV(idA, tipA, nrL, grM);
	NodFAV* nod = creareNodFAV(fav);
	inserareFAV(nod);
	echo("Avion adaugat!\n");
}

void updateFAV(char* DATE) {
	char* dat = copiaza(DATE);
	char* id = strtok(dat, " \t");
	NodFAV* elem = cautaElementFAV(id);
	if (elem != nullptr) {
		linieFAV* linie = elem->infoUtil;
		free(linie->idAeronava);
		linie->idAeronava = copiaza(id);
		char* aux = strtok(NULL, " \t");
		int tipA = atoi(aux);
		switch (tipA) {
			case 0: linie->tipAero = linie->PAS; break;
			case 1: linie->tipAero = linie->CAR; break;
		}
		aux = strtok(NULL, " \t");
		linie->nrLocuri = atoi(aux);
		aux = strtok(NULL, " \t");
		linie->greutateMaxima = atof(aux);
		echo("Date avion actualizate!\n");
	} else echo("\nElementul nu a fost gasit!\n");
}

void deleteFAV(char* id) {
	NodFAV* elem = cautaElementFAV(id);
	if (elem) {
		listaDublaCirculara = elem->next;
		listaDublaCirculara->prev = elem->prev;
		elem->prev->next = listaDublaCirculara;
		free(elem->infoUtil->idAeronava);
		free(elem->infoUtil);
		free(elem);
		echo("Avion sters!\n");
	} else echo("\nNu exista elementul\n");
}


void main() {
	log = fopen("log.txt", "wt");
	initializareFAV();
	afisareFAV();
	findFAV("zxcv");
	addFAV("aeronava4 1 34 456");
	updateFAV("aeronava4 1 36 345");
	afisareFAV();
	fclose(log);
}
