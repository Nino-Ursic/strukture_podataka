#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/*
2. Definirati strukturu osoba (ime, prezime, godina roðenja) i napisati program koji:
A. dinamièki dodaje novi element na poèetak liste,
B. ispisuje listu,
C. dinamièki dodaje novi element na kraj liste,
D. pronalazi element u listi (po prezimenu),
E. briše odreðeni element iz liste,
U zadatku se ne smiju koristiti globalne varijable.
*/
typedef struct osoba Osoba;
typedef Osoba* Pozicija;

struct osoba {
    char ime[50];
    char prezime[50];
    int godina_rodenja;
    Pozicija pozicija;
};

void dodaj_na_pocetak(Pozicija head);
int ispis_liste(Pozicija head);
void dodaj_na_kraj(Pozicija head);
void pronadi_prezime(Pozicija head);
void izbrisi_element(Pozicija head);


int main() {

    Osoba head = { .ime = {0}, .prezime = {0}, .godina_rodenja = {0} };

    dodaj_na_pocetak(&head);
    dodaj_na_pocetak(&head);
    dodaj_na_kraj(&head);
    ispis_liste(&head);
    pronadi_prezime(&head);
    izbrisi_element(&head);
    ispis_liste(&head);


    return 0;
};

void dodaj_na_pocetak(Pozicija head) {
    Pozicija novi;
    novi = (Pozicija)malloc(sizeof(Osoba));

    novi->pozicija = head->pozicija;
    printf("Unesite ime:");
    scanf("%s", novi->ime);
    printf("Unesite prezime:");
    scanf("%s", novi->prezime);
    printf("Unesite godinu rodenja:");
    scanf("%d", &novi->godina_rodenja);

    head->pozicija = novi;
}
int ispis_liste(Pozicija head) {
    Pozicija temp = head;

    while (1) {
        temp = temp->pozicija;
        printf("ime: %s\nprezime: %s\nGodina rodenja: %d\n", temp->ime, temp->prezime, temp->godina_rodenja);
        if (temp->pozicija == NULL) {
            return 0;
        }
    }

}
void dodaj_na_kraj(Pozicija head) {
    Pozicija zadnji = head;

    while (zadnji->pozicija != NULL) {
        zadnji = zadnji->pozicija;
    }
    Pozicija novi;
    novi = (Pozicija)malloc(sizeof(Osoba));

    novi->pozicija = zadnji->pozicija;
    printf("Unesite ime:");
    scanf("%s", novi->ime);
    printf("Unesite prezime:");
    scanf("%s", novi->prezime);
    printf("Unesite godinu rodenja:");
    scanf("%d", &novi->godina_rodenja);
    zadnji->pozicija = novi;

}
void pronadi_prezime(Pozicija head) {
    Pozicija temp = head;
    printf("Unesite traženo prezime:");
    char trazi[50];
    scanf("%s", trazi);

    while (temp->pozicija != NULL) {
        temp = temp->pozicija;
        if (strcmp(temp->prezime, trazi) == 0) {
            break;
        }
    }
    printf("ime: %s\nprezime: %s\nGodina rodenja: %d\n", temp->ime, temp->prezime, temp->godina_rodenja);
}
void izbrisi_element(Pozicija head) {
    Pozicija temp = head;
    Pozicija prethodni = head;
    Pozicija trazeni = head;
    printf("Unesite traženo ime:");
    scanf("%s", trazeni->ime);
    printf("Unesite traženo prezime:");
    scanf("%s", trazeni->prezime);
    printf("Unesite traženu godinu:");
    scanf("%d", &trazeni->godina_rodenja);
    while (temp->pozicija != NULL) {
        temp = temp->pozicija;
        if (strcmp(temp->prezime, trazeni->prezime) == 0 && strcmp(temp->ime, trazeni->ime) == 0 && temp->godina_rodenja == trazeni->godina_rodenja) {
            break;
        }
        prethodni = prethodni->pozicija;
    }
    if (temp->pozicija != NULL) {
        prethodni->pozicija = temp->pozicija;
        free(temp);
    }
    else {
        printf("Element not found.\n");
    }
}
