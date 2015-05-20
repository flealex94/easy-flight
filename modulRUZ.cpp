#include "string.h"
#include "malloc.h"
#include "stdio.h"
#include "stdlib.h"

struct linieRUZ
{
	char* idZbor;
	int idAvion;
	char* oraPlecare;
	char* oraSosire;
	char* orasPlecare;
	char* orasSosire;
	float  costuriTotaleRuta;
};

struct NodRUZ
{
	linieRUZ* info;
	NodRUZ* next;
};

struct HashRUZ
{
	(NodRUZ*)* listaRUZ;
	int dim;
};

HashRUZ hashtable;
NodRUZ* nod = nullptr;

linieRUZ* creareRUZ(char* idZbor, int idAvion, char* oraPlecare, char* oraSosire, char* orasPlecare,
	char* orasSosire, float costuriTotaleRuta)
{
	linieRUZ* ruz = (linieRUZ*)malloc(sizeof(linieRUZ));
	ruz->idZbor = (char*)malloc(sizeof(char)*(strlen(idZbor) + 1));
	strcpy(ruz->idZbor, idZbor);
	ruz->idAvion = idAvion;
	ruz->oraPlecare = (char*)malloc(sizeof(char)*(strlen(oraPlecare) + 1));
	strcpy(ruz->oraPlecare, oraPlecare);
	ruz->oraSosire = (char*)malloc(sizeof(char)*(strlen(oraSosire) + 1));
	strcpy(ruz->oraSosire, oraSosire);
	ruz->orasPlecare = (char*)malloc(sizeof(char)*(strlen(orasPlecare) + 1));
	strcpy(ruz->orasPlecare, orasPlecare);
	ruz->orasSosire = (char*)malloc(sizeof(char)*(strlen(orasSosire) + 1));
	strcpy(ruz->orasSosire, orasSosire);
	ruz->costuriTotaleRuta = costuriTotaleRuta;

	return ruz;
}

NodRUZ* creareNod(linieRUZ* mod)
{
	NodRUZ* nou = (NodRUZ*)malloc(sizeof(NodRUZ));
	nou->info = mod;
	nou->next = nullptr;
	return nou;
}

void inserareNodRUZ(NodRUZ** lista, NodRUZ* nod)
{
	NodRUZ* tempo = *lista;
	if (!tempo)
	{
		*lista = nod;
	}
	else
	{
		while (tempo->next)
		tempo = tempo->next;
		tempo->next = nod;
	}
}

HashRUZ alocaMemorieRUZ()
{
	HashRUZ ht;
	ht.dim = 20;
	ht.listaRUZ = (NodRUZ**)malloc(sizeof(NodRUZ*)*ht.dim);
	for (int i = 0; i < ht.dim; i++)
	{
		ht.listaRUZ[i] = nullptr;
	}
	return ht;

}

int functieHashRUZ(char* idZbor, int dim)
{
	return idZbor[0] % dim;
}

void inserareRUZ(HashRUZ ht, NodRUZ* nod)
{
	//aici determinam pozitia in tabela de dispersie
	int poz = functieHashRUZ(nod->info->idZbor, ht.dim);
	NodRUZ* lista = ht.listaRUZ[poz];
	inserareNodRUZ(&lista, nod);
	//actualizare lista in tabela de dispersie
	ht.listaRUZ[poz] = lista;

}

void afisareLinieRUZ(linieRUZ* ruz)
{
	if (ruz){
		printf(" %s %d %s %s %s %s %f\n", ruz->idZbor, ruz->idAvion, ruz->oraPlecare, ruz->oraSosire,
			ruz->orasPlecare, ruz->orasSosire, ruz->costuriTotaleRuta);
	}
	else printf("\nLista goala!");
}

void afisareRUZ(HashRUZ hashtable)
{
	if (hashtable.listaRUZ)
	{
		for (int i = 0; i < hashtable.dim; i++)
		{
			NodRUZ* ruz = hashtable.listaRUZ[i];
			while (ruz)
			{
				afisareLinieRUZ(ruz->info);
				ruz = ruz->next;
			}
		}
	}
	else printf("\nNu exista nicio ruta de zbor!");
}


NodRUZ* updateRUZ(char* idZbor,char* Date)
{
	//Punerea validarilor!
	int poz = functieHashRUZ(idZbor, hashtable.dim);
	NodRUZ* lista = hashtable.listaRUZ[poz];
	while (lista->next && (strcmp(lista->info->idZbor, idZbor) != 0))
		lista = lista->next;
	if (strcmp(lista->info->idZbor, idZbor) == 0)
	{
		char* da = (char*)malloc(sizeof(char)*strlen(Date));
		strcpy(da, Date);
		linieRUZ* ruz = (linieRUZ*)malloc(sizeof(linieRUZ));
		char* id;
		char s[] = " ";
		id = strtok(da, s);
		lista->info->idAvion = atoi(id);
		while (id != NULL){
			
			printf("%d ", lista->info->idAvion);
			id = strtok(NULL, " ");
			free(lista->info->oraPlecare);
			lista->info->oraPlecare = (char*)malloc(sizeof(char)*(strlen(id) + 1));
			strcpy(lista->info->oraPlecare, id);
			printf("%s ", lista->info->oraPlecare);
			id = strtok(NULL, " ");
			free(lista->info->oraSosire);
			lista->info->oraSosire = (char*)malloc(sizeof(char)*(strlen(id) + 1));
			strcpy(lista->info->oraSosire, id);
			printf("%s ", lista->info->oraSosire);
			id = strtok(NULL, " ");
			free(lista->info->orasPlecare);
			lista->info->orasPlecare = (char*)malloc(sizeof(char)*(strlen(id) + 1));
			strcpy(lista->info->orasPlecare, id);
			printf("%s ", lista->info->orasPlecare);
			id = strtok(NULL, " ");
			free(lista->info->orasSosire);
			lista->info->orasSosire = (char*)malloc(sizeof(char)*(strlen(id) + 1));
			strcpy(lista->info->orasSosire, id);
			printf("%s ", lista->info->orasSosire);
			id = strtok(NULL, " ");
			lista->info->costuriTotaleRuta = atof(id);
			printf("%f ", lista->info->costuriTotaleRuta);
			id = strtok(NULL, " ");

		}
		printf("\nModificare efectuata cu succes!");
	}
	else printf("\nNu exista ruta de zbor cu acest id!");
	 return lista;
}

linieRUZ* cautaElementRUZ(char* idZbor, HashRUZ ht) {

	int poz = functieHashRUZ(idZbor, ht.dim);
	NodRUZ* list = ht.listaRUZ[poz];

	while (list!=nullptr&&strcmp(list->info->idZbor, idZbor) != 0)
		list = list->next;
	 if (strcmp(list->info->idZbor, idZbor) == 0)
		return list->info;
	 else if (strcmp(list->info->idZbor, idZbor) != 0) return nullptr;
	

}

void findRUZ(HashRUZ ht, char* idZbor)
{
	linieRUZ* elem = cautaElementRUZ(idZbor, ht);
	if (elem)
		printf(" %s %d %s %s %s %s %f\n", elem->idZbor, elem->idAvion, elem->oraPlecare, elem->oraSosire,
		elem->orasPlecare, elem->orasSosire, elem->costuriTotaleRuta);
	else printf("\nNu exista aceasta ruta de zbor!");
}

//nu merge cum trebuie
//sterge elementul urmator
//pentru ultimul nu sterge nimic, lasa ht-ul intact
void deleteRUZ(HashRUZ ht, char* idZbor) {
	int poz = functieHashRUZ(idZbor, ht.dim);
	NodRUZ* list = ht.listaRUZ[poz];

	if (strcmp(list->info->idZbor, idZbor) == 0) {
		if (ht.listaRUZ[poz]->info == NULL) {
			free(ht.listaRUZ[poz]);
			ht.listaRUZ[poz] = NULL;
		}
		else {
			NodRUZ* date = ht.listaRUZ[poz];
			ht.listaRUZ[poz] = date->next;
			free(date);
		}
	}
	else {
		NodRUZ* date = ht.listaRUZ[poz];
		while (date->next != NULL&&strcmp(date->info->idZbor, idZbor) != 0)
			date = date->next;



		NodRUZ* da = date->next;
		if (da == NULL) {
			date->next = NULL;
			free(da);
		}
		else {
			date->next = da->next;
			free(da);
		}
	}
}

void intilializareRUZ(FILE* file, HashRUZ hashtable)
{	
	if (!file)
	{
		printf("Fisierul nu a fost gasit!");
	}
	else
	{
		char idZbor[20],oraPlecare[6],oraSosire[6],orasPlecare[20],orasSosire[20];
		int idAvion;
		float  costuriTotaleRuta;
		fscanf(file, "%s", idZbor);
		while (!feof(file))
		{

			fscanf(file, "%d", &idAvion);
			fscanf(file, "%s", &oraPlecare);
			fscanf(file, "%s", &oraSosire);
			fscanf(file, "%s", &orasPlecare);
			fscanf(file, "%s", &orasSosire);
			fscanf(file, "%f", &costuriTotaleRuta);
			linieRUZ* ruz = creareRUZ(idZbor, idAvion, oraPlecare, oraSosire, orasPlecare, orasSosire, costuriTotaleRuta);
			nod = creareNod(ruz);
			inserareRUZ(hashtable, nod);
			fscanf(file, "%s", idZbor);
		}

	}


}

void addRUZ(char* Date)
{
	char* da = (char*)malloc(sizeof(char)*strlen(Date));
	strcpy(da, Date);
	linieRUZ* ruz = (linieRUZ*)malloc(sizeof(linieRUZ));
	char* id;
	char s[] = " ";
	id = strtok( da, s);
	ruz->idZbor = (char*)malloc(sizeof(char)*(strlen(id) + 1));
	strcpy(ruz->idZbor, id);
	while (id != NULL){
		printf("%s ", ruz->idZbor);
		id = strtok(NULL, " ");
		ruz->idAvion = atoi(id);
		printf("%d ", ruz->idAvion);
		id = strtok(NULL, " ");
		ruz->oraPlecare = (char*)malloc(sizeof(char)*(strlen(id) + 1));
		strcpy(ruz->oraPlecare, id);
		printf("%s ", ruz->oraPlecare);
		id = strtok(NULL, " ");
		ruz->oraSosire = (char*)malloc(sizeof(char)*(strlen(id) + 1));
		strcpy(ruz->oraSosire, id);
		printf("%s ", ruz->oraSosire);
		id = strtok(NULL, " ");

		ruz->orasPlecare = (char*)malloc(sizeof(char)*(strlen(id) + 1));
		strcpy(ruz->orasPlecare, id);
		printf("%s ", ruz->orasPlecare);
		id = strtok(NULL, " ");
		ruz->orasSosire = (char*)malloc(sizeof(char)*(strlen(id) + 1));
		strcpy(ruz->orasSosire, id);
		printf("%s ", ruz->orasSosire);
		id = strtok(NULL, " ");
		ruz->costuriTotaleRuta = atof(id);
		printf("%f ", ruz->costuriTotaleRuta);
		id = strtok(NULL, " ");
	
	}
	NodRUZ* nod = creareNod(ruz);
	inserareRUZ(hashtable, nod);
}

void main()
{
	hashtable = alocaMemorieRUZ();
	FILE* file;
	file = fopen("program.txt", "r");
	intilializareRUZ(file, hashtable);
	afisareRUZ(hashtable);
	//findRUZ(hashtable, "hash3");
	updateRUZ("hash1", "1 12:30 13:50 bucur valcea 256.6");
	printf("\n=========");

	//addRUZ("hash10 1 12:30 13:50 bucuresti valcea 256.6");

	
	//stergereHash(hashtable,"hash1");
	//deleteRUZ(hashtable, "hash3");
	afisareRUZ(hashtable);

}
