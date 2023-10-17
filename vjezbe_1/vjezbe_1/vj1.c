#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>


typedef struct {
	char ime[50]; 
	char prezime[50];
	int br_bodova;
}Osoba;

int main() {
	int n=1;//broj redaka
	int i;
	int max_br_bodova;
	float  relativan_br_bodova;
	FILE *fp;
	fp = fopen("popis.txt", "r");
	if (fp == NULL) {
		puts("Pogreska!");
		return 1;
	}
	while (!feof(fp)) {
		char z = getc(fp);
		if (z=='\n') {
			n++;
		}
	}
	Osoba *osobe;
	osobe = (Osoba *)malloc(sizeof(Osoba)*n);//dinamicka alokacija memorije
	
	
	printf("Unesite max broj bodova:\n");
	scanf("%d", &max_br_bodova);
	printf("Popis:\n");
	fseek(fp, 0, SEEK_SET);
	for (i = 0; i < n; i++) {
		fscanf(fp,"%s %s %d", osobe[i].ime, osobe[i].prezime, &osobe[i].br_bodova);
		relativan_br_bodova = (float)osobe[i].br_bodova / max_br_bodova * 100;
		printf("Ime: %s.\nPrezime: %s.\nApsolutan broj bodova: %d.\nRelativan broj bodova: %f.\n", osobe[i].ime, osobe[i].prezime, osobe[i].br_bodova, relativan_br_bodova);
	}
	fclose(fp);
	free(osobe);


	return 0;
}