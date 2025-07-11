// BFS generation solution


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

#define ROWS 4
#define COLS 4

typedef struct Node {
    unsigned short board;
    unsigned short status;
    unsigned short blacks;
    unsigned short whites;
    struct Node* next;
} Node;

typedef struct Queue
{
    Node* front;
    Node* rear;
} Queue;



Node* createNode(unsigned short board, unsigned short status, unsigned short whites, unsigned short blacks) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->board = board;
    newNode->status = status;
    newNode->blacks = blacks;
    newNode->whites = whites;
    newNode->next = NULL;
    return newNode;
}


void insert(Node** head, unsigned short board, unsigned short status, unsigned short whites, unsigned short blacks) {
    Node* newNode = createNode(board, status, whites, blacks);


    newNode->next = *head;
    *head = newNode;
}


// For queue data structure

Queue* CreatQueue( ) {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->front = queue->rear = NULL;
    return queue;
}

void enqueue(Queue* q, Node* newNode) {
    if (q->rear == NULL) {
        q->rear = q->front = newNode;
        return;
    }
    q->rear->next = newNode;
    q->rear = newNode;
}

void dequeue(Queue* q) {
    if (q->front == NULL) return;
    Node* temp = q->front;
    q->front = q->front->next;
    if (q->front == NULL)
        q->rear = NULL;

    free(temp);


}

Node* top(Queue* q) {
    if (q->front == NULL)
        return NULL;
    return q->front;
}

///////////////////////////////////////

bool coord(unsigned short board, short i, short j) {
    short index = i * COLS + j;
    short bIndex = 1 << index;
    bool res = board & bIndex;
    return res;
}

bool isEmpty(short i, short j, unsigned short status) {
    return !coord(status, i, j);
}

bool check2x2(unsigned short board, unsigned short status) {

    for (char i = 0; i < COLS - 1; i++) {

        for (char j = 0; j < ROWS - 1; j++) {
            if (!(isEmpty(i, j, status) || isEmpty(i + 1, j, status) || isEmpty(i, j + 1, status) || isEmpty(i + 1, j + 1, status))) {

                char res = coord(board, i, j) + coord(board, i + 1, j) + coord(board, i, j + 1) + coord(board, i + 1, j + 1);
                if (res == 4 || res == 0) {
                    return false;
                }
            }
        }
    }
    return true;
}


unsigned short changeBit(unsigned short number, short i, short j, short bit) {
    short index = i * COLS + j;


    return (number & ~(1 << index)) | (bit << index);

}

bool validCoord(short i, short j) {
    short index = i * COLS + j;
    return (i < 4 && j < 4 && i >= 0 && j >= 0);
}

bool validForNext(unsigned short board, bool color, short adjI, short adjJ, short status, short visited) {
    return (validCoord(adjI, adjJ) &&
        !isEmpty(adjI, adjJ, status) &&
        coord(visited, adjI, adjJ) == 0 &&
        coord(board, adjI, adjJ) == color);
}

bool checkCountedCell(bool color, short counted, short allWhites, short allBlackes) {
    if (color == 1) {
        if (counted == allBlackes)
            return true;
    }
    else
        if (counted == allWhites)
            return true;
    return false;

}

short countContinuCell(unsigned short board, short i, short j, short status, unsigned short visited) {

    bool color = coord(board, i, j);
    visited = changeBit(visited, i, j, 1);

    short count = 0;

    if (validCoord(i, j) && !isEmpty(i, j, status)) {     // top
        count = 1;
    }

    short adjI = i - 1;
    short adjJ = j;
    if (validForNext(board, color, adjI, adjJ, status, visited)) {
        count += countContinuCell(board, adjI, adjJ, status, visited);
    }

    adjI = i + 1;
    adjJ = j;
    if (validForNext(board, color, adjI, adjJ, status, visited)) {

        count += countContinuCell(board, adjI, adjJ, status, visited);

    }

    adjI = i;
    adjJ = j - 1;
    if (validForNext(board, color, adjI, adjJ, status, visited)) {

        count += countContinuCell(board, adjI, adjJ, status, visited);

    }

    adjI = i;
    adjJ = j + 1;
    if (validForNext(board, color, adjI, adjJ, status, visited)) {

        count += countContinuCell(board, adjI, adjJ, status, visited);
    }

    return count;
}

bool Continuity(unsigned short board, unsigned short status, short whites, short blacks) {


    short firstI = 0;
    short firstJ = 0;
    bool color;

    short i = 0, j = 0;


    // Find the first filled cell
    bool condition = false;
    for (i; i < ROWS; i++) {
        for (j = 0; j < COLS; j++) {
            if (!isEmpty(i, j, status)) {
                color = coord(board, i, j);
                condition = true;
                break;
            }
        }
        if (condition) break;
    }

    short numberOfCell = countContinuCell(board, i, j, status, 0);


    if (!checkCountedCell(color, numberOfCell, whites, blacks)) {
        return false;
    }

    color = !color;
    condition = false;
    for (i; i < ROWS; i++) {
        for (j = 0; j < COLS; j++) {
            if (!isEmpty(i, j, status) && coord(board, i, j) == color) {
                condition = true;
                break;
            }
        }
        if (condition) break;
    }


    numberOfCell = countContinuCell(board, i, j, status, 0);

    if (!checkCountedCell(color, numberOfCell, whites, blacks))
        return false;

    return true;

}

void showList(Node* head) {
    Node* p = head;
    int count = 0;

    while (p != NULL) {
        if (Continuity(p->board, p->status, p->whites, p->blacks)) {
            printf("status is: %d board is: %d, black: %d, white:%d\n", p->status, p->board, p->blacks, p->whites);

        }
        count++;

        p = p->next;
    }
    printf("\n%d\n", count);
}

void showBoard(short board, short status) {

    for (short i = 0; i < ROWS; i++) {
        for (short j = 0; j < COLS; j++) {
            if (coord(status, i, j) == 1) {

                bool cell = coord(board, i, j);
                if (cell)
                    printf(RED "#", CYN);
                else
                    printf(CYN "O", RESET);
            }
            else {
                printf(WHT "+" RESET);
            }
        }
        printf("\n");
    }

}

void showBoards(Node* head, unsigned short num) {

    Node* p = head;
    short count = 0;


    while (p != NULL) {
        printf("-------------------------\n");
        printf("status is: %d board is: %d, black: %d, white:%d\n", p->status, p->board, p->blacks, p->whites);
        showBoard(p->board, p->status);

        p = p->next;
    }
}

// Give a head of a linkList to store each board that generated
int count = 0;
int temp = 0;
void generate(Node** head) {
    temp++;


    unsigned short newStatus, blackCell, whiteCell;
    Queue* queue = CreatQueue( );

    // root node
    Node* node = createNode(0, 0, 0, 0);
    enqueue(queue, node);

    while (node != NULL) {
        node = top(queue);
        if (node == NULL) break;
        for (short x = 0; x < ROWS; x++) {
            for (short y = 0; y < COLS; y++) {
                if (coord(node->status, x, y) == 0) {
                    newStatus = changeBit(node->status, x, y, 1);
                    blackCell = changeBit(node->board, x, y, 1);
                    if (check2x2(blackCell, newStatus) && Continuity(blackCell, newStatus, node->whites, node->blacks + 1)) {
                        if (newStatus == (1 << 8)-2) {
                            insert(head, blackCell, newStatus, node->whites, node->blacks + 1);
                            printf("----------\n");
                            printf("board %d, status %d\n", blackCell, newStatus);
                            showBoard(blackCell, newStatus);
                        }
                        else
                            enqueue(queue, createNode(blackCell, newStatus, node->whites, node->blacks + 1));
                    }
                    whiteCell = node->board;
                    if (check2x2(whiteCell, newStatus) && Continuity(whiteCell, newStatus, node->whites + 1, node->blacks)) {
                        if (newStatus == (1 << 8)-2) {
                            insert(head, whiteCell, newStatus, node->blacks, node->whites + 1);
                        }

                        else
                            enqueue(queue, createNode(whiteCell, newStatus, node->whites + 1, node->blacks));
                    }
                }
            }
        }
        dequeue(queue);
    }
}



int main( ) {
    Node* head = NULL;
    generate(&head);
}
