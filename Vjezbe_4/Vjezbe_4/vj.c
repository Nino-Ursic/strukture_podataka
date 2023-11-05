/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/*
4. Napisati program za zbrajanje i mno�enje polinoma. Koeficijenti i eksponenti se
�itaju iz datoteke.
Napomena: Eksponenti u datoteci nisu nu�no sortirani.
*/
typedef struct pol polinom;
typedef polinom* Position;
struct pol {
    int coefficient;
    int exponent;
    Position next;
};

void read_from_file(Position head1, Position head2);
void string_into_list(char* buffer, Position head);
void add_sorted(Position head, int coef, int expo);
void print_list(Position head1);
void add_polinoms(Position head1, Position head2, Position adding_head);
void multiply_polinoms(Position head1, Position head2, Position multiply_head);


int main() {
    polinom head1 = { .coefficient = 0, .exponent = 0, .next = NULL };
    polinom head2 = { .coefficient = 0, .exponent = 0, .next = NULL };
    polinom adding_head = { .coefficient = 0, .exponent = 0, .next = NULL };
    polinom multiply_head = { .coefficient = 0, .exponent = 0, .next = NULL };

    read_from_file(&head1, &head2);

    printf("First polinom:");
    print_list(&head1);

    printf("\nSecond polinom:");
    print_list(&head2);

    printf("\nAdded polinoms:");
    add_polinoms(&head1, &head2, &adding_head);
    print_list(&adding_head);

    printf("\nMultiplied polinoms:");
    multiply_polinoms(&head1, &head2, &multiply_head);
    print_list(&multiply_head);

    return 0;
}



void read_from_file(Position head1, Position head2) {
    FILE* file_pointer = NULL;
    file_pointer = fopen("polinoms.txt", "r");

    char buffer[1024] = { 0 };

    fgets(buffer, 1024, file_pointer);
    string_into_list(buffer, head1);

    fgets(buffer, 1024, file_pointer);
    string_into_list(buffer, head2);


}

void string_into_list(char* buffer, Position head) {
    char* currentBuffer = buffer;
    int numBytes, coef, expo;

    while (strlen(currentBuffer) > 0) {//
        sscanf(currentBuffer, "%dx^%d %n", &coef, &expo, &numBytes);
        add_sorted(head, coef, expo);
        currentBuffer += numBytes;
    }
}

void add_sorted(Position head, int coef, int expo) {
    Position temp = head->next;
    Position previous = head;
    Position new_element = (Position)malloc(sizeof(polinom));

    new_element->coefficient = coef;
    new_element->exponent = expo;

    while (temp != NULL) {
        if (new_element->exponent > temp->exponent) {
            new_element->next = previous->next;
            previous->next = new_element;
            break;
        }
        else if (new_element->exponent == temp->exponent) {
            temp->coefficient = temp->coefficient + new_element->coefficient;
            free(new_element);
            break;
        }
        temp = temp->next;
        previous = previous->next;
    }
    if (temp == NULL) {
        new_element->next = previous->next;
        previous->next = new_element;
    }


}

void print_list(Position head) {
    Position temp = head;

    while (temp->next != NULL) {
        temp = temp->next;
        printf("%dx^%d ", temp->coefficient, temp->exponent);
    }
}
void add_polinoms(Position head1, Position head2, Position adding_head) {
    Position temp;
    Position p1 = head1->next;
    Position p2 = head2->next;

    while (p1 != NULL && p2 != NULL) {
        if (p1->exponent == p2->exponent) {
            add_sorted(adding_head, p1->coefficient + p2->coefficient, p1->exponent);
            p1 = p1->next;
            p2 = p2->next;
        }
        else if (p1->exponent > p2->exponent) {
            add_sorted(adding_head, p1->coefficient, p1->exponent);
            p1 = p1->next;
        }
        else {
            add_sorted(adding_head, p2->coefficient, p2->exponent);
            p2 = p2->next;
        }
    }

    if (p1 != NULL) {
        temp = p1;
    }
    else {
        temp = p2;
    }
    while (temp != NULL) {
        add_sorted(adding_head, temp->coefficient, temp->exponent);
        temp = temp->next;
    }

}

void multiply_polinoms(Position head1, Position head2, Position multiply_head) {
    Position p1, p2;
    for (p1 = head1->next; p1 != NULL; p1 = p1->next) {
        for (p2 = head2->next; p2 != NULL; p2 = p2->next) {
            add_sorted(multiply_head, p1->coefficient * p2->coefficient, p1->exponent + p2->exponent);
        }
    }
}




