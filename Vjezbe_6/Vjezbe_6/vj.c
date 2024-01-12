#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
6. Napisati program koji èita datoteku racuni.txt u kojoj su zapisani nazivi svih datoteka koji
predstavljaju pojedini raèun. Na poèetku svake datoteke je zapisan datum u kojem vremenu je
raèun izdat u formatu YYYY-MM-DD. Svaki sljedeæi red u datoteci predstavlja artikl u formatu
naziv, kolièina, cijena. Potrebno je formirati vezanu listu raèuna sortiranu po datumu. Svaki èvor
vezane liste sadržava vezanu listu artikala sortiranu po nazivu artikla. Nakon toga potrebno je
omoguæiti upit kojim æe korisnik saznati koliko je novaca sveukupno potrošeno na specifièni
artikl u odreðenom vremenskom razdoblju i u kojoj je kolièini isti kupljen.
*/

typedef struct article Article;
typedef Article* Position_A;
struct article {
    char name[50];
    int quantity;
    double price;
    Position_A next;
};

typedef struct {
    int year;
    int month;
    int day;
}date;

typedef struct invoice Invoice;
typedef Invoice* Position_I;
struct invoice {
    date invoice_date;
    Position_A head_A;
    Position_I next;
};

int read_from_file(Position_I head_I);
int add_inovice_sorted(Position_I head_I, char file_name[]);
int add_article_sorted(Position_A head_A, char name[], int quantity, double price);
int request(Position_I head_I);

int main()
{


    Invoice head_I = { .head_A = NULL, .next = NULL };

    if (read_from_file(&head_I)) {
        printf("Program failed\n");
        return 1;
    }

    if (request(&head_I)) {
        printf("Request failed\n");
    }

    return 0;
}

int read_from_file(Position_I head_I) {

    char file_name[50];

    FILE* file_pointer = NULL;
    file_pointer = fopen("racuni.txt", "r");

    if (file_pointer == NULL) {
        printf("ERROR failed openning racuni.txt\n");
        return 1;
    }

    while (fscanf(file_pointer, " %s", file_name) == 1) {

        if (add_inovice_sorted(head_I, file_name)) {
            printf("ERROR: failed to add invoice.\n");
            return 1;
        }

    }

    fclose(file_pointer);

    return 0;
}

int add_inovice_sorted(Position_I head_I, char file_name[]) {

    char article_name[50];
    int quantity = 0;
    double price = 0.0;

    Position_I temp = head_I;

    Position_I new_invoice = NULL;
    new_invoice = (Position_I)malloc(sizeof(Invoice));

    if (new_invoice == NULL) {
        printf("ERROR: failed alocating memory.\n");
        return 1;
    }


    new_invoice->head_A = (Position_A)malloc(sizeof(Article));
    if (new_invoice->head_A == NULL) {
        printf("ERROR: failed allocating memory for head_A.\n");
        return 1;
    }

    new_invoice->head_A->quantity = 0;
    new_invoice->head_A->price = 0;
    new_invoice->head_A->next = NULL;

    new_invoice->next = NULL;

    new_invoice->invoice_date.year = 0;
    new_invoice->invoice_date.month = 0;
    new_invoice->invoice_date.day = 0;



    FILE* file_pointer = NULL;
    file_pointer = fopen(file_name, "r");

    if (file_pointer == NULL) {
        printf("ERROR opening invoice file.\n");
        return 1;
    }


    if (fscanf(file_pointer, " %d-%d-%d\n", &new_invoice->invoice_date.year, &new_invoice->invoice_date.month, &new_invoice->invoice_date.day) != 3) {
        printf("ERROR reading invoice date.\n");
        return 1;
    }


    while (fscanf(file_pointer, " %50[^,], %d, %lf\n", article_name, &quantity, &price) == 3) {

        if (add_article_sorted(new_invoice->head_A, article_name, quantity, price)) {
            printf("ERROR: failed to add article.\n");
            return 1;
        }

    }

    if (head_I->next != NULL) {
        while (temp != NULL) {
            if (temp->next->invoice_date.year > new_invoice->invoice_date.year || temp->next->invoice_date.year == new_invoice->invoice_date.year && (temp->next->invoice_date.month > new_invoice->invoice_date.month || temp->next->invoice_date.month == new_invoice->invoice_date.month && temp->next->invoice_date.day > new_invoice->invoice_date.day)) {
                break;
            }

            temp = temp->next;
        }
    }

    new_invoice->next = temp->next;
    temp->next = new_invoice;



    fclose(file_pointer);

    return 0;
}

int add_article_sorted(Position_A head_A, char name[], int quantity, double price) {

    Position_A temp = head_A;
    Position_A new_article = NULL;

    new_article = (Position_A)malloc(sizeof(Article));

    if (new_article == NULL) {
        printf("ERROR alocating memory for new article.\n");
        return 1;
    }


    strcpy(new_article->name, name);
    new_article->quantity = quantity;
    new_article->price = price;
    new_article->next = NULL;


    if (head_A->next != NULL) {
        while (temp->next != NULL && strcmp(new_article->name, temp->next->name) > 0) {
            temp = temp->next;
        }
    }

    new_article->next = temp->next;
    temp->next = new_article;

    return 0;
}

int request(Position_I head_I) {

    char name[50];
    date start;
    date finish;

    int quantity = 0;
    double price = 0.0;
    int flag = 1;

    Position_I temp = head_I->next;

    printf("\nUnesite ime artikla:");
    scanf("%s", name);

    printf("\nUnesite poèetni datum(dan, mjesec i godinu)\n");
    scanf("%d %d %d", &start.day, &start.month, &start.year);

    printf("\nUnesite konaèni datum(dan, mjesec i godinu)\n");
    scanf("%d %d %d", &finish.day, &finish.month, &finish.year);


    while (temp != NULL) {

        Position_A temp_A = temp->head_A->next;

        if (temp->invoice_date.year > finish.year || temp->invoice_date.year == finish.year && (temp->invoice_date.month > finish.month || (temp->invoice_date.month == finish.month && temp->invoice_date.day > finish.day))) {
            break;
        }

        if (temp->invoice_date.year > start.year || (temp->invoice_date.year == start.year && (temp->invoice_date.month > start.month || temp->invoice_date.month == start.month && temp->invoice_date.day > start.day))) {

            while (temp_A != NULL) {

                if (strcmp(temp_A->name, name) == 0) {
                    flag = 0;
                    quantity += temp_A->quantity;
                    price += (temp_A->price * temp_A->quantity);
                }

                temp_A = temp_A->next;
            }
        }


        temp = temp->next;
    }

    if (flag) {
        printf("Traženi proizvod nije pronaðen\n");
        return 1;
    }

    printf("Kupljena kolièina je: %d\n", quantity);
    printf("Cijena te kolièine je: %lf\n", price);

    return 0;
}
