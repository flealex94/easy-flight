#include<stdio.h>
#include<malloc.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

struct linieCAR {
	int idRezervare;
	char* idZbor;
	float greutateMarfa;
	float nivelRisc;
	float costTotal;
};
struct NodAVL {
	linieCAR* car;
	int ge;
	NodAVL* st;
	NodAVL* dr;
};

NodAVL* arbore = NULL;

char* copiaza(char* string) {
	char* nou = (char*)malloc(sizeof(char)*(strlen(string) + 1));
	strcpy(nou, string);
	return nou;
}

linieCAR* creareCAR(int idR, char* idZ, float grM, float nvR){
	linieCAR* car = (linieCAR*)malloc(sizeof(linieCAR));
	car->idRezervare = idR;
	car->idZbor = copiaza(idZ);
	car->greutateMarfa = grM;
	car->nivelRisc = nvR;
	car->costTotal = car->greutateMarfa*car->nivelRisc * 7;

	return car;
}

int max(int a, int b){
	return (a>b?a:b);
}
int H(NodAVL* rad){
	if (rad)	return 1 + max(H(rad->st), H(rad->dr));
	else		return 0;
}
void gradEchilibru(NodAVL* rad){
	if (rad)	rad->ge = H(rad->dr) - H(rad->st);
}

NodAVL* rotatieSimplaStanga(NodAVL* pivot, NodAVL* fiuDR){
	pivot->dr = fiuDR->st;
	gradEchilibru(pivot);
	fiuDR->st = pivot;
	gradEchilibru(fiuDR);
	return fiuDR;
}

NodAVL* rotatieSimplaDreapta(NodAVL* pivot, NodAVL* fiuSt){
	pivot->st = fiuSt->dr;
	gradEchilibru(pivot);
	fiuSt->dr = pivot;
	gradEchilibru(fiuSt);
	return fiuSt;
}

NodAVL* rotatieDublaStDr(NodAVL* pivot, NodAVL* fiuSt){
	pivot->st = rotatieSimplaStanga(fiuSt, fiuSt->dr);
	gradEchilibru(pivot);
	fiuSt = pivot->st;
	fiuSt = rotatieSimplaDreapta(pivot, fiuSt);
	gradEchilibru(fiuSt);
	return fiuSt;
}

NodAVL* rotatieDublaDrSt(NodAVL* pivot, NodAVL* fiuDr){
	pivot->dr = rotatieSimplaDreapta(fiuDr, fiuDr->st);
	gradEchilibru(pivot);
	fiuDr = pivot->dr;
	fiuDr = rotatieSimplaStanga(pivot, fiuDr);
	gradEchilibru(fiuDr);
	return fiuDr;
}

void inserareAVL(NodAVL* &rad, linieCAR* carg){
	if (rad)	{
		if (rad->car->idRezervare < carg->idRezervare)
			inserareAVL(rad->dr, carg);
		else
			if (rad->car->idRezervare > carg->idRezervare)
				inserareAVL(rad->st, carg);
			else
				printf("Elementul %d este deja prezent in arbore.\n",carg->idRezervare);
	}
	else{
		rad = (NodAVL*)malloc(sizeof(NodAVL));
		rad->car = carg;
		rad->dr = NULL;
		rad->st = NULL;
	}

	gradEchilibru(rad);
	if (rad->ge == 2)
		if (rad->ge == -1)
			rad = rotatieDublaDrSt(rad, rad->dr);
		else
			rad = rotatieSimplaStanga(rad, rad->dr);
	else
		if (rad->ge == -2)
		{
			if (rad->ge == 1)
				rad = rotatieDublaStDr(rad, rad->st);
			else
				rad = rotatieSimplaDreapta(rad, rad->st);
		}

}

NodAVL* cautaElementCAR(int id)
{
	if (arbore != NULL)
	{
		NodAVL* origine = arbore;
		do
		{
			if (arbore->car->idRezervare == id)
				return arbore;
			if (arbore->car->idRezervare > id)
				arbore = arbore->st;
			else arbore = arbore->dr;
		} while (arbore != origine);
	}
}
void updateCAR(char* DATE)
{
	char* dat = copiaza(DATE);
	char* aux;
	int id;
	float greutateMarfa, nivelRisc, costTotal;
	aux = strtok(dat, " \t");
	id = atoi(aux);
	NodAVL* elem = cautaElementCAR(id);
	if (elem != nullptr){
		linieCAR* linie = elem->car;
		linie->idRezervare = id;
		char* idZb = strtok(NULL," \t");
		free(linie->idZbor);
		linie->idZbor = copiaza(idZb);
		char* aux = strtok(NULL, " \t");
		linie->greutateMarfa = atof(aux);
		aux = strtok(NULL, " \t");
		linie->nivelRisc = atof(aux);
		linie->costTotal = linie->costTotal*linie->nivelRisc * 7;
	}//echo("Date gestiune cargo actualizate!\n");
}
 //else echo("\nElementul nu a fost gasit!\n");


void addCAR(char* DATE)
{
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
	inserareAVL(arbore, car);
	//echo("Gestiune cargo adaugata!\n");
}

void afisareCAR(linieCAR* car){
	printf("%d %s %f %f %f\n", car->idRezervare, car->idZbor, car->greutateMarfa, car->nivelRisc,car->costTotal);
}
void SRD(NodAVL* rad){
	if (rad)
	{
		SRD(rad->st);
		afisareCAR(rad->car);
		SRD(rad->dr);
	}
}



void citireFisierCAR(FILE* f, NodAVL* &arbore){
	if (!f){
		printf("Nu se poate deschide fisierul!\n");
	}
	else{
		int idRezervare;
		float nivelRisc;
		char idZbor[100];
		float greutateMarfa;
		fscanf(f, "%d", &idRezervare);
		while (!feof(f)){
			fscanf(f, "%s", &idZbor);
			fscanf(f, "%f", &greutateMarfa);
			fscanf(f, "%f", &nivelRisc);
			linieCAR* car = creareCAR(idRezervare,idZbor,greutateMarfa,nivelRisc);
			inserareAVL(arbore, car);

			fscanf(f, "%d", &idRezervare);
		}
	}
}


void main(){
	FILE* file = fopen("marfuri.txt","r");
	citireFisierCAR(file,arbore);
	addCAR("5 ana 3 5");
	SRD(arbore);
	printf("\n-----------");
	updateCAR("5 maria 30 4");
	SRD(arbore);
}
