#include "string.h"
#include "malloc.h"
#include "stdio.h"

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
	nou->next = NULL;
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
		ht.listaRUZ[i] = NULL;
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
	
	printf(" %s %d %s %s %s %s %f\n", ruz->idZbor, ruz->idAvion, ruz->oraPlecare, ruz->oraSosire,
		ruz->orasPlecare, ruz->orasSosire, ruz->costuriTotaleRuta);
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


NodRUZ* updateRUZ(HashRUZ ht, char* idZbor)
{

	int poz = functieHashRUZ(idZbor, ht.dim);
	NodRUZ* lista = ht.listaRUZ[poz];
	while (lista->next && (strcmp(lista->info->idZbor, idZbor) != 0))
		lista = lista->next;
	if (strcmp(lista->info->idZbor, idZbor) == 0)
	{
		printf("\nCe vrei sa modifici?\n");
		printf("1.Id-ul zborului\n");
		printf("2.Id-ul avionului\n");
		printf("3.Ora de plecare\n");
		printf("4.Ora de sosire\n");
		printf("5.Orasul de plecare\n");
		printf("6.Orasul de sosire\n");
		printf("7.Costurile total/ruta");


		int ad;
		printf("\nIntrodu cifra corespunzatoare alegerii:");
		scanf("%d", &ad);

		switch (ad)
		{
		case 1:
		{
				  free(lista->info->idZbor);
				  printf("\nIntroduceti noul id al zborului:");
				  char temp[100];
				  scanf("%s", &temp);
				  lista->info->idZbor = (char*)malloc(strlen(temp) + 1);
				  strcpy(lista->info->idZbor, temp);

		}break;

		case 2:{ printf("\nIntroduceti noul id al avionului:");
			int nr;
			scanf("%d", &nr);
			lista->info->idAvion = nr; }break;

		case 3:{
				   free(lista->info->oraPlecare);
				   printf("\nIntroduceti noua ora de plecare:");
				   char temp[100];
				   scanf("%s", &temp);
				   lista->info->oraPlecare = (char*)malloc(strlen(temp) + 1);
				   strcpy(lista->info->oraPlecare, temp); }break;

		case 4:{free(lista->info->oraSosire);
			printf("\nIntroduceti noua ora de sosire:");
			char temp[100];
			scanf("%s", &temp);
			lista->info->oraSosire = (char*)malloc(strlen(temp) + 1);
			strcpy(lista->info->oraSosire, temp); }break;

		case 5:{free(lista->info->orasPlecare);
			printf("\nIntroduceti noul oras de plecare:");
			char temp[100];
			scanf("%s", &temp);
			lista->info->orasPlecare = (char*)malloc(strlen(temp) + 1);
			strcpy(lista->info->orasPlecare, temp); }break;

		case 6:{free(lista->info->orasSosire);
			printf("\nIntroduceti noul oras de sosire:");
			char temp[100];
			scanf("%s", &temp);
			lista->info->orasSosire = (char*)malloc(strlen(temp) + 1);
			strcpy(lista->info->orasSosire, temp); }break;

		case 7:{
				   printf("\nIntroduceti costurile noi/ruta:");
				   int nr;
				   scanf("%f", &nr);
				   lista->info->costuriTotaleRuta = nr;
		}break;


		}
	}
	else return lista;
}

linieRUZ* cautaElementRUZ(char* idZbor, HashRUZ ht) {

	int poz = functieHashRUZ(idZbor, ht.dim);
	NodRUZ* list = ht.listaRUZ[poz];

	if (list!=NULL&&strcmp(list->info->idZbor, idZbor) != 0)
		list = list->next;
	 if (strcmp(list->info->idZbor, idZbor) == 0)
		return list->info;
	 else if (strcmp(list->info->idZbor, idZbor) != 0) return NULL;
	

}

NodRUZ* cautaElementRUZ1(char* idZbor, HashRUZ ht) {

	int poz = functieHashRUZ(idZbor, ht.dim);
	NodRUZ* list = ht.listaRUZ[poz];

	if (list != NULL&&strcmp(list->info->idZbor, idZbor) != 0)
		list = list->next;
	if (strcmp(list->info->idZbor, idZbor) == 0)
		return list;
	else if (strcmp(list->info->idZbor, idZbor) != 0) return NULL;


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
void deleteRUZ(HashRUZ ht, char* idZbor)
{
	NodRUZ* elem = cautaElementRUZ1(idZbor, ht);
	
	if (elem)
	{
		
		free(elem);
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
			NodRUZ* nod = creareNod(ruz);
			inserareRUZ(hashtable, nod);
			fscanf(file, "%s", idZbor);
		}

	}


}

void addRUZ(HashRUZ ht)
{
	linieRUZ* ruz = (linieRUZ*)malloc(sizeof(linieRUZ));
	printf("\nIntroduceti datele pentru o noua ruta de zbor:");
	 char idZbor[20], oraPlecare[6], oraSosire[6], orasPlecare[20], orasSosire[20];
	 int idAvion; float  costuriTotaleRuta;
	printf("ID zbor:");
	scanf("%s", &idZbor);
	ruz->idZbor = (char*)malloc(sizeof(char)*(strlen(idZbor) + 1));
	strcpy(ruz->idZbor, idZbor);
	printf("ID avion:");
	scanf("%d", &idAvion);
	printf("Ora plecare:");
	scanf("%s", &oraPlecare);
	printf("Ora sosire:");
	scanf("%s", &oraSosire);
	printf("Oras plecare:");
	scanf("%s", &orasPlecare);
	printf("Oras sosire:");
	scanf("%s", &orasSosire);
	printf("Costuri totale ruta:");
	scanf("%f", &costuriTotaleRuta);
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

	NodRUZ* nod = creareNod(ruz);
	inserareRUZ(ht, nod);
}

void main()
{
	HashRUZ hashtable;
	hashtable = alocaMemorieRUZ();
	FILE* file;
	file = fopen("program.txt", "r");
	intilializareRUZ(file, hashtable);
	afisareRUZ(hashtable);
	//findRUZ(hashtable, "hash19");
	//updateRUZ(hashtable, "hash2");
	printf("=========");
	//addRUZ(hashtable);

	deleteRUZ(hashtable, "hash2");
	afisareRUZ(hashtable);

}
