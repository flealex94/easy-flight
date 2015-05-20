#include "stdio.h"
#include "string.h"
#include "malloc.h"
#include "stdlib.h"
char* copiaza(char* string) {
	char* nou = (char*)malloc(sizeof(char)*(strlen(string) + 1));
	strcpy(nou, string);
	return nou;
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
void initializareFAV(FILE*& file) {
	if (!file) {
		printf("Nu se poate deschide fisierul!\n");
	} else {
		int tipAero, nrLocuri; float greutateMaxima;
		char idAeronava[20];
		fscanf(file, "%s", idAeronava);
		while (!feof(file)) {
			fscanf(file, "%d", &tipAero);
			fscanf(file, "%d", &nrLocuri);
			fscanf(file, "%f", &greutateMaxima);
			linieFAV* fav = creareFAV(idAeronava, tipAero, nrLocuri, greutateMaxima);
			NodFAV* nod = creareNodFAV(fav);
			inserareFAV(nod);

			fscanf(file, "%s", idAeronava);
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

void findFAV(char* id) {
	NodFAV* elem = cautaElementFAV(id);
	if (elem != nullptr) {
		linieFAV* linie = elem->infoUtil;
		printf("\n  %s %lf %i %d\n", linie->idAeronava, linie->greutateMaxima, linie->tipAero, linie->nrLocuri);
	} else printf("\nNu exista elementul\n");
}

void updateFAV(char* DATE) {
	char* dat = copiaza(DATE);
	char* id = strtok(dat, " \t");
	NodFAV* elem = cautaElementFAV(id);
	if (elem != nullptr) {
		linieFAV* linie = elem->infoUtil;
		free(linie->idAeronava);
		linie->idAeronava = copiaza(strtok(NULL, " \t"));
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
	} else printf("\nElementul nu a fost gasit!\n");
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
	} else printf("\nNu exista elementul\n");
}

void addFAV(char* DATE) {
	char* dat = copiaza(DATE);
	char* aux;
	char* idA = strtok(DATE, " \t");;
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
}


void main() {
	FILE* file;
	file = fopen("flaero.txt", "r");;
	initializareFAV(file);
	afisareFAV();
	//findFAV("qwer");
	/*addFAV("aeronava4 1 34 456");
	updateFAV("aeronava4", "1 36 345");
	afisareFAV();*/

}
