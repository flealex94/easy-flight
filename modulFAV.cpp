#include "stdio.h"
#include "string.h"
#include "malloc.h"

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
	nou->next = nou->prev = NULL;
	return nou;
}
void inserareFAV(NodFAV* listaDublaCirculara, NodFAV* nod) {
	if (listaDublaCirculara == NULL) {
		listaDublaCirculara = nod;
		listaDublaCirculara->next = listaDublaCirculara->prev = listaDublaCirculara;
	} else {
		nod->next = listaDublaCirculara;
		nod->prev = listaDublaCirculara->prev;
		listaDublaCirculara->prev->next = nod;
		listaDublaCirculara->prev = nod;
	}
}

void initializareFAV(FILE* file, NodFAV* listaDublaCirculara) {
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
			inserareFAV(listaDublaCirculara, nod);

			fscanf(file, "%s", idAeronava);
		}
	}
}

NodFAV* cautaElementFAV(NodFAV* lista, char* id) {
	if (lista) {
		NodFAV* origine = lista;
		do {
			if (strcmp(lista->infoUtil->idAeronava, id) == 0)
				return lista;
			lista = lista->next;
		} while (lista != origine);
	}

	return nullptr;
}

void updateFAV(NodFAV* lista, char* idAeronava) {
	NodFAV* elem = cautaElementFAV(lista, idAeronava);
	if (elem) {
		printf("\nCe vrei sa modifici?\n");
		printf("1.Id-ul aeronavei\n");
		printf("2.Numar de locuri\n");
		printf("3.Greutatea maxima\n");
		printf("4.Tipul aeronavei\n");
		int ad;
		printf("\nIntrodu cifra corespunzatoare alegerii:");
		scanf("%d", &ad);

		switch (ad) {
			case 1:
				free(elem->infoUtil->idAeronava);
				printf("\nIntroduceti noul id al aeronavei:");
				char temp[100];
				scanf("%s", &temp);
				elem->infoUtil->idAeronava = (char*)malloc(strlen(temp) + 1);
				strcpy(elem->infoUtil->idAeronava, temp);
				break;
			case 2:
				printf("\nIntroduceti noul numar de locuri:");
				int nr;
				scanf("%d", &nr);
				elem->infoUtil->nrLocuri = nr;
				break;
			case 3:
				printf("\nIntroduceti noua greutate maxima:");
				int n;
				scanf("%d", &n);
				elem->infoUtil->greutateMaxima = n;
				break;
			case 4:
				if (elem->infoUtil->tipAero == 1)
					elem->infoUtil->tipAero = elem->infoUtil->PAS;
				else if (elem->infoUtil->tipAero == 0)
					elem->infoUtil->tipAero == elem->infoUtil->CAR;
		}
	} else
		printf("\nNu exista aceasta aeronava");
}


void findFAV(NodFAV* lista, char* id) {
	linieFAV* elem = cautaElementFAV(lista, id)->infoUtil;
	if (elem) printf("\n  %s %lf %i %d\n", elem->idAeronava, elem->greutateMaxima, elem->tipAero, elem->nrLocuri);
	else printf("\nNu exista elementul\n");
}

void afisareFAV(NodFAV* lista) {
	if (lista) {
		NodFAV* origine = lista;
		do {
			printf(" %s %lf %i %d\n", lista->infoUtil->idAeronava, lista->infoUtil->greutateMaxima, lista->infoUtil->tipAero, lista->infoUtil->nrLocuri);
			lista = lista->next;
		} while (lista != origine);
	} else printf("\nLista goala\n");
}

void deleteFAV(NodFAV* &lista, char* id) {
	NodFAV* elem = cautaElementFAV(lista, id);
	if (elem) {
		lista = elem->next;
		lista->prev = elem->prev;
		elem->prev->next = lista;
		free(elem->infoUtil->idAeronava);
		free(elem->infoUtil);
		free(elem);
	} else printf("\nNu exista elementul\n");
}

void addFAV(NodFAV* lista) {
	linieFAV* fav = (linieFAV*)malloc(sizeof(linieFAV));
	char id[100];
	printf("\nIntroduceti datele pentru o noua flota de aeronave.\n");
	printf("ID aeronava:");
	scanf("%s", id);
	fav->idAeronava = (char*)malloc(sizeof(char)*(strlen(id) + 1));
	strcpy(fav->idAeronava, id);
	printf("Nr locuri:"); scanf("%d", &fav->nrLocuri);
	printf("Greutate maxima:"); scanf("%lf", &fav->greutateMaxima);
	printf("Tip aeronava PAS/CAR:"); scanf("%d", &fav->tipAero);
	if (fav->tipAero != 0 && fav->tipAero != 1) {
		printf("\nNu se accepta decat valoarea 0 sau 1! Reintroduceti!\n");
		printf("Tip aeronava PAS/CAR:"); scanf("%d", &fav->tipAero);
	}
	NodFAV* nod = creareNodFAV(fav);
	inserareFAV(lista, nod);
}



void main() {
	NodFAV* lista = NULL;
	FILE* file;
	file = fopen("flaero.txt", "r");;
	initializareFAV(file, lista);
}
