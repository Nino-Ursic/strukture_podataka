#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/*
5. Napisati program koji iz datoteke èita postfiks izraz i zatim korištenjem stoga raèuna
rezultat. Stog je potrebno realizirati preko vezane liste.
*/
typedef struct element Element;
typedef Element* Position;
struct element {
    int number;
    Position next;
};

int calculateFromPostfix(char* fileName, Position head, double* result, char* buffer);
int readPostfixFromFile(char* fileName, char* buffer);
int pushElement(Position head, int n);
int calculate(Position head, char operation, double* result);
int pop(Position head, int* number);

int main() {

    Element head = { .number = 0, .next = NULL };
    double result = 0.0;
    char buffer[1024];

    if (calculateFromPostfix("postfix.txt", &head, &result, buffer)) {
        printf("Unable to calculate postfix result");
        return 1;
    }

    printf("Result is: %lf", result);

    return 0;
}

int calculateFromPostfix(char* fileName, Position head, double* result, char* buffer) {

    char operation;
    int status=0, numBytes=0, n=0;
    readPostfixFromFile(fileName, buffer);

    while (strlen(buffer) > 0) {
        if (status = sscanf(buffer, " %d %n", &n, &numBytes) == 1) {
            if (pushElement(head, n)) {
                return 1;
            }
            buffer += numBytes;
        }
        else if (status = sscanf(buffer, " %c %n", &operation, &numBytes) == 1) {
            if (calculate(head, operation, result)) {
                return 1;
            }
            buffer += numBytes;
        }
        else {
            printf("Unable to read postfix");
            return 1;
        }

    }

    return 0;
}

int readPostfixFromFile(char* fileName, char* buffer) {

    FILE* filePointer = NULL;
    filePointer = fopen(fileName, "r");

    fgets(buffer, 1024, filePointer);

    fclose(filePointer);

    return 0;
}

int pushElement(Position head, int n) {

    Position newElement = (Position)malloc(sizeof(Element));
    if (newElement == NULL) {
        printf("Unable to alocate memory");
        return 1;
    }
    newElement->number = n;
    newElement->next = head->next;
    head->next = newElement;

    return 0;
}

int calculate(Position head, char operation, double* result) {
    int number1=0, number2=0;

    if (pop(head, &number1) || pop(head, &number2) == 1) {
        return 1;
    }

    switch (operation) {
    case '+':
        *result = number2 + number1;
        break;
    case '-':
        *result = number2 - number1;
        break;
    case '*':
        *result = number2 * number1;
        break;
    case '/':
        if (number1 == 0) {
            printf("can't divide by 0");
            return 1;
        }
        *result = number2 / number1;
        break;
    default:
        printf("Operation %c not supported", operation);
        return 1;
    }

    if (pushElement(head, *result)) {
        return 1;
    }

    return 0;
}

int pop(Position head, int* number) {
    if (head->next == NULL) {
        printf("list is empty");
        return 1;
    }
    Position temp = head->next;
    *number = temp->number;

    head->next = temp->next;
    free(temp);

    return 0;
}







