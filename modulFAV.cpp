#include "stdio.h"
#include "string.h"
#include "malloc.h"
#include "stdlib.h"

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
	fav->idAeronava = (char*)malloc((strlen(idA) + 1));
	strcpy(fav->idAeronava, idA);
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
void inserareFAV(NodFAV* listaDublaCirculara,NodFAV* nod) {
	if (listaDublaCirculara == nullptr) {
		listaDublaCirculara = nod;
		listaDublaCirculara->next = listaDublaCirculara->prev = listaDublaCirculara;
	}
	else {
		nod->next = listaDublaCirculara;
		listaDublaCirculara->prev->next = nod;
		nod->prev = listaDublaCirculara->prev;
		listaDublaCirculara->prev = nod;
	}
}
void initializareFAV(FILE*& file) {
	if (!file) {
		printf("Nu se poate deschide fisierul!\n");
	}
	else {
		int tipAero, nrLocuri; float greutateMaxima;
		char idAeronava[20];
		fscanf(file, "%s", idAeronava);
		while (!feof(file)) {
			fscanf(file, "%d", &tipAero);
			fscanf(file, "%d", &nrLocuri);
			fscanf(file, "%f", &greutateMaxima);
			linieFAV* fav = nullptr; fav = creareFAV(idAeronava, tipAero, nrLocuri, greutateMaxima);
			NodFAV* nod = nullptr; nod = creareNodFAV(fav);
			inserareFAV(listaDublaCirculara, nod);

			fscanf(file, "%s", idAeronava);
		}
	}
}
NodFAV* cautaElementFAV(char* id) {
	if (listaDublaCirculara) {
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
	}
	else printf("\nLista goala\n");
}

void findFAV(char* id) {
	linieFAV* elem = nullptr; elem = cautaElementFAV(id)->infoUtil;
	if (elem != nullptr) printf("\n  %s %lf %i %d\n", elem->idAeronava, elem->greutateMaxima, elem->tipAero, elem->nrLocuri);
	else printf("\nNu exista elementul\n");
}

void updateFAV(char* id,char* Date) {
	char* da = (char*)malloc(sizeof(char)*strlen(Date));
	strcpy(da, Date);
	linieFAV* fav = (linieFAV*)malloc(sizeof(linieFAV));
	while (listaDublaCirculara->next && (strcmp(listaDublaCirculara->infoUtil->idAeronava, id) != 0))
				listaDublaCirculara = listaDublaCirculara->next;
	if (strcmp(listaDublaCirculara->infoUtil->idAeronava, id) == 0)
	{
		char* id;
		char s[] = " ";
		id = strtok(da, s);
		//???Conversie! fav->CAR=id;
		//listaDublaCirculara->infoUtil->CAR =atoi (id);
		while (id != NULL)
		{
			id = strtok(NULL, " ");
			listaDublaCirculara->infoUtil->nrLocuri = atoi(id);
			id = strtok(NULL, " ");
			listaDublaCirculara->infoUtil->greutateMaxima = atof(id);
			id = strtok(NULL, " ");

		}
	}
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
	}
	else printf("\nNu exista elementul\n");
}

void addFAV(char* Date) {
	char* da = (char*)malloc(sizeof(char)*strlen(Date));
	strcpy(da, Date);
	linieFAV* fav = (linieFAV*)malloc(sizeof(linieFAV));
	char* id;
	char s[] = " ";
	id = strtok(da, s);
	fav->idAeronava = (char*)malloc(sizeof(char)*(strlen(id) + 1));
	strcpy(fav->idAeronava, id);
	while (id != NULL)
	{
		printf("%s ", fav->idAeronava);
		id = strtok(NULL, " ");
		//???Conversie! fav->CAR=id;
		id = strtok(NULL, " ");
		fav->nrLocuri =atoi(id);
		printf("%d ", fav->nrLocuri);
		id = strtok(NULL, " ");
		fav->greutateMaxima = atof(id);
		printf("%lf ", fav->greutateMaxima);
		id = strtok(NULL, " ");

	}
	NodFAV* nod = nullptr;
	nod = creareNodFAV(fav);
	inserareFAV(listaDublaCirculara, nod);
	
}


void main() {
	FILE* file;
	file = fopen("flaero.txt", "r");;
	initializareFAV(file);
	afisareFAV();
	//findFAV("qwer");
	addFAV("aeronava4 1 34 456");
	updateFAV("aeronava4", "1 36 345");
	afisareFAV();
	
}
