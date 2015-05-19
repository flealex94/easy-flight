#include "string.h"
#include "malloc.h"
#include "stdio.h"

struct modulRUZ {
	char* idZbor;
	int idAvion;
	char* oraPlecare;
	char* oraSosire;
	char* orasPlecare;
	char* orasSosire;
	float  costuriTotaleRuta;
};

struct NodLista {
	modulRUZ* info;
	NodLista* next;
};

struct HashT {
	(NodLista*)* vector;
	int dim;
};

NodLista* creareNod(modulRUZ* mod) {
	NodLista* nou = (NodLista*)malloc(sizeof(NodLista));
	nou->info = mod;
	nou->next = NULL;
	return nou;
}

void inserareNodLista(NodLista** lista, NodLista* nod) {
	NodLista* tempo = *lista;
	if (!tempo) {
		*lista = nod;
	} else {
		while (tempo->next)
			tempo = tempo->next;
		tempo->next = nod;
	}
}

HashT alocaMemorie() {
	HashT ht;
	ht.dim = 20;
	ht.vector = (NodLista**)malloc(sizeof(NodLista*)*ht.dim);
	for (int i = 0; i < ht.dim; i++) {
		ht.vector[i] = NULL;
	}
	return ht;

}

int functieHash(char* idZbor, int dim) {
	return idZbor[0] % dim;
}

void inserareHT(HashT ht, NodLista* nod) {
	//aici determinam pozitia in tabela de dispersie
	int poz = functieHash(nod->info->idZbor, ht.dim);
	NodLista* lista = ht.vector[poz];
	inserareNodLista(&lista, nod);
	//actualizare lista in tabela de dispersie
	ht.vector[poz] = lista;

}

void afisareModul(modulRUZ* mod) {
	printf(" %s %d %s %s %s %s %f\n", mod->idZbor, mod->idAvion, mod->oraPlecare, mod->oraSosire,
		   mod->orasPlecare, mod->orasSosire, mod->costuriTotaleRuta);
}

void afisareHt(HashT ht) {
	if (ht.vector) {
		for (int i = 0; i < ht.dim; i++) {
			NodLista* temp = ht.vector[i];
			while (temp) {
				afisareModul(temp->info);
				temp = temp->next;
			}
		}
	}
}


NodLista* modificaInHt(HashT ht, char* idZbor) {
	int poz = functieHash(idZbor, ht.dim);
	NodLista* lista = ht.vector[poz];
	if (strcmp(lista->info->idZbor, idZbor) == 0) {
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

		switch (ad) {
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
	} else return lista;
}

NodLista* cautaInHt(char* idZbor, HashT ht) {

	int poz = functieHash(idZbor, ht.dim);
	NodLista* list = ht.vector[poz];
	if (ht.vector[poz] == NULL) {
		return 0;
	} else if (strcmp(list->info->idZbor, idZbor) != 0)
		list = list->next;
	else if (strcmp(list->info->idZbor, idZbor) == 0) {
		printf("Cautare");
		printf(" %s %d %s %s %s %s %f\n", ht.vector[poz]->info->idZbor, ht.vector[poz]->info->idAvion, ht.vector[poz]->info->oraPlecare, ht.vector[poz]->info->oraSosire,
			   ht.vector[poz]->info->orasPlecare, ht.vector[poz]->info->orasSosire, ht.vector[poz]->info->costuriTotaleRuta);
	}
	return 0;

}

NodLista* deleteLista(NodLista* lista) {
	if (lista) {
		lista->next = deleteLista(lista->next);
		free(lista);
		return NULL;
	} else return NULL;
}

void deleteHashComplet(HashT ht) {
	if (ht.vector != NULL) {
		for (int i = 0; i < ht.dim; i++)
			ht.vector[i] = deleteLista(ht.vector[i]);
		free(ht.vector);
	}
}


//nu merge cum trebuie
NodLista* deleteElement(HashT ht, char* id) {
	int poz = functieHash(id, ht.dim);
	NodLista* list = ht.vector[poz];
	if (ht.vector[poz] == NULL) {
		return 0;
	} else if (strcmp(list->info->idZbor, id) != 0)
		list = list->next;
	else if (strcmp(list->info->idZbor, id) == 0) {
		NodLista* tmp = list;
		list = list->next;
		free(tmp);
	}
	return list;
}

void citireFisier(FILE* file) {
	HashT ht;
	ht = alocaMemorie();
	NodLista* lista = NULL;

	if (!file) {
		printf("Fisierul nu a fost gasit!");
	} else {
		modulRUZ* mod = NULL;
		char buf[100];
		fscanf(file, "%s", buf);


		while (!feof(file)) {
			modulRUZ* mod = (modulRUZ*)malloc(sizeof(modulRUZ));
			mod->idZbor = (char*)malloc(sizeof(char)*(strlen(buf) + 1));
			strcpy(mod->idZbor, buf);

			fscanf(file, "%d", &mod->idAvion);
			char buff[100];


			fscanf(file, "%s", buff);
			mod->oraPlecare = (char*)malloc(sizeof(char)*(strlen(buff) + 1));
			strcpy(mod->oraPlecare, buff);

			fscanf(file, "%s", buff);
			mod->oraSosire = (char*)malloc(sizeof(char)*(strlen(buff) + 1));
			strcpy(mod->oraSosire, buff);


			fscanf(file, "%s", buff);
			mod->orasPlecare = (char*)malloc(sizeof(char)*(strlen(buff) + 1));
			strcpy(mod->orasPlecare, buff);

			fscanf(file, "%s", buff);
			mod->orasSosire = (char*)malloc(sizeof(char)*(strlen(buff) + 1));
			strcpy(mod->orasSosire, buff);

			fscanf(file, "%f", &mod->costuriTotaleRuta);

			NodLista* nod = creareNod(mod);
			inserareHT(ht, nod);
			fscanf(file, "%s", buf);
		}
		afisareHt(ht);
		cautaInHt("25fgfg", ht);
		//modificaInHt(ht,"25fgfg");
		deleteElement(ht, "25fgfg");
		printf("==============\n");
		afisareHt(ht);

	}


}

void main() {
	FILE* file;
	file = fopen("program.txt", "r");
	citireFisier(file);
}
