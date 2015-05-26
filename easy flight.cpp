#include "stdio.h"
#include "string.h"
#include "malloc.h"
#include "stdlib.h"
#include <time.h>
#include <ctype.h>

FILE* flaero;
FILE* marfuri;
FILE* program;
FILE* pasageri;
FILE* batch;
FILE* log;
char* copiaza(char* string) {
	char* nou = (char*)malloc(sizeof(char)*(strlen(string) + 1));
	strcpy(nou, string);
	return nou;
}
void ftimestamp() {
	time_t ltime; /* calendar time */
	ltime = time(NULL); /* get current cal time */
	char* time = nullptr;
	time = copiaza(asctime(localtime(&ltime)));
	time[strlen(time) - 1] = '\0';
	fprintf_s(log, "%s: ", time);
}
void echo(char* msg) {
	char* mes = copiaza(msg);
	printf(mes);
	ftimestamp();
	fprintf(log, mes);
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
void inserareFAV(NodFAV* nod) {
	NodFAV* elem = cautaElementFAV(nod->infoUtil->idAeronava);
	if (elem != nullptr){
		if (listaDublaCirculara == nullptr) {
			listaDublaCirculara = nod;
			listaDublaCirculara->next = listaDublaCirculara->prev = listaDublaCirculara;
		} else {
			nod->next = listaDublaCirculara;
			listaDublaCirculara->prev->next = nod;
			nod->prev = listaDublaCirculara->prev;
			listaDublaCirculara->prev = nod;
		}
	} else echo("\tElement duplicat!!\n");
}
void initializareFAV() {
	echo("Initializez FAV...\n");
	flaero = fopen("flaero.txt", "rt");
	if (!flaero) {
		echo("\tNu se poate deschide fisierul!\n");
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
		fclose(flaero);
	}
}

void afisareLinieFAV(linieFAV* fav) {
	char msg[54];
	sprintf(msg, "\t%s %lf %i %d\n", fav->idAeronava, fav->greutateMaxima, fav->tipAero, fav->nrLocuri);
	echo(msg);
}
void afisareFAV() {
	if (listaDublaCirculara != nullptr) {
		NodFAV* origine = listaDublaCirculara;
		do {
			afisareLinieFAV(listaDublaCirculara->infoUtil);
			listaDublaCirculara = listaDublaCirculara->next;
		} while (listaDublaCirculara != origine);
	} else echo("\tLista goala\n");
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
		afisareLinieFAV(linie);
	} else echo("\tNu exista elementul\n");
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
	echo("\tAvion adaugat!\n");
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
		echo("\tDate avion actualizate!\n");
	} else echo("\tElementul nu a fost gasit!\n");
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
		echo("\tAvion sters!\n");
	} else echo("\tNu exista elementul\n");
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
HashRUZ tabelaDispersie;

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
	tabelaDispersie.dim = 20;
	tabelaDispersie.listaRUZ = (NodRUZ**)malloc(sizeof(NodRUZ*)*tabelaDispersie.dim);
	for (int i = 0; i < tabelaDispersie.dim; i++) {
		tabelaDispersie.listaRUZ[i] = nullptr;
	}
}
int functieHashRUZ(char* idZbor) {
	int sum = 0;
	while (*idZbor) {
		sum = sum*tabelaDispersie.dim + *(idZbor++);
	}
	return sum % tabelaDispersie.dim;
}
void inserareRUZ(NodRUZ* nod) {
	int poz = functieHashRUZ(nod->info->idZbor);
	NodRUZ* lista = tabelaDispersie.listaRUZ[poz];
	inserareNodRUZ(lista, nod);
	tabelaDispersie.listaRUZ[poz] = lista;
}
void initializareRUZ() {
	echo("Initializez RUZ...\n");
	alocaMemorieRUZ();
	program = fopen("program.txt", "rt");
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
		fclose(program);
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
	if (tabelaDispersie.listaRUZ != nullptr) {
		for (int i = 0; i < tabelaDispersie.dim; i++) {
			NodRUZ* ruz = tabelaDispersie.listaRUZ[i];
			while (ruz) {
				afisareLinieRUZ(ruz->info);
				ruz = ruz->next;
			}
		}
	} else echo("\tNu exista nicio ruta de zbor!\n");
}
NodRUZ* cautaElementRUZ(char* idZbor) {
	int poz = functieHashRUZ(idZbor);
	NodRUZ* list = tabelaDispersie.listaRUZ[poz];
	while (list != nullptr && strcmp(list->info->idZbor, idZbor) != 0)
		list = list->next;
	if ((list != nullptr) && (strcmp(list->info->idZbor, idZbor) == 0))
		return list;
	else return nullptr;
}

void findRUZ(char* idZbor) {
	NodRUZ* elem = cautaElementRUZ(idZbor);
	if (elem != nullptr) {
		linieRUZ* linie = elem->info;
		afisareLinieRUZ(linie);
	} else echo("\tNu exista aceasta ruta de zbor!\n");
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
	NodRUZ* lista = tabelaDispersie.listaRUZ[poz];
	NodRUZ* elem = cautaElementRUZ(idZbor);
	if (elem != nullptr) {
		linieRUZ* linie = elem->info;
		char* aux = strtok(NULL, " \t"); free(linie->idAvion); linie->idAvion = copiaza(aux);
		aux = strtok(NULL, " \t"); linie->oraPlecare = atoi(aux);
		aux = strtok(NULL, " \t"); linie->oraSosire = atoi(aux);
		aux = strtok(NULL, " \t"); free(linie->orasPlecare); linie->orasPlecare = copiaza(aux);
		aux = strtok(NULL, " \t"); free(linie->orasSosire);	linie->orasSosire = copiaza(aux);
		aux = strtok(NULL, " \t"); linie->costuriTotaleRuta = atof(aux);

		echo("\tRuta de zbor actualizata!\n");
	} else echo("\tNu exista ruta de zbor cu acest id!\n");
}

void deleteRUZ(char* idZbor) {
	NodRUZ* elem = cautaElementRUZ(idZbor);
	if (elem != nullptr) {
		int poz = functieHashRUZ(idZbor);
		NodRUZ* lista = tabelaDispersie.listaRUZ[poz];
		if (lista == elem) {
			tabelaDispersie.listaRUZ[poz] = elem->next;
		} else {
			NodRUZ* tmp = lista;
			while ((tmp->next != nullptr) && (tmp->next != elem)) {
				tmp = tmp->next;
			}
			tmp->next = tmp->next->next;
			tabelaDispersie.listaRUZ[poz] = lista;
			//free(tmp->info->idAvion); free(tmp->info->idZbor); free(tmp->info->orasPlecare); free(tmp->info->orasSosire); free(tmp->info); free(tmp);
		}
		echo("Ruta de zbor stearsa!\n");
	} else echo("\tNu exista ruta de zbor cu acest id!\n");
}

////////////////////////////////////////////////////////////////////////////

struct linieCAR {
	int idRezervare;
	char* idZbor;
	float greutateMarfa;
	float nivelRisc;
	float costTotal;
};
struct NodCAR {
	linieCAR* car;
	int ge;
	NodCAR* st;
	NodCAR* dr;
};
NodCAR* arboreAVL = NULL;

linieCAR* creareCAR(int idR, char* idZ, float grM, float nvR) {
	linieCAR* car = (linieCAR*)malloc(sizeof(linieCAR));
	car->idRezervare = idR;
	car->idZbor = copiaza(idZ);
	car->greutateMarfa = grM;
	car->nivelRisc = nvR;
	car->costTotal = car->greutateMarfa*car->nivelRisc * 7;

	return car;
}
int max(int a, int b) {
	return (a > b ? a : b);
}
int H(NodCAR* rad) {
	if (rad)	return 1 + max(H(rad->st), H(rad->dr));
	else		return 0;
}
void gradEchilibru(NodCAR* rad) {
	if (rad)	rad->ge = H(rad->dr) - H(rad->st);
}
NodCAR* rotatieSimplaStanga(NodCAR* pivot, NodCAR* fiuDR) {
	pivot->dr = fiuDR->st;
	gradEchilibru(pivot);
	fiuDR->st = pivot;
	gradEchilibru(fiuDR);
	return fiuDR;
}
NodCAR* rotatieSimplaDreapta(NodCAR* pivot, NodCAR* fiuSt) {
	pivot->st = fiuSt->dr;
	gradEchilibru(pivot);
	fiuSt->dr = pivot;
	gradEchilibru(fiuSt);
	return fiuSt;
}
NodCAR* rotatieDublaDreapta(NodCAR* pivot, NodCAR* fiuSt) {
	pivot->st = rotatieSimplaStanga(fiuSt, fiuSt->dr);
	gradEchilibru(pivot);
	fiuSt = pivot->st;
	fiuSt = rotatieSimplaDreapta(pivot, fiuSt);
	gradEchilibru(fiuSt);
	return fiuSt;
}
NodCAR* rotatieDublaStanga(NodCAR* pivot, NodCAR* fiuDr) {
	pivot->dr = rotatieSimplaDreapta(fiuDr, fiuDr->st);
	gradEchilibru(pivot);
	fiuDr = pivot->dr;
	fiuDr = rotatieSimplaStanga(pivot, fiuDr);
	gradEchilibru(fiuDr);
	return fiuDr;
}
NodCAR* echilibrare(NodCAR *p){
	NodCAR *w;
	gradEchilibru(p);//se calculeaza factorul de echilibru a nodului curent p
	if (p->ge == -2){// daca p nod este critic
		w = p->st; // atunci w este copilul stanga al lui p
		if (w->ge == 1)// si daca acesta are factorul de echilibru 1
			p = rotatieDublaDreapta(p, p->st);// atunci se face dubla rotatie dreapta
		else//altfel se face o simpla rotatie dreapta
			p = rotatieSimplaDreapta(p, p->st);
	}
	else
		if (p->ge == 2){//daca p nod este critic
			w = p->dr;//w este copilul dreapta al nodului curent p
			if (w->ge == -1)// si acesta are factorul de ech -1
				p = rotatieDublaStanga(p, p->dr);//se face o dubla rotatie stanga
			else//altfel se face o simpla rotatie stanga
				p = rotatieSimplaStanga(p, p->dr);
		}
	return p;
}

NodCAR* stergere(NodCAR *p, int x){
	NodCAR *q, *r, *w;
	if (p != NULL)//daca nodul curent este diferit de NULL
		if (x<p->car->idRezervare) //cheia care se doreste stearsa este mai mica decat informatia din nod
			p->st = stergere(p->st, x); // se cauta cheia de sters in subarborele stang al nodului curent
		else
			if (x>p->car->idRezervare) // daca cheia este mai mare
				p->dr = stergere(p->dr, x);// se cauta in subarborele drept
			else{
				//daca cheia este egala cu informatia din nodul curent
				q = p;//un nod q devine p
				if (q->dr == NULL) // daca copilul drept al lui q eate NULL
					p = q->st;// atunci p devine q->stanga
				else if (q->st == NULL) //altfel daca copilul stang al lui q este NULL
						p = q->dr;// p devine q->dreapta
					 else{
						w = q->st;//altfel w este copilul stanga al lui q
						r = q;// r devine q
						if (w->dr != NULL) { // daca copilul drept al lui w nun este NULL
							while (w->dr != NULL){
								r = w;
								w = w->dr;
							}
							p->car->idRezervare = w->car->idRezervare;
							q = w;
							r->dr = w->st;
							r = p->st;
							w = w->st;
							if (r != NULL)
								while ((r != w) && (r != NULL)){
									r = echilibrare(r);
									r = r->dr;
								}
						}
						else{
							p->car->idRezervare = w->car->idRezervare;
							p->st = w->st;
							q = w;
						}
					}
					delete(q);// se sterge q
			}
			if (p != NULL)
				p = echilibrare(p);// se echilibreaza p daca nu este NULL
			return p;
}

void inserareCAR(NodCAR* &rad, linieCAR* carg) {
	if (rad != nullptr) {
		if (rad->car->idRezervare < carg->idRezervare)
			inserareCAR(rad->dr, carg);
		else
			if (rad->car->idRezervare > carg->idRezervare)
				inserareCAR(rad->st, carg);
			else{
				char msg[30];
				sprintf(msg, "\tElementul %d este deja prezent in arboreAVL.\n", carg->idRezervare);
				echo(msg);
			}
	} else {
		rad = (NodCAR*)malloc(sizeof(NodCAR));
		rad->car = carg;
		rad->dr = NULL;
		rad->st = NULL;
	}

	gradEchilibru(rad);
	if (rad->ge == 2)
		if (rad->ge == -1)
			rad = rotatieDublaStanga(rad, rad->dr);
		else
			rad = rotatieSimplaStanga(rad, rad->dr);
	else
		if (rad->ge == -2) {
			if (rad->ge == 1)
				rad = rotatieDublaDreapta(rad, rad->st);
			else
				rad = rotatieSimplaDreapta(rad, rad->st);
		}
}
void initializareCAR() {
	echo("Initializez CAR...\n");
	marfuri = fopen("marfuri.txt", "rt");
	if (!marfuri) {
		printf("Nu se poate deschide fisierul!\n");
	} else {
		int idRezervare;
		float nivelRisc;
		char idZbor[100];
		float greutateMarfa;
		fscanf(marfuri, "%d", &idRezervare);
		while (!feof(marfuri)) {
			fscanf(marfuri, "%s", &idZbor);
			fscanf(marfuri, "%f", &greutateMarfa);
			fscanf(marfuri, "%f", &nivelRisc);
			linieCAR* car = creareCAR(idRezervare, idZbor, greutateMarfa, nivelRisc);
			inserareCAR(arboreAVL, car);

			fscanf(marfuri, "%d", &idRezervare);
		}
		fclose(marfuri);
	}
}
NodCAR* cautaElementCAR(NodCAR* rad, int id) {
	if (rad == nullptr)
		return nullptr;
	else {
		if (rad->car->idRezervare == id)
			return rad;
		else {
			if (rad->car->idRezervare > id)
				cautaElementCAR(rad->st, id);
			else cautaElementCAR(rad->dr, id);
		}
	}
}
void afisareLinieCAR(linieCAR* car) {
	char msg[54];
	sprintf(msg, "%d %s %f %f $%f\n", car->idRezervare, car->idZbor, car->greutateMarfa, car->nivelRisc, car->costTotal);
	echo(msg);
}
void afisareCAR(NodCAR* rad) { //SRD
	if (rad) {
		afisareCAR(rad->st);
		afisareLinieCAR(rad->car);
		afisareCAR(rad->dr);
	}
}

void findCAR(char* id) {
	int idRezervare = atoi(id);
	NodCAR* elem = cautaElementCAR(arboreAVL, idRezervare);
	if (elem != nullptr) {
		linieCAR* linie = elem->car;
		afisareLinieCAR(linie);
	}
	else echo("\tNu exista aceasta ruta de zbor!\n");
}

void addCAR(char* DATE) {
	char* dat = copiaza(DATE);
	char* aux;
	int idRezervare;
	float greutateMarfa, nivelRisc, costTotal;
	aux = strtok(dat, " \t");
	idRezervare = atoi(aux);
	char* idZbor = strtok(NULL, " \t");
	greutateMarfa = atof(aux);
	aux = strtok(NULL, " \t");
	nivelRisc = atof(aux);
	linieCAR* car = creareCAR(idRezervare, idZbor, greutateMarfa, nivelRisc);
	inserareCAR(arboreAVL, car);
	echo("\tGestiune cargo adaugata!\n");
}

void updateCAR(char* DATE) {
	char* dat = copiaza(DATE);
	char* aux;
	int id;
	float greutateMarfa, nivelRisc, costTotal;
	aux = strtok(dat, " \t");
	id = atoi(aux);
	NodCAR* elem = cautaElementCAR(arboreAVL, id);
	if (elem != nullptr) {
		linieCAR* linie = elem->car;
		linie->idRezervare = id;
		char* idZb = strtok(NULL, " \t");
		free(linie->idZbor);
		linie->idZbor = copiaza(idZb);
		char* aux = strtok(NULL, " \t");
		linie->greutateMarfa = atof(aux);
		aux = strtok(NULL, " \t");
		linie->nivelRisc = atof(aux);
		linie->costTotal = linie->costTotal*linie->nivelRisc * 7;
		echo("\tDate gestiune cargo actualizate!\n");
	} else echo("\tElementul nu a fost gasit!\n");
}

void deleteCAR(char* id) {
	int idRezervare = atoi(id);
	NodCAR* ele = cautaElementCAR(arboreAVL, idRezervare), *elem=nullptr;
	if (elem != nullptr) {
		elem = stergere(elem, idRezervare);
		echo("\tRezervare stearsa!\n");
	}
	else echo("\tRezervare stearsa!\n");
}

void checkCAR(char* idZbor) {

}

void costsCAR(char* idZbor) {

}

////////////////////////////////////////////////////////////////////////////

struct linieREP {
	unsigned int locZbor;
	float costBilet;
	int idRezervare;
	char* idZbor;
	char* numeClient;
};

struct ListaSREP {
	linieREP* info;
	ListaSREP* nextS;
};
struct ListaPREP {
	linieREP* info;
	ListaPREP* nextP;
	ListaSREP* listaNoduri;
};
ListaPREP* graf = NULL;
int contor = 5;
ListaPREP* creareNodPREP(linieREP* rep) {
	ListaPREP* nou = (ListaPREP*)malloc(sizeof(ListaPREP));
	nou->info = rep;
	nou->nextP = NULL;
	nou->listaNoduri = NULL;;
	return nou;
}
ListaSREP* creareNodSREP(linieREP* rep) {
	ListaSREP* nou = (ListaSREP*)malloc(sizeof(ListaSREP));
	nou->info = rep;
	nou->nextS = NULL;
	return nou;
}
linieREP* creareREP(int idRez, char* idZbor, char* nume, int loc, float cost) {
	linieREP* rep = (linieREP*)malloc(sizeof(linieREP));
	rep->idRezervare = idRez;
	rep->idZbor = copiaza(idZbor);
	rep->numeClient = copiaza(nume);
	rep->locZbor = loc;
	rep->costBilet = cost;
	return rep;
}
ListaPREP* inserareLPREP(ListaPREP*& graf, linieREP* rep) {
	if (graf == NULL) {
		graf = creareNodPREP(rep);
		return graf;
	}
	else {
		ListaPREP* temp = graf;
		while (temp->nextP != NULL)
			temp = temp->nextP;
		temp->nextP = creareNodPREP(rep);
		return temp->nextP;
	}
}
ListaSREP* inserareLSREP(ListaSREP* noduriREP, linieREP* rep) {
	if (noduriREP == NULL)
		noduriREP = creareNodSREP(rep);
	else {
		ListaSREP* temp = noduriREP;
		while (temp->nextS)
			temp = temp->nextS;
		temp->nextS = creareNodSREP(rep);
	}
	return noduriREP;
}
ListaSREP* cauta(ListaSREP* srep) {
	return srep;
}
ListaPREP* cautaElementREP(int id) {
	if (graf != NULL) {
		ListaPREP* origine = graf;
		do {
			if (graf->info->idRezervare == id)
				return graf;
			else if (graf->listaNoduri->info->idRezervare == id)
				cauta(graf->listaNoduri);
			graf = graf->nextP;
		} while (graf != origine);
	}
	return nullptr;
}
void findREP(char* id){

}

void addREP(char* date) {
	char* dat = copiaza(date);
	char* aux;
	int idR, loc;
	float cost;
	aux = strtok(dat, " \t");
	idR = atoi(aux);
	char* idZ = strtok(NULL, " \t");
	char* nume = strtok(NULL, " \t");
	aux = strtok(NULL, " \t");
	loc = atoi(aux);
	aux = strtok(NULL, " \t");
	cost = atof(aux);

	linieREP* rep = creareREP(idR, idZ, nume, loc, cost);
	inserareLPREP(graf, rep);
	echo("Adaugare efectuata cu succes!\n");
}

void updateREP(char* date) {
	char* dat = copiaza(date);
	char* aux;
	int idR;
	aux = strtok(dat, " \t");
	idR = atoi(aux);
	ListaPREP* elem = cautaElementREP(idR);
	if (elem != nullptr) {
		linieREP* linie = elem->info;
		char* idZ = strtok(NULL, " \t");
		free(linie->idZbor);
		linie->idZbor = copiaza(idZ);
		char* nume = strtok(NULL, " \t");
		free(linie->numeClient);
		linie->numeClient = copiaza(nume);
		aux = strtok(NULL, " \t");
		linie->locZbor = atoi(aux);
		aux = strtok(NULL, " \t");
		linie->costBilet = atof(aux);
		echo("Date modul rezervare actualizate!\n");
	}
	else
		echo("Elementul nu a fost gasit!");
}

void deleteREP(char* id){

}

void checkREP(char* id){

}

void costsREP(char* id){

}

void initializareREP(FILE* f, ListaPREP* rep) {
	
	if (!f) {
		echo("\tNu se poate deschide fisierul!");
	}
	else {
		int idRez;
		char idZbor[100];
		char nume[100];
		int loc;
		float cost; int aux;
		fscanf(f, "%d", &idRez);
		while (!feof(f))
		{
			fscanf(f, "%s", &idZbor);
			fscanf(f, "%s", &nume);
			fscanf(f, "%d", &loc);
			fscanf(f, "%f", &cost);
			fscanf(f, "%d", &aux);
			linieREP* rep = creareREP(idRez, idZbor, nume, loc, cost);
			//for (int i = 0; i < contor; i++) {
			ListaPREP* temp = inserareLPREP(graf, rep);
			while (fscanf(f, "%d", &idRez) && fscanf(f, "%s", &idZbor) &&
				fscanf(f, "%s", &nume) && fscanf(f, "%d", &loc) &&
				fscanf(f, "%f", &cost) && aux != -1) {
				linieREP* rep1 = creareREP(idRez, idZbor, nume, loc, cost);
				ListaSREP* noduri = temp->listaNoduri;
				noduri = inserareLSREP(noduri, rep1);
				temp->listaNoduri = noduri;
				fscanf(f, "%d", &aux);
			}
			//}
			fscanf(f, "%d", &idRez);
		}
	}
}

void afisareREP(ListaPREP* graf) {
	while (graf) {
		printf("Lista principala: %d %s %s %d %f \n", graf->info->idRezervare, graf->info->idZbor,
			graf->info->numeClient, graf->info->locZbor, graf->info->costBilet);
		ListaSREP* noduri = graf->listaNoduri;
		while (noduri) {
			printf("Lista secundara: %d %s %s %d %f \n", noduri->info->idRezervare, noduri->info->idZbor,
				noduri->info->numeClient, noduri->info->locZbor, noduri->info->costBilet);
			noduri = noduri->nextS;
		}
		graf = graf->nextP;
	}
}

struct picker {
	char* comenzi[22];
	void(*functii[22])(char*);
	int getpoz(char* com, char** comenzi) {
		int poz = 0;
		while (strcmp(com, comenzi[poz]) != 0 && poz < 21)
			poz++;
		return poz;
	}
	void(*getfunk(char* com))(char*) {
		return functii[getpoz(com, comenzi)];
	}
}picker;
void notFound(char* somestring) {
	echo("\n404: Nu exista functia!\n");
}

void actualizareFAV() {
	flaero = fopen("flaero.txt", "wt");
	if (listaDublaCirculara != nullptr) {
		NodFAV* origine = listaDublaCirculara;
		do {
			linieFAV* fav = listaDublaCirculara->infoUtil;
			fprintf(flaero, "%s %lf %i %d\n", fav->idAeronava, fav->greutateMaxima, fav->tipAero, fav->nrLocuri);
			listaDublaCirculara = listaDublaCirculara->next;
		} while (listaDublaCirculara != origine);
	}
	fclose(flaero);
}
void actualizareCAR(NodCAR* rad) {
	marfuri = fopen("marfuri.txt", "wt");
	if (rad) {
		actualizareCAR(rad->st);
		linieCAR* car = rad->car;
		fprintf(marfuri, "%d %s %f %f $%f\n", car->idRezervare, car->idZbor, car->greutateMarfa, car->nivelRisc, car->costTotal);
		actualizareCAR(rad->dr);
	}
	fclose(marfuri);
}
void actualizareRUZ() {
	program = fopen("program.txt", "wt");
	if (tabelaDispersie.listaRUZ != nullptr) {
		for (int i = 0; i < tabelaDispersie.dim; i++) {
			NodRUZ* ruz = tabelaDispersie.listaRUZ[i];
			while (ruz) {
				linieRUZ* linie = ruz->info;
				fprintf(program, "\t%s %s %d %d %s %s %f\n", linie->idZbor, linie->idAvion, linie->oraPlecare, linie->oraSosire,
					linie->orasPlecare, linie->orasSosire, linie->costuriTotaleRuta);
				ruz = ruz->next;
			}
		}
	}
	fclose(program);
}
void actualizareREP() {
	//pasageri = fopen("pasageri.txt", "wt");

	//fclose(pasageri);
}
void exit(char* somestring) {
	actualizareFAV();
	actualizareCAR(arboreAVL);
	actualizareRUZ();
	actualizareREP();
	fclose(log);
	exit(0);
}
void initPicker() { //aici se initializeaza pickerul cu comenzi si functiile aferente
	for (int i = 0; i < 25; i++) {
		picker.comenzi[i] = nullptr; //(char*)malloc(7);
		picker.functii[i] = nullptr;
	}
	picker.functii[21] = notFound;

	picker.comenzi[0] = copiaza("FINDFAV");
	picker.functii[0] = findFAV;
	picker.comenzi[1] = copiaza("ADDFAV");
	picker.functii[1] = addFAV;
	picker.comenzi[2] = copiaza("UPDATEFAV");
	picker.functii[2] = updateFAV;
	picker.comenzi[3] = copiaza("DELETEFAV");
	picker.functii[3] = deleteFAV;
	picker.comenzi[4] = copiaza("FINDRUZ");
	picker.functii[4] = findRUZ;
	picker.comenzi[5] = copiaza("ADDRUZ");
	picker.functii[5] = addRUZ;
	picker.comenzi[6] = copiaza("UPDATERUZ");
	picker.functii[6] = updateRUZ;
	picker.comenzi[7] = copiaza("DELETERUZ");
	picker.functii[7] = deleteRUZ;
	picker.comenzi[8] = copiaza("FINDCAR");
	picker.functii[8] = findCAR;
	picker.comenzi[9] = copiaza("ADDCAR");
	picker.functii[9] = addCAR;
	picker.comenzi[10] = copiaza("UPDATECAR");
	picker.functii[10] = updateCAR;
	picker.comenzi[11] = copiaza("DELETECAR");
	picker.functii[11] = deleteCAR;
	picker.comenzi[12] = copiaza("CHECKCAR");
	picker.functii[12] = checkCAR;
	picker.comenzi[13] = copiaza("COSTSCAR");
	picker.functii[13] = costsCAR;
	picker.comenzi[14] = copiaza("FINDREP");
	picker.functii[14] = findREP;
	picker.comenzi[15] = copiaza("ADDREP");
	picker.functii[15] = addREP;
	picker.comenzi[16] = copiaza("UPDATEREP");
	picker.functii[16] = updateREP;
	picker.comenzi[17] = copiaza("DELETEREP");
	picker.functii[17] = deleteREP;
	picker.comenzi[18] = copiaza("CHECKREP");
	picker.functii[18] = checkREP;
	picker.comenzi[19] = copiaza("COSTSREP");
	picker.functii[19] = costsREP;
	picker.comenzi[20] = copiaza("EXIT");
	picker.functii[20] = exit;
}
char* upcase(char* string) {
	if (string != nullptr){
		char* upcase = (char*)malloc(strlen(string) + 1);
		char* ptr = upcase;
		while (*string)
			*(ptr++) = toupper(*(string++));
		*ptr = '\0';
		return upcase;
	}
}
void interp(char* comline) {
	if (*comline != '\0'){
		//primeste un linia cu intreaga comanda ca si string
		char msg[77];
		sprintf(msg, "\nExecut: %s\n", comline);
		echo(msg);
		char* aux = copiaza(comline);
		//concateneaza numele comenzii si cel al modulului pentru a putea cauta functia 
		char* com = upcase(strtok(aux, " \t")); aux = upcase(strtok(nullptr, " \t"));
		if (com != '\0'){
			strcat(com, aux);
			char* data = strtok(nullptr, ""); //ce mai ramane din string reprezinta date de intrare pentru functie
			picker.getfunk(com)(data);
		}
	}
}
void fetch() {
	char comanda[99];
	batch = fopen("batch.txt", "rt");
	if (!batch)
		echo("Nu exista fisier batch\n");
	else{
		while (fgets(comanda, sizeof(comanda), batch)) {
			comanda[strlen(comanda) - 1] = '\0';
			interp(comanda);
		}
		fclose(batch);
	}
}
void iniTOT() {
	log = fopen("log.txt", "wt");
	initPicker();
	initializareFAV();
	initializareRUZ();
	initializareCAR();
	echo("Initializez REP...\n");
	marfuri = fopen("pasgeri.txt", "rt");
	initializareREP(pasageri,graf);
	fetch();
}
void testare_module() {
	/*
	find FAV aeronava4
	add FAV aeronava6 1 34 456
	update FAV aeronava4 1 36 345
	delete FAV aeronava6
	find ruz zbor1
	add RUZ zbor5 aeronava3 11 12 sibiu valcea 1024.3
	update RUZ zbor2 aeronava2 16 18 sibiu hunedoara 99.2
	delete RUZ zbor5
	find CAR 5 ana 3 5
	add CAR 5 ana 3 5
	update CAR 5 maria 30 4
	delete CAR 5 ana 3 5
	find CAR 13
	add CAR 16 zbor4 77.2 1.5
	update CAR 13 zbor3 77.2 1.5
	delete CAR 5

	*/
	afisareFAV();
	afisareRUZ();
	afisareCAR(arboreAVL);
	afisareREP(graf);
}
void asteapta_comanda(){
	char comanda[34];
	printf("Indroduceti o comanda (sau Ctrl+Z): ");
	while (fgets(comanda, sizeof(comanda), stdin)) {
		comanda[strlen(comanda) - 1] = '\0';
		interp(comanda);
		printf("Indroduceti comanda (sau Ctrl+Z): ");
	}
	exit("~~~~~~ !!GATAAA!! ~~~~~~~");
}
void main() {
	iniTOT();
	//testare_module();
	//asteapta_comanda();
	addREP("8 jjj lijd 9 23.3");
	afisareREP(graf);
	/*deleteFAV("aeronava1");
	exit("gata");*/
}
