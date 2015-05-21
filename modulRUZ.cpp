//#include "string.h"
//#include "malloc.h"
//#include "stdio.h"
//#include "stdlib.h"
//#include <time.h>
//#include <ctype.h>
//FILE* program;
//FILE* log;
//char* copiaza(char* string) {
//	char* nou = (char*)malloc(sizeof(char)*(strlen(string) + 1));
//	strcpy(nou, string);
//	return nou;
//}
//void ftimestamp() {
//	time_t ltime; /* calendar time */
//	ltime = time(NULL); /* get current cal time */
//	char* time;
//	time = copiaza(asctime(localtime(&ltime)));
//	time[strlen(time) - 1] = '\0';
//	fprintf(log, "%s: ", time);
//}
//void echo(char* msg) {
//	printf(msg);
//	ftimestamp();
//	fprintf(log, msg);
//}
//struct linieRUZ {
//	char* idZbor;
//	char* idAvion;
//	int oraPlecare;
//	int oraSosire;
//	char* orasPlecare;
//	char* orasSosire;
//	float costuriTotaleRuta;
//};
//struct NodRUZ {
//	linieRUZ* info;
//	NodRUZ* next;
//};
//struct HashRUZ {
//	(NodRUZ*)* listaRUZ;
//	int dim;
//};
//
//HashRUZ hashTable;
//
//linieRUZ* creareRUZ(char* idZbor, char* idAvion, int oraPlecare, int oraSosire, char* orasPlecare,
//					char* orasSosire, float costuriTotaleRuta) {
//	linieRUZ* ruz = (linieRUZ*)malloc(sizeof(linieRUZ));
//	ruz->idZbor = copiaza(idZbor);
//	ruz->idAvion = copiaza(idAvion);
//	ruz->oraPlecare = oraPlecare;
//	ruz->oraSosire = oraSosire;
//	ruz->orasPlecare = copiaza(orasPlecare);
//	ruz->orasSosire = copiaza(orasSosire);
//	ruz->costuriTotaleRuta = costuriTotaleRuta;
//
//	return ruz;
//}
//NodRUZ* creareNod(linieRUZ* nod) {
//	NodRUZ* nou = (NodRUZ*)malloc(sizeof(NodRUZ));
//	nou->info = nod;
//	nou->next = nullptr;
//	return nou;
//}
//void inserareNodRUZ(NodRUZ* &lista, NodRUZ* nod) {
//	if (lista == nullptr) {
//		lista = nod;
//	} else {
//		while (lista->next)
//			lista = lista->next;
//		lista->next = nod;
//	}
//}
//void alocaMemorieRUZ() {
//	hashTable.dim = 20;
//	hashTable.listaRUZ = (NodRUZ**)malloc(sizeof(NodRUZ*)*hashTable.dim);
//	for (int i = 0; i < hashTable.dim; i++) {
//		hashTable.listaRUZ[i] = nullptr;
//	}
//}
//int functieHashRUZ(char* idZbor) {
//	int sum = 0;
//	while (*idZbor) {
//		sum = sum*hashTable.dim + *(idZbor++);
//	}
//	return sum % hashTable.dim;
//}
//void inserareRUZ(NodRUZ* nod) {
//	int poz = functieHashRUZ(nod->info->idZbor);
//	NodRUZ* lista = hashTable.listaRUZ[poz];
//	inserareNodRUZ(lista, nod);
//	hashTable.listaRUZ[poz] = lista;
//}
//void intilializareRUZ() {
//	alocaMemorieRUZ();
//	program = fopen("program.txt", "r");
//	if (!program) {
//		echo("\nFisierul nu a fost gasit!\n");
//	} else {
//		char idZbor[20], orasPlecare[20], orasSosire[20], idAvion[20];
//		int oraPlecare, oraSosire;
//		float  costuriTotaleRuta;
//		fscanf(program, "%s", idZbor);
//		while (!feof(program)) {
//			fscanf(program, "%s", idAvion);
//			fscanf(program, "%d", &oraPlecare);
//			fscanf(program, "%d", &oraSosire);
//			fscanf(program, "%s", orasPlecare);
//			fscanf(program, "%s", orasSosire);
//			fscanf(program, "%f", &costuriTotaleRuta);
//			linieRUZ* ruz = creareRUZ(idZbor, idAvion, oraPlecare, oraSosire, orasPlecare, orasSosire, costuriTotaleRuta);
//			NodRUZ* nod = creareNod(ruz);
//			inserareRUZ(nod);
//
//			fscanf(program, "%s", idZbor);
//		}
//	}
//}
//
//void afisareLinieRUZ(linieRUZ* ruz) {
//	if (ruz) {
//		char msg[54];
//		sprintf(msg, "\t%s %s %d %d %s %s %f\n", ruz->idZbor, ruz->idAvion, ruz->oraPlecare, ruz->oraSosire,
//				ruz->orasPlecare, ruz->orasSosire, ruz->costuriTotaleRuta);
//		echo(msg);
//	} /*else echo("\Element vid!\n");*/
//}
//void afisareRUZ() {
//	if (hashTable.listaRUZ != nullptr) {
//		for (int i = 0; i < hashTable.dim; i++) {
//			NodRUZ* ruz = hashTable.listaRUZ[i];
//			while (ruz) {
//				afisareLinieRUZ(ruz->info);
//				ruz = ruz->next;
//			}
//		}
//	} else echo("\nNu exista nicio ruta de zbor!\n");
//}
//NodRUZ* cautaElementRUZ(char* idZbor) {
//	int poz = functieHashRUZ(idZbor);
//	NodRUZ* list = hashTable.listaRUZ[poz];
//	while (list != nullptr && strcmp(list->info->idZbor, idZbor) != 0)
//		list = list->next;
//	if (strcmp(list->info->idZbor, idZbor) == 0)
//		return list;
//	else return nullptr;
//}
//
//void findRUZ(char* idZbor) {
//	NodRUZ* elem = cautaElementRUZ(idZbor);
//	if (elem != nullptr) {
//		linieRUZ* linie = elem->info;
//		char msg[54];
//		sprintf(msg, "\t%s %s %d %d %s %s %f\n", linie->idZbor, linie->idAvion, linie->oraPlecare, linie->oraSosire,
//				linie->orasPlecare, linie->orasSosire, linie->costuriTotaleRuta);
//		echo(msg);
//	} else echo("\nNu exista aceasta ruta de zbor!\n");
//}
//
//void updateRUZ(char* DATE) {
//	char* dat = copiaza(DATE);
//	char* idZbor = strtok(dat, " \t");
//	int poz = functieHashRUZ(idZbor);
//	NodRUZ* lista = hashTable.listaRUZ[poz];
//	NodRUZ* elem = cautaElementRUZ(idZbor);
//	if (elem != nullptr) {
//		linieRUZ* linie = elem->info;
//		char* aux = strtok(NULL, " \t"); free(linie->idAvion); linie->idAvion = copiaza(aux);
//		aux = strtok(NULL, " \t"); linie->oraPlecare = atoi(aux);
//		aux = strtok(NULL, " \t"); linie->oraSosire = atoi(aux);
//		aux = strtok(NULL, " \t"); free(linie->orasPlecare); linie->orasPlecare = copiaza(aux);
//		aux = strtok(NULL, " \t"); free(linie->orasSosire);	linie->orasSosire = copiaza(aux);
//		aux = strtok(NULL, " \t"); linie->costuriTotaleRuta = atof(aux);
//
//		echo("Modificare efectuata cu succes!\n");
//	} else echo("\nNu exista ruta de zbor cu acest id!\n");
//}
//
//void addRUZ(char* DATE) {
//	char* dat = copiaza(DATE);
//	char* aux;
//	char oraPlecare, oraSosire;
//	float  costuriTotaleRuta;
//
//	char* idZbor = strtok(dat, " \t");
//	char* idAvion = strtok(NULL, " \t");
//	aux = strtok(NULL, " \t"); oraPlecare = atoi(aux);
//	aux = strtok(NULL, " \t"); oraSosire = atoi(aux);
//	char* orasPlecare = strtok(NULL, " \t");
//	char* orasSosire = strtok(NULL, " \t");
//	aux = strtok(NULL, " \t"); costuriTotaleRuta = atof(aux);
//	linieRUZ* ruz = creareRUZ(idZbor, idAvion, oraPlecare, oraSosire, orasPlecare, orasSosire, costuriTotaleRuta);
//	NodRUZ* nod = creareNod(ruz);
//	inserareRUZ(nod);
//	echo("Adaugare efectuata cu succes!\n");
//}
//
//void deleteRUZ(char* idZbor) {
//	NodRUZ* elem = cautaElementRUZ(idZbor);
//	if (elem != nullptr) {
//		int poz = functieHashRUZ(idZbor);
//		NodRUZ* lista = hashTable.listaRUZ[poz];
//		if (lista == elem) {
//			hashTable.listaRUZ[poz] = elem->next;
//		} else {
//			NodRUZ* tmp = lista;
//			while ((tmp->next != nullptr) && (tmp->next != elem)) {
//				tmp = tmp->next;
//			}
//			tmp->next = tmp->next->next;
//			hashTable.listaRUZ[poz] = lista;
//			//free(tmp->info->idAvion); free(tmp->info->idZbor); free(tmp->info->orasPlecare); free(tmp->info->orasSosire); free(tmp->info); free(tmp);
//		}
//		echo("Stergere efectuata cu succes!\n");
//	} else echo("\nNu exista ruta de zbor cu acest id!\n");
//}
//
//void main() {
//	log = fopen("log.txt", "wt");
//	intilializareRUZ();
//	//afisareRUZ();
//	//findRUZ("hash3");
//	//updateRUZ("hash1 11asdf 13 17 bucur valcea 256.6");
//
//	addRUZ("hash10 13asdf 12 15 bucuresti valcea 256.6");
//	deleteRUZ("hash10");
//	afisareRUZ();
//}
