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
FILE* program;

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
			printf("\t%s %lf %i %d\n", linie->idAeronava, linie->greutateMaxima, linie->tipAero, linie->nrLocuri);
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

////////////////////////////////////////////////////////////////////////////

struct linieRUZ {
	char* idZbor;
	char* idAvion;
	int oraPlecare;
	int oraSosire;
	char* orasPlecare;
	char* orasSosire;
	float costuriTotaleRuta;
};
struct NodRUZ {
	linieRUZ* info;
	NodRUZ* next;
};
struct HashRUZ {
	(NodRUZ*)* listaRUZ;
	int dim;
};
HashRUZ hashTable;

linieRUZ* creareRUZ(char* idZbor, char* idAvion, int oraPlecare, int oraSosire, char* orasPlecare,
					char* orasSosire, float costuriTotaleRuta) {
	linieRUZ* ruz = (linieRUZ*)malloc(sizeof(linieRUZ));
	ruz->idZbor = copiaza(idZbor);
	ruz->idAvion = copiaza(idAvion);
	ruz->oraPlecare = oraPlecare;
	ruz->oraSosire = oraSosire;
	ruz->orasPlecare = copiaza(orasPlecare);
	ruz->orasSosire = copiaza(orasSosire);
	ruz->costuriTotaleRuta = costuriTotaleRuta;

	return ruz;
}
NodRUZ* creareNod(linieRUZ* nod) {
	NodRUZ* nou = (NodRUZ*)malloc(sizeof(NodRUZ));
	nou->info = nod;
	nou->next = nullptr;
	return nou;
}
void inserareNodRUZ(NodRUZ* &lista, NodRUZ* nod) {
	if (lista == nullptr) {
		lista = nod;
	} else {
		while (lista->next)
			lista = lista->next;
		lista->next = nod;
	}
}
void alocaMemorieRUZ() {
	hashTable.dim = 20;
	hashTable.listaRUZ = (NodRUZ**)malloc(sizeof(NodRUZ*)*hashTable.dim);
	for (int i = 0; i < hashTable.dim; i++) {
		hashTable.listaRUZ[i] = nullptr;
	}
}
int functieHashRUZ(char* idZbor) {
	int sum = 0;
	while (*idZbor) {
		sum = sum*hashTable.dim + *(idZbor++);
	}
	return sum % hashTable.dim;
}
void inserareRUZ(NodRUZ* nod) {
	int poz = functieHashRUZ(nod->info->idZbor);
	NodRUZ* lista = hashTable.listaRUZ[poz];
	inserareNodRUZ(lista, nod);
	hashTable.listaRUZ[poz] = lista;
}
void intilializareRUZ() {
	alocaMemorieRUZ();
	program = fopen("program.txt", "r");
	if (!program) {
		echo("\nFisierul nu a fost gasit!\n");
	} else {
		char idZbor[20], orasPlecare[20], orasSosire[20], idAvion[20];
		int oraPlecare, oraSosire;
		float  costuriTotaleRuta;
		fscanf(program, "%s", idZbor);
		while (!feof(program)) {
			fscanf(program, "%s", idAvion);
			fscanf(program, "%d", &oraPlecare);
			fscanf(program, "%d", &oraSosire);
			fscanf(program, "%s", orasPlecare);
			fscanf(program, "%s", orasSosire);
			fscanf(program, "%f", &costuriTotaleRuta);
			linieRUZ* ruz = creareRUZ(idZbor, idAvion, oraPlecare, oraSosire, orasPlecare, orasSosire, costuriTotaleRuta);
			NodRUZ* nod = creareNod(ruz);
			inserareRUZ(nod);

			fscanf(program, "%s", idZbor);
		}
	}
}
void afisareLinieRUZ(linieRUZ* ruz) {
	if (ruz) {
		char msg[54];
		sprintf(msg, "\t%s %s %d %d %s %s %f\n", ruz->idZbor, ruz->idAvion, ruz->oraPlecare, ruz->oraSosire,
				ruz->orasPlecare, ruz->orasSosire, ruz->costuriTotaleRuta);
		echo(msg);
	} /*else echo("\Element vid!\n");*/
}
void afisareRUZ() {
	if (hashTable.listaRUZ != nullptr) {
		for (int i = 0; i < hashTable.dim; i++) {
			NodRUZ* ruz = hashTable.listaRUZ[i];
			while (ruz) {
				afisareLinieRUZ(ruz->info);
				ruz = ruz->next;
			}
		}
	} else echo("\nNu exista nicio ruta de zbor!\n");
}
NodRUZ* cautaElementRUZ(char* idZbor) {
	int poz = functieHashRUZ(idZbor);
	NodRUZ* list = hashTable.listaRUZ[poz];
	while (list != nullptr && strcmp(list->info->idZbor, idZbor) != 0)
		list = list->next;
	if (strcmp(list->info->idZbor, idZbor) == 0)
		return list;
	else return nullptr;
}

void findRUZ(char* idZbor) {
	NodRUZ* elem = cautaElementRUZ(idZbor);
	if (elem != nullptr) {
		linieRUZ* linie = elem->info;
		char msg[54];
		sprintf(msg, "\t%s %s %d %d %s %s %f\n", linie->idZbor, linie->idAvion, linie->oraPlecare, linie->oraSosire,
				linie->orasPlecare, linie->orasSosire, linie->costuriTotaleRuta);
		echo(msg);
	} else echo("\nNu exista aceasta ruta de zbor!\n");
}

void addRUZ(char* DATE) {
	char* dat = copiaza(DATE);
	char* aux;
	char oraPlecare, oraSosire;
	float  costuriTotaleRuta;

	char* idZbor = strtok(dat, " \t");
	char* idAvion = strtok(NULL, " \t");
	aux = strtok(NULL, " \t"); oraPlecare = atoi(aux);
	aux = strtok(NULL, " \t"); oraSosire = atoi(aux);
	char* orasPlecare = strtok(NULL, " \t");
	char* orasSosire = strtok(NULL, " \t");
	aux = strtok(NULL, " \t"); costuriTotaleRuta = atof(aux);
	linieRUZ* ruz = creareRUZ(idZbor, idAvion, oraPlecare, oraSosire, orasPlecare, orasSosire, costuriTotaleRuta);
	NodRUZ* nod = creareNod(ruz);
	inserareRUZ(nod);
	echo("Ruta de zbor adaugata!\n");
}

void updateRUZ(char* DATE) {
	char* dat = copiaza(DATE);
	char* idZbor = strtok(dat, " \t");
	int poz = functieHashRUZ(idZbor);
	NodRUZ* lista = hashTable.listaRUZ[poz];
	NodRUZ* elem = cautaElementRUZ(idZbor);
	if (elem != nullptr) {
		linieRUZ* linie = elem->info;
		char* aux = strtok(NULL, " \t"); free(linie->idAvion); linie->idAvion = copiaza(aux);
		aux = strtok(NULL, " \t"); linie->oraPlecare = atoi(aux);
		aux = strtok(NULL, " \t"); linie->oraSosire = atoi(aux);
		aux = strtok(NULL, " \t"); free(linie->orasPlecare); linie->orasPlecare = copiaza(aux);
		aux = strtok(NULL, " \t"); free(linie->orasSosire);	linie->orasSosire = copiaza(aux);
		aux = strtok(NULL, " \t"); linie->costuriTotaleRuta = atof(aux);

		echo("Ruta de zbor actualizata!\n");
	} else echo("\nNu exista ruta de zbor cu acest id!\n");
}

void deleteRUZ(char* idZbor) {
	NodRUZ* elem = cautaElementRUZ(idZbor);
	if (elem != nullptr) {
		int poz = functieHashRUZ(idZbor);
		NodRUZ* lista = hashTable.listaRUZ[poz];
		if (lista == elem) {
			hashTable.listaRUZ[poz] = elem->next;
		} else {
			NodRUZ* tmp = lista;
			while ((tmp->next != nullptr) && (tmp->next != elem)) {
				tmp = tmp->next;
			}
			tmp->next = tmp->next->next;
			hashTable.listaRUZ[poz] = lista;
			//free(tmp->info->idAvion); free(tmp->info->idZbor); free(tmp->info->orasPlecare); free(tmp->info->orasSosire); free(tmp->info); free(tmp);
		}
		echo("Ruta de zbor stearsa!\n");
	} else echo("\nNu exista ruta de zbor cu acest id!\n");
}

void main() {
	log = fopen("log.txt", "wt");
	initializareFAV();
	intilializareRUZ();

	findFAV("zxcv");
	addFAV("aeronava4 1 34 456");
	updateFAV("aeronava4 1 36 345");
	deleteFAV("zxcv");
	afisareFAV();

	findRUZ("hash3");
	addRUZ("hash10 13asdf 12 15 bucuresti valcea 256.6");
	updateRUZ("hash1 11asdf 13 17 bucur valcea 256.6");
	deleteRUZ("hash3");
	afisareRUZ();
}
