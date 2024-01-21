/*
Zadatak za oslobaðanje praktiènog dijela ispita:

"BookHaven" je knjižnica koja želi unaprijediti svoj sustav praæenja knjiga, autora i korisnika.
Svaka knjiga ima informacije o naslovu, autoru, godini izdanja i dostupnim primjercima.
Korisnici mogu posuðivati knjige i vratiti ih nakon nekog vremena.

Program mora korisniku omoguæiti:
    a) ispis svih knjiga abecedno (tako da se mogu vidjeti svi podatci o knjizi i korisnici koji trenutno posuðuju knjigu)
    b) ispis svih korisnika abecedno (tako da se mogu vidjeti sve knjige koje je neki korisnik posudio)
    c) pretraživanje knjiga po godini izdanja (tako da se mogu vidjeti sve knjige iz te godine i njihova kolièina)
    d) pretraživanje knjiga po nazivu autora (tako da se mogu vidjeti sve knjige tog autora i njihova kolièina)
    e) unos novog korisnika
    f) posudba knjige korisniku
        - može više primjeraka iste knjige posuditi od puta
        - korisnik u sebe nikad ne smije imati više od 5 knjiga sve skupa
        - ako pokušava napravit posudbu koja æe ga dovesti preko tog broja program treba upozoriti korisnika na to i ispisati mu broj koliko još knjiga posudit
        - ako korisnik pita za više primjeraka knjige, a knjižnica na stanju nema nema dovoljno treba korisniku omoguæiti izbor hoæe li posudit to što ima ili ne.
    g) povratak knjiga na stanje
    h) spremi (u datoteku/datoteke spremiti trenutno stanje u knjižnici, tako da kad se iduæi put pokrene program moguæe nastaviti od spremljenog stanja)

Sami odaberite strukturu podataka i format datoteka koju æe te koristiti za ovo izvesti.
*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct book Book;
typedef Book* book_position;

typedef struct user User;
typedef User* user_position;

struct book {
    char name[50];
    char author[50];
    int publication_year;
    int available;
    user_position head_user;
    book_position next;
};

struct user {
    char name[50];
    int book_number;
    book_position head_borrowed_books;
    user_position next;
};

typedef struct hash Hash;
typedef Hash* hash_position;
struct hash {
    book_position book;
    hash_position next;
};

int load_books(book_position head_book, hash_position* year_hash, hash_position* author_hash);
int load_users(user_position head_user);
int add_book_sorted(book_position head_book, book_position new_book);
int add_user_sorted(user_position head_user, user_position new_user);
int add_to_hash_year(hash_position* year_hash, book_position new_book);
int add_to_hash_author(hash_position* year_hash, book_position new_book);
int author_hash_key(char* name);
book_position alocate_book();
user_position alocate_user();

int print_books_alphabetical(book_position head_book);
int print_users_alphabetical(user_position head_user);
int find_book_year(hash_position* year_hash);
int find_book_author(hash_position* author_hash);
int add_user(user_position head_user);
int borrow_book(book_position head_book, user_position head_user);
int return_book(book_position head_book, user_position head_user);
int remove_from_list(user_position head_user, char* user_name);
int save_books(book_position head_book);
int save_users(user_position head_user);


int main() {

    book_position head_book;
    head_book = alocate_book();

    user_position head_user;
    head_user = alocate_user();

    hash_position year_hash[100];
    hash_position author_hash[101];

    for (int i = 0; i < 100; i++) {
        year_hash[i] = NULL;
        author_hash[i] = NULL;
    }

    load_books(head_book, year_hash, author_hash);
    load_users(head_user);

    while (1) {
        printf("OPTIONS:\n");
        printf("0) IZLAZ\n");
        printf("1) ispis svih knjiga abecedno\n");
        printf("2) ispis svih korisnika abecedno\n");
        printf("3) pretraživanje knjiga po godini izdanja\n");
        printf("4) pretraživanje knjiga po nazivu autora\n");
        printf("5) unos novog korisnika\n");
        printf("6) posudba knjige\n");
        printf("7) povratak knjige na stanje\n");
        printf("8) spremi stanje\n\n");

        printf("izaberite opciju:");

        int choice = 0;
        scanf("%d", &choice);

        switch (choice) {
        case 0:
            return 0;
        case 1:
            print_books_alphabetical(head_book);
            break;
        case 2:
            print_users_alphabetical(head_user);
            break;
        case 3:
            find_book_year(year_hash);
            break;
        case 4:
            find_book_author(author_hash);
            break;
        case 5:
            add_user(head_user);
            break;
        case 6:
            borrow_book(head_book, head_user);
            break;
        case 7:
            return_book(head_book, head_user);
            break;
        case 8:
            save_books(head_book);
            save_users(head_user);
            break;
        default:
            printf("Invalid choice\n");
            break;
        }
    }

    return 0;
}

int load_books(book_position head_book, hash_position* year_hash, hash_position* author_hash) {

    FILE* file = NULL;
    file = fopen("library.txt", "r");

    if (file == NULL) {
        printf("No previous book records found\n");
        return 1;
    }

    book_position temp = alocate_book();
    book_position new_book = NULL;
    user_position new_user = NULL;
    char buffer[1024];

    while (fscanf(file, "%49[^,], %49[^,], %d, %d\n", temp->name, temp->author, &temp->publication_year, &temp->available) == 4) {
        new_book = alocate_book();
        strcpy(new_book->name, temp->name);
        strcpy(new_book->author, temp->author);
        new_book->publication_year = temp->publication_year;
        new_book->available = temp->available;
        new_book->head_user = alocate_user();

        char* currenBuffer = buffer;
        char name[50];
        int numBytes = 0;
        int i = fscanf(file, "%[^\n]", buffer);

        while (strlen(currenBuffer) > 0) {
            sscanf(currenBuffer, "%49[^,], %n", name, &numBytes);
            if (strcmp(name, "-") != 0) {
                new_user = alocate_user();
                strcpy(new_user->name, name);
                add_user_sorted(new_book->head_user, new_user);
            }

            currenBuffer += numBytes;
        }
        getc(file);

        add_book_sorted(head_book, new_book);
        add_to_hash_year(year_hash, new_book);
        add_to_hash_author(author_hash, new_book);
    }


    fclose(file);
    return 0;
}

int load_users(user_position head_user) {

    FILE* file = NULL;
    file = fopen("users.txt", "r");

    if (file == NULL) {
        printf("No previous users records found\n");
        return 1;
    }

    user_position temp = alocate_user();
    user_position new_user = NULL;
    book_position new_book = NULL;
    char name[50];
    char buffer[1024];

    while (fscanf(file, "%49[^,], %d\n", temp->name, &temp->book_number) == 2) {
        new_user = alocate_user();

        strcpy(new_user->name, temp->name);
        new_user->book_number = temp->book_number;
        new_user->head_borrowed_books = alocate_book();

        int numBytes = 0;
        char* currenBuffer = buffer;
        fscanf(file, "%[^\n]", buffer);

        while (strlen(currenBuffer) > 0) {
            sscanf(currenBuffer, "%49[^,], %n", name, &numBytes);
            if (strcmp(name, "-") != 0) {
                new_book = alocate_book();
                strcpy(new_book->name, name);
                add_book_sorted(new_user->head_borrowed_books, new_book);
            }

            currenBuffer += numBytes;
        }
        getc(file);
        add_user_sorted(head_user, new_user);
    }

    fclose(file);
    return 0;
}

int add_book_sorted(book_position head_book, book_position new_book) {

    book_position temp = head_book;

    while (temp->next != NULL) {
        if (strcmp(new_book->name, temp->next->name) < 0) {
            break;
        }

        temp = temp->next;
    }

    new_book->next = temp->next;
    temp->next = new_book;

    return 0;
}

int add_user_sorted(user_position head_user, user_position new_user) {

    user_position temp = head_user;

    while (temp->next != NULL) {
        if (strcmp(new_user->name, temp->next->name) < 0) {
            break;
        }

        temp = temp->next;
    }

    new_user->next = temp->next;
    temp->next = new_user;

    return 0;
}

int add_to_hash_year(hash_position* year_hash, book_position new_book) {

    hash_position new_element = (hash_position)malloc(sizeof(Hash));

    if (new_element == NULL) {
        printf("ERROR alocating memory for hash element.\n");
        return 1;
    }

    new_element->book = new_book;

    int key = new_book->publication_year % 100;

    new_element->next = year_hash[key];
    year_hash[key] = new_element;

    return 0;
}
int add_to_hash_author(hash_position* year_hash, book_position new_book) {

    hash_position new_element = (hash_position)malloc(sizeof(Hash));

    if (new_element == NULL) {
        printf("ERROR alocating memory for hash element.\n");
        return 1;
    }

    new_element->book = new_book;

    int key = author_hash_key(new_book->author);

    new_element->next = year_hash[key];
    year_hash[key] = new_element;

    return 0;
}

int author_hash_key(char* name) {

    unsigned long key = 5831;
    int c;

    while ((c = *name++)) {
        key = ((key << 5) + key) + c;
    }

    return (int)(key % 101);
}

book_position alocate_book() {
    book_position new_book = NULL;
    new_book = (book_position)malloc(sizeof(Book));

    if (new_book == NULL) {
        printf("ERROR alocating memory for book\n");
        return NULL;
    }

    strcpy(new_book->name, "");
    strcpy(new_book->author, "");
    new_book->publication_year = 0;
    new_book->available = 0;
    new_book->head_user = NULL;
    new_book->next = NULL;

    return new_book;
}

user_position alocate_user() {
    user_position new_user = NULL;
    new_user = (user_position)malloc(sizeof(User));

    if (new_user == NULL) {
        printf("ERROR alocating memory for user\n");
        return NULL;
    }

    strcpy(new_user->name, "");
    new_user->book_number = 0;
    new_user->head_borrowed_books = NULL;
    new_user->next = NULL;

    return new_user;
}



int print_books_alphabetical(book_position head_book) {

    book_position temp = head_book->next;
    printf("---------------\n");
    while (temp != NULL) {

        printf("NASLOV: %s\n", temp->name);
        printf("AUTHOR: %s\n", temp->author);
        printf("PUBLICATION YEAR: %d\n", temp->publication_year);
        printf("AVAILABLE BOOKS: %d\n", temp->available);
        printf("USERS: ");

        user_position temp_user = temp->head_user->next;

        while (temp_user != NULL) {

            printf("%s, ", temp_user->name);

            temp_user = temp_user->next;
        }

        printf("\n---------------\n");

        temp = temp->next;
    }

    return 0;
}

int print_users_alphabetical(user_position head_user) {

    user_position temp = head_user->next;
    printf("---------------\n");
    while (temp != NULL) {

        printf("IME KORISNIKA: %s\n", temp->name);
        printf("BROJ KNJIGA: %d\n", temp->book_number);
        printf("POSUÐIVANE KNJIGE: ");

        book_position temp_book = temp->head_borrowed_books->next;

        while (temp_book != NULL) {

            printf("%s, ", temp_book->name);

            temp_book = temp_book->next;
        }

        printf("\n---------------\n");

        temp = temp->next;
    }

    return 0;
}

int find_book_year(hash_position* year_hash) {

    int year = 0;

    printf("Unesite traženu godinu:");
    scanf("%d", &year);

    hash_position temp = year_hash[year % 100];
    printf("---------------\n");
    while (temp != NULL) {
        if (temp->book->publication_year == year) {
            printf("NAZIV: %s\nBROJ KNJIGA: %d\n", temp->book->name, temp->book->available);
            printf("---------------\n");
        }

        temp = temp->next;
    }


    return 0;
}

int find_book_author(hash_position* author_hash) {

    char name[50];
    int key = 0;
    getchar();
    printf("Unesite ime autora:");
    scanf("%[^\n]", name);

    key = author_hash_key(name);

    hash_position temp = author_hash[key];
    printf("---------------\n");
    while (temp != NULL) {

        if (strcmp(temp->book->author, name) == 0) {
            printf("NAZIV: %s\nBROJ KNJIGA: %d\n", temp->book->name, temp->book->available);
            printf("---------------\n");
        }

        temp = temp->next;
    }

    return 0;
}

int add_user(user_position head_user) {

    char name[50];
    getchar();
    printf("Unesite ime novog korisnika:");
    scanf("%[^\n]", name);

    user_position new_user = NULL;
    new_user = alocate_user();
    new_user->head_borrowed_books = alocate_book();
    strcpy(new_user->name, name);

    add_user_sorted(head_user, new_user);

    return 0;
}

int borrow_book(book_position head_book, user_position head_user) {

    char user_name[50];
    char name[50];
    int quantity;
    getchar();
    printf("Unesite naziv korisnika:");
    scanf("%[^\n]", user_name);

    user_position temp_user = head_user->next;

    while (temp_user != NULL && (strcmp(temp_user->name, user_name) != 0)) {
        temp_user = temp_user->next;
    }
    if (temp_user == NULL) {
        printf("User not found\n");
        return 0;
    }

    if (temp_user->book_number >= 5) {
        printf("Dosegnut maksimalan broj knjiga.\n");
        return 0;
    }

    printf("Unesite naziv knjige:");
    getchar();
    scanf("%[^\n]", name);
    printf("Unesite željenu kolièinu:");
    scanf("%d", &quantity);

    if (temp_user->book_number + quantity > 5) {
        printf("Korisnik ima previše knjiga.\n");
        printf("želite li posuditi %d knjiga (da/ne):", 5 - temp_user->book_number);
        char response[2];
        scanf("%s", response);

        if (strcmp(response, "ne") == 0) {
            return 0;
        }
        else if (strcmp(response, "da") == 0) {
            quantity = 5 - temp_user->book_number;
        }
        else {
            printf("Invalid input\n");
            return 1;
        }
    }

    book_position temp_book = head_book->next;

    while (temp_book != NULL && (strcmp(temp_book->name, name) != 0)) {
        temp_book = temp_book->next;
    }
    if (temp_book == NULL) {
        printf("Book not found\n");
        return 0;
    }

    if (temp_book->available < quantity) {
        printf("Nema dovoljno knjiga.\n");
        if (temp_book->available == 0) {
            return 0;
        }
        printf("Želite li posuditi %d knjiga (da/ne):", temp_book->available);
        char response[2];
        scanf("%s", response);

        if (strcmp(response, "ne") == 0) {
            return 0;
        }
        else if (strcmp(response, "da") == 0) {
            quantity = temp_book->available;
        }
        else {
            printf("Invalid input");
            return 1;
        }
    }

    temp_user->book_number += quantity;
    book_position new_book = alocate_book();
    strcpy(new_book->name, temp_book->name);
    add_book_sorted(temp_user->head_borrowed_books, new_book);
    temp_book->available -= quantity;
    for (int i = 0; i < quantity; i++) {
        user_position new_user = alocate_user();
        strcpy(new_user->name, temp_user->name);
        add_user_sorted(temp_book->head_user, new_user);
    }
    return 0;
}

int return_book(book_position head_book, user_position user_head) {

    char user_name[50];
    char book_name[50];
    int quantity = 0;

    printf("Unesite ime korisnika:");
    getchar();
    scanf("%[^\n]", user_name);
    printf("Unesite naziv knjige:");
    getchar();
    scanf("%[^\n]", book_name);
    printf("Unesite kolièinu:");
    scanf("%d", &quantity);

    user_position temp_user = user_head->next;

    while (temp_user != NULL && (strcmp(temp_user->name, user_name) != 0)) {
        temp_user = temp_user->next;
    }
    if (temp_user == NULL) {
        printf("User not found\n");
        return 0;
    }

    book_position temp_book = head_book->next;

    while (temp_book != NULL && (strcmp(temp_book->name, book_name) != 0)) {
        temp_book = temp_book->next;
    }
    if (temp_book == NULL) {
        printf("Book not found\n");
        return 0;
    }

    temp_user->book_number -= quantity;
    temp_book->available += quantity;
    remove_from_list(temp_book->head_user, user_name);

    return 0;
}

int remove_from_list(user_position head_user, char* user_name) {

    user_position temp_user = head_user;
    user_position to_delete = NULL;

    while (temp_user->next != NULL && (strcmp(temp_user->next->name, user_name) != 0)) {
        temp_user = temp_user->next;
    }

    if (temp_user->next == NULL) {
        printf("User not found\n");
        return 0;
    }

    to_delete = temp_user->next;
    temp_user->next = to_delete->next;
    free(to_delete);

    return 0;
}

int save_books(book_position head_book) {

    FILE* file = NULL;
    file = fopen("library.txt", "w");

    if (file == NULL) {
        printf("ERROR opening books file\n");
        return 1;
    }

    book_position temp_book = head_book->next;

    while (temp_book != NULL) {

        fprintf(file, "%s, %s, %d, %d\n", temp_book->name, temp_book->author, temp_book->publication_year, temp_book->available);

        user_position temp_user = temp_book->head_user->next;

        if (temp_user == NULL) {
            fprintf(file, "-, \n");
        }
        while (temp_user != NULL) {
            fprintf(file, "%s, ", temp_user->name);
            temp_user = temp_user->next;
        }
        printf("\n");

        temp_book = temp_book->next;
    }

    fclose(file);
    return 0;
}
int save_users(user_position head_user) {

    FILE* file = NULL;
    file = fopen("users.txt", "w");

    if (file == NULL) {
        printf("ERROR opening users file\n");
        return 1;
    }

    user_position temp_user = head_user->next;

    while (temp_user != NULL) {

        fprintf(file, "%s, %d\n", temp_user->name, temp_user->book_number);

        book_position temp_book = temp_user->head_borrowed_books->next;

        if (temp_book == NULL) {
            fprintf(file, "-, \n");
        }
        while (temp_book != NULL) {
            fprintf(file, "%s, ", temp_book->name);

            temp_book = temp_book->next;
        }
        printf("\n");

        temp_user = temp_user->next;
    }

    return 0;
}

