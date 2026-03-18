#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHUNK 15

// ------------------ ROPE DATA STRUCTURE ---------------------

typedef struct Rope {
    char str[CHUNK + 1];
    int weight;
    struct Rope *left, *right;
} Rope;

Rope* createLeaf(char* s) {
    Rope* node = (Rope*)malloc(sizeof(Rope));
    strncpy(node->str, s, CHUNK);
    node->str[CHUNK] = '\0';
    node->left = node->right = NULL;
    node->weight = strlen(node->str);
    return node;
}

Rope* buildRope(char* s) {
    int len = strlen(s);
    if (len <= CHUNK)
        return createLeaf(s);

    int mid = len / 2;
    char leftStr[100], rightStr[100];

    strncpy(leftStr, s, mid);
    leftStr[mid] = '\0';
    strcpy(rightStr, s + mid);

    Rope* node = (Rope*)malloc(sizeof(Rope));
    node->left = buildRope(leftStr);
    node->right = buildRope(rightStr);
    node->weight = mid;
    node->str[0] = '\0';

    return node;
}

void ropeToString(Rope* root, char* out) {
    if (!root) return;

    ropeToString(root->left, out);

    if (root->left == NULL && root->right == NULL)
        strcat(out, root->str);

    ropeToString(root->right, out);
}

// ------------------ BOOK STRUCTURE ----------------------

typedef struct {
    Rope* title;
    Rope* borrower;
    char block[10];
    int available;
    char returnDate[20];
} Book;

Book books[100];
int countBooks = 0;

// ------------------ ADMIN FUNCTIONS ----------------------

void adminAddBook() {
    char title[100], borrower[100];
    getchar();
    printf("\nEnter book title: ");
    gets(title);

    printf("Enter block where book is placed (A1/B2/C3 etc): ");
    gets(books[countBooks].block);

    printf("Is book available? (1 Yes / 0 No): ");
    scanf("%d", &books[countBooks].available);
    getchar();

    books[countBooks].title = buildRope(title);

    if (books[countBooks].available == 0) {
        printf("Enter name of borrower: ");
        gets(borrower);
        books[countBooks].borrower = buildRope(borrower);
        strcpy(books[countBooks].returnDate, "After 15 days");
    } else {
        books[countBooks].borrower = buildRope("-");
        strcpy(books[countBooks].returnDate, "-");
    }

    countBooks++;
    printf("\n✔ Book added successfully!\n");
}

// ------------------ STUDENT FUNCTIONS ----------------------

void studentSearchBook() {
    char query[100];
    getchar();
    printf("\nEnter book title to search: ");
    gets(query);

    for (int i = 0; i < countBooks; i++) {
        char t[200] = "";
        ropeToString(books[i].title, t);

        if (strcmp(t, query) == 0) {
            printf("\n📘 Book found!\n");
            printf("Title: %s\n", t);

            if (books[i].available) {
                printf("Status: ✔ Available\n");
                printf("Located at Block: %s\n", books[i].block);
            } else {
                char b[200] = "";
                ropeToString(books[i].borrower, b);
                printf("Status: ❌ Not Available\n");
                printf("Borrowed by: %s\n", b);
                printf("Return in: %s\n", books[i].returnDate);
            }
            return;
        }
    }
    printf("\n❌ Book not found\n");
}

// ------------------ MAIN PROGRAM ----------------------

int main() {
    int userType;

    while (1) {
        printf("\n====== WHO ARE YOU? ======\n");
        printf("1. Admin\n");
        printf("2. Student\n");
        printf("3. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &userType);

        if (userType == 1) {
            int ch;
            while (1) {
                printf("\n====== ADMIN MENU ======\n");
                printf("1. Add Book\n");
                printf("2. Back\n");
                printf("Enter choice: ");
                scanf("%d", &ch);

                if (ch == 1) adminAddBook();
                else break;
            }
        }

        else if (userType == 2) {
            int ch;
            while (1) {
                printf("\n====== STUDENT MENU ======\n");
                printf("1. Search Book\n");
                printf("2. Back\n");
                printf("Enter choice: ");
                scanf("%d", &ch);

                if (ch == 1) studentSearchBook();
                else break;
            }
        }

        else if (userType == 3) {
            printf("\nThank you!\n");
            break;
        }

        else printf("\nInvalid choice!\n");
    }

    return 0;
}
