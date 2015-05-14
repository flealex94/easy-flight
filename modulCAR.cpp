#include<stdio.h>
#include<malloc.h>
struct linieCAR {
	int idRezervare;
	int idZbor;
	float greutateMarfa;
	char nivelRisc;
};
struct NodAVL {
	linieCAR* car;
	int ge;
	NodAVL* st;
	NodAVL* dr;
};

linieCAR* creareCAR(int idR, int idZ, float grM, char nvR){
	linieCAR* car = (linieCAR*)malloc(sizeof(linieCAR));
	car->idRezervare = idR;
	car->idZbor = idZ;
	car->greutateMarfa = grM;
	car->nivelRisc = nvR;

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

void afisareCAR(linieCAR* car){
	printf("%d %d %f %d\n", car->idRezervare, car->idZbor, car->greutateMarfa, car->nivelRisc);
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
		int idRezervare, idZbor, nivelRisc;
		float greutateMarfa;
		fscanf(f, "%d", &idRezervare);
		while (!feof(f)){
			fscanf(f, "%d", &idZbor);
			fscanf(f, "%f", &greutateMarfa);
			fscanf(f, "%d", &nivelRisc);
			linieCAR* car = creareCAR(idRezervare,idZbor,greutateMarfa,nivelRisc);
			inserareAVL(arbore, car);

			fscanf(f, "%d", &idRezervare);
		}
	}
}


void main(){
	FILE* f = fopen("marfuri.txt","r");
	NodAVL* arbore = NULL;
	citireFisierCAR(f,arbore);

	SRD(arbore);
}