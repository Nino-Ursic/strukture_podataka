#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
/*
8. Napisati program koji omoguæava rad s binarnim stablom pretraživanja. Treba
omoguæiti unošenje novog elementa u stablo, ispis elemenata (inorder, preorder, postorder i
level order), brisanje i pronalaženje nekog elementa.
*/

typedef struct cvor Cvor;
typedef Cvor* position;
struct cvor {
    int element;
    position L;
    position R;
};

position addCvor(position root, int newElement);
int print_inorder(position root);
int print_preorder(position root);
int print_postorder(position root);
int print_levelOrder(position root);
position delete_element(position root, int element);
position minCvor(position root);
position find_element(position root, int element);


int main()
{

    position rootCvor = NULL;
    rootCvor = malloc(sizeof(Cvor));

    if (rootCvor == NULL) {
        printf("ERROR alocating memory for root cvor!\n");
        return 1;
    }

    rootCvor->L = NULL;
    rootCvor->R = NULL;
    printf("Unesite element root cvora:");
    scanf("%d", &rootCvor->element);


    while (1) {
        printf("\nMenu:\n");
        printf("1 - Unos novog elementa u stablo\n");
        printf("2 - Ispis elemenata stabla\n");
        printf("3 - Brisanje elementa\n");
        printf("4 - Pronalaženje elementa\n");
        printf("5 - exit\n");

        int choice = 0;

        printf("Unesite broj opcije:");
        scanf("%d", &choice);

        if (choice == 1) {

            int newElement = 0;
            printf("Unesite novi element:");
            scanf("%d", &newElement);

            addCvor(rootCvor, newElement);

        }
        else if (choice == 2) {

            printf("\nPrint menu:\n");
            printf("1 - inorder\n");
            printf("2 - preorder\n");
            printf("3 - postorder\n");
            printf("4 - level order\n");

            int choice = 0;
            printf("Izaberite tip ispisa:");
            scanf("%d", &choice);

            if (choice == 1) {

                print_inorder(rootCvor);

            }
            else if (choice == 2) {

                print_preorder(rootCvor);

            }
            else if (choice == 3) {

                print_postorder(rootCvor);

            }
            else if (choice == 4) {

                print_levelOrder(rootCvor);

            }
            else {
                printf("ERROR invalid choice!");
            }



        }
        else if (choice == 3) {

            int element = 0;
            printf("Unesite element koji želite izbrisati:");
            scanf("%d", &element);

            delete_element(rootCvor, element);

        }
        else if (choice == 4) {

            int element = 0;
            printf("Unesite element koji želite pronaæi:");
            scanf("%d", &element);

            find_element(rootCvor, element);

        }
        else if (choice == 5) {
            printf("\nExiting program\n");
            break;

        }

    }



    return 0;
}

position addCvor(position root, int newElement) {

    if (root == NULL) {

        root = (position)malloc(sizeof(Cvor));

        if (root == NULL) {
            printf("ERROR alocating memory for new element!");
            return NULL;
        }

        root->element = newElement;
        root->L = NULL;
        root->R = NULL;

    }
    else if (newElement > root->element) {

        root->R = addCvor(root->R, newElement);

    }
    else if (newElement < root->element) {

        root->L = addCvor(root->L, newElement);

    }

    return root;
}

int print_inorder(position root) {

    if (root == NULL) {
        return 0;
    }
    print_inorder(root->L);
    printf("%d, ", root->element);
    print_inorder(root->R);

    return 0;
}

int print_preorder(position root) {

    if (root == NULL) {
        return 0;
    }
    printf("%d, ", root->element);
    print_preorder(root->L);
    print_preorder(root->R);

    return 0;
}

int print_postorder(position root) {

    if (root == NULL) {
        return 0;
    }
    print_postorder(root->L);
    print_postorder(root->R);
    printf("%d, ", root->element);

    return 0;
}

int print_levelOrder(position root) {

    if (root == NULL) {
        printf("Tree is empty");
        return 1;
    }

    position queue[100];
    int front = 0, rear = 0;

    queue[rear++] = root;

    while (front < rear) {
        position current = queue[front++];

        printf("%d ", current->element);

        if (current->L != NULL)
            queue[rear++] = current->L;

        if (current->R != NULL)
            queue[rear++] = current->R;
    }
    return 0;
}


position delete_element(position root, int element) {

    if (root == NULL) {
        printf("element not found");
        return NULL;
    }

    if (element < root->element) {

        root->L = delete_element(root->L, element);

    }
    else if (element > root->element) {

        root->R = delete_element(root->R, element);

    }
    else if (root->L != NULL && root->R != NULL) {

        position temp = minCvor(root->R);

        root->element = temp->element;

        delete_element(root->R, root->element);

    }
    else {
        if (root->L == NULL) {

            position to_delete = root;
            root = root->R;

            free(to_delete);

        }
        else {

            position to_delete = root;
            root = root->L;

            free(to_delete);

        }
    }


    return root;
}

position minCvor(position root) {

    while (root->L != NULL) {
        root = root->L;
    }

    return root;
}

position find_element(position root, int element) {

    while (1) {
        if (root == NULL) {
            printf("Element not found");
            return NULL;
        }
        else if (element < root->element) {
            root = root->L;
        }
        else if (element > root->element) {
            root = root->R;
        }
        else {

            printf("Element %d found.", root->element);
            return root;
        }
    }

    return NULL;
}