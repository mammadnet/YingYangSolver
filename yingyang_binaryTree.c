// binary tree slotion

#define RED  "\x1B[31m"
#define GRN  "\x1B[32m"
#define YEL  "\x1B[33m"
#define BLU  "\x1B[34m"
#define MAG  "\x1B[35m"
#define CYN  "\x1B[36m"
#define WHT  "\x1B[37m"
#define RESET "\x1B[0m"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define ROWS 4
#define COLS 4

typedef struct Node {
    unsigned short board;
    unsigned short status;
    unsigned short blacks;
    unsigned short whites;
    struct Node* next;
} Node;

void insert(Node** head, unsigned short board, unsigned short status, unsigned short blacks, unsigned short whites) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->board = board;
    newNode->status = status;
    newNode->blacks = blacks;
    newNode->whites = whites;


    newNode->next = *head;
    *head = newNode;
}

bool coord(unsigned short board, short i, short j) {
    short index = i * COLS + j;
    short bIndex = 1 << index;
    bool res = board & bIndex;
    return res;
}

bool check2x2(unsigned short board, unsigned short status) {

    char endI = (status / ROWS);
    char endJ = status % (ROWS);
    char tempJ = endJ;

    for (char i = 0; i < endI; i++) {

        if (i == endI - 1) tempJ = endJ;

        for (char j = 0; j < tempJ; j++) {
            char res = coord(board, i, j) + coord(board, i + 1, j) + coord(board, i, j + 1) + coord(board, i + 1, j + 1);
            if (res == 4 || res == 0) {
                return false;
            }
        }
    }
    return true;
}

bool isEmpty(short i, short j, unsigned short status) {
    short index = i * COLS + j;
    return index <= status;
}

unsigned short changeBit(unsigned short number, short i, short j, short bit) {
    short index = i * COLS + j;
    return (number & ~(1 << index)) | (bit << index);

}

bool validate(short i, short j) {
    short index = i * COLS + j;
    return (i < 4 && j < 4 && i >= 0 && j >= 0);
}

short countContinuCell(unsigned short board, short i, short j, short status, unsigned short visited) {

    bool color = coord(board, i, j);
    visited = changeBit(visited, i, j, 1);

    short count = 0;

    if (validate(i, j) && isEmpty(i, j, status)) {
        count = 1;
    }



    short adjI = i - 1;
    short adjJ = j;
    if (validate(adjI, adjJ) && isEmpty(adjI, adjJ, status) && coord(visited, adjI, adjJ) == 0 && coord(board, adjI, adjJ) == color) {     // top

        count += countContinuCell(board, adjI, adjJ, status, visited);



    }

    adjI = i + 1;
    adjJ = j;
    if (validate(adjI, adjJ) && isEmpty(adjI, adjJ, status) && coord(visited, adjI, adjJ) == 0 && coord(board, adjI, adjJ) == color) {     // down


        count += countContinuCell(board, adjI, adjJ, status, visited);


    }

    adjI = i;
    adjJ = j - 1;
    if (validate(adjI, adjJ) && isEmpty(adjI, adjJ, status) && coord(visited, adjI, adjJ) == 0 && coord(board, adjI, adjJ) == color) {     // left


        count += countContinuCell(board, adjI, adjJ, status, visited);


    }

    adjI = i;
    adjJ = j + 1;
    if (validate(adjI, adjJ) && isEmpty(adjI, adjJ, status) && coord(visited, adjI, adjJ) == 0 && coord(board, adjI, adjJ) == color) {     // right


        count += countContinuCell(board, adjI, adjJ, status, visited);


    }

    return count;
}

bool Continuity(unsigned short board, unsigned short status, short whites, short blacks) {


    short i = 0;
    short j = 0;
    bool color = coord(board, i, j);

    short numberOfCell = countContinuCell(board, i, j, status, 0);



    // Find the coordinates of another color

    if (color == 0) {
        if (numberOfCell != whites)
            return false;
    }
    else {
        if (numberOfCell != blacks)
            return false;
    }

    short adjI[3] = { 1, 0, 1 };
    short adjJ[3] = { 0, 1, 1 };

    for (int x = 0; x < 3; x++) {
        if (coord(board, i + adjI[x], j + adjJ[x]) == !color) {
            i = i + adjI[x];
            j = j + adjJ[x];
            break;
        }
    }



    numberOfCell = countContinuCell(board, i, j, status, 0);

    color = coord(board, i, j);

    if (color == 0) {
        if (numberOfCell != whites)
            return false;
    }
    else {
        if (numberOfCell != blacks)
            return false;
    }

    return true;


}

// Give a head of a linkList to store each board that generated
void generate(Node** head, unsigned short board, unsigned short status, unsigned short blacks, unsigned short whites) {

    if (status == 16) {
        insert(head, board, status, blacks, whites);

        return;
    }

    unsigned short blackCell = board | (1 << status);
    if (check2x2(blackCell, status)) {
        generate(head, blackCell, status + 1, blacks + 1, whites);
    }



    unsigned short whiteCell = board;
    if (check2x2(whiteCell, status)) {
        generate(head, whiteCell, status + 1, blacks, whites + 1);
    }

}

void showList(Node* head) {
    Node* p = head;
    int count = 0;

    while (p != NULL) {
        if (Continuity(p->board, p->status, p->whites, p->blacks)) {

            count++;
        }

        p = p->next;
    }
    printf("\n%d\n", count);
}

void showBoard(Node* head, unsigned short num) {

    Node* p = head;
    short count = 0;


    while (p != NULL) {

        if (Continuity(p->board, 16, p->whites, p->blacks)) {
            count++;
            printf(YEL "-------------------------\n" RESET);

            printf(GRN "status is: %d board is: %d, black: %d, white:%d\n" RESET, p->status, p->board, p->blacks, p->whites);
            for (short i = 0; i < ROWS; i++) {
                for (short j = 0; j < COLS; j++) {
                    bool cell = coord(p->board, i, j);
                    if (cell)
                        printf(RED "#" RESET);
                    else
                        printf(CYN "O" RESET);
                }
                printf("\n");
            }
        }

        p = p->next;
    }
    printf(MAG "Number of correct boards: %d\n" RESET, count);
}

int main( ) {
    Node* head = NULL;
    clock_t begin = clock();
    generate(&head, 0, 0, 0, 0);

    showBoard(head, 0);
    clock_t end = clock();
    double timeSpend = (double)(end-begin) / CLOCKS_PER_SEC;
    printf(GRN "Process Time-> %.5f Sec\n" RESET, timeSpend);

}
