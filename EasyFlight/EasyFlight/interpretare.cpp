#include <stdio.h>
#include <string.h>

struct picker {
	char* comenzi[3];
	void(*functii[3])(char*);
	int getPoz(char* com, char** comenzi){
		int poz = 0;
		while (strcpy(com, comenzi[poz]) != 0)
			poz++;
		return poz;
	}
	void(*getFunc(char* com))(char*){
		return functii[getPoz(com, comenzi)];
	}
}picker;
void someFunk(char* somestring){
	printf("%s", somestring);
}
int main(){
	picker.comenzi[0] = new char[7];
	strcpy(picker.comenzi[0], "asdf");
	picker.functii[0] = someFunk;
	picker.getFunc("asdf")("omg merge!");

	return 0;
}