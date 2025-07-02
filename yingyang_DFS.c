// each node has less than 32 child

#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define WHT "\x1B[37m"
#define RESET "\x1B[0m"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ROWS 4
#define COLS 4

typedef struct Node
{
    unsigned short board;
    unsigned short status;
    unsigned short blacks;
    unsigned short whites;
    struct Node *next;
} Node;

void insert(Node **head, unsigned short board, unsigned short status, unsigned short blacks, unsigned short whites)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->board = board;
    newNode->status = status;
    newNode->blacks = blacks;
    newNode->whites = whites;

    newNode->next = *head;
    *head = newNode;
}

bool coord(unsigned short board, short i, short j)
{
    short index = i * COLS + j;
    short bIndex = 1 << index;
    bool res = board & bIndex;
    return res;
}

bool isEmpty(short i, short j, unsigned short status)
{
    return !coord(status, i, j);
}

bool check2x2(unsigned short board, unsigned short status)
{

    for (char i = 0; i < COLS - 1; i++)
    {

        for (char j = 0; j < ROWS - 1; j++)
        {
            // printf("i %d, j %d\n", i, j);
            if (!(isEmpty(i, j, status) || isEmpty(i + 1, j, status) || isEmpty(i, j + 1, status) || isEmpty(i + 1, j + 1, status)))
            {

                char res = coord(board, i, j) + coord(board, i + 1, j) + coord(board, i, j + 1) + coord(board, i + 1, j + 1);
                if (res == 4 || res == 0)
                {
                    // printf("false -> i %d, j %d, %d\n", i, j, res);

                    // if(board == 1)
                    //     printf("-in check2x2->%d, %d\n", board, status);
                    return false;
                }
            }
        }
        // printf("COLS-> %d, endj-> %d", endI, endJ);
    }
    return true;
}

unsigned short changeBit(unsigned short number, short i, short j, short bit)
{
    short index = i * COLS + j;

    return (number & ~(1 << index)) | (bit << index);
}

bool validCoord(short i, short j)
{
    short index = i * COLS + j;
    // printf("in validCoord i %d, j %d\n", i, j);
    return (i < 4 && j < 4 && i >= 0 && j >= 0);
}

bool validForNext(unsigned short board, bool color, short adjI, short adjJ, short status, short visited)
{
    return (validCoord(adjI, adjJ) &&
            !isEmpty(adjI, adjJ, status) &&
            coord(visited, adjI, adjJ) == 0 &&
            coord(board, adjI, adjJ) == color);
}

bool checkCountedCell(bool color, short counted, short allWhites, short allBlackes)
{
    if (color == 1)
    {
        if (counted == allBlackes)
            return true;
    }
    else if (counted == allWhites)
        return true;
    return false;
}

short countContinuCell(unsigned short board, short i, short j, short status, unsigned short visited)
{

    bool color = coord(board, i, j);
    visited = changeBit(visited, i, j, 1);

    short count = 0;

    if (validCoord(i, j) && !isEmpty(i, j, status))
    { // top
        count = 1;
    }

    // printf("color %d, i-> %d, j-> %d\n", color, i, j);

    short adjI = i - 1;
    short adjJ = j;
    // printf("top\n");
    if (validForNext(board, color, adjI, adjJ, status, visited))
    {
        count += countContinuCell(board, adjI, adjJ, status, visited);
        // printf("topCLOSE\n");
    }

    adjI = i + 1;
    adjJ = j;
    // printf("down\n");
    if (validForNext(board, color, adjI, adjJ, status, visited))
    {

        count += countContinuCell(board, adjI, adjJ, status, visited);
        // printf("downCLOSE\n");
    }

    adjI = i;
    adjJ = j - 1;
    // printf("left\n");
    if (validForNext(board, color, adjI, adjJ, status, visited))
    {

        count += countContinuCell(board, adjI, adjJ, status, visited);
        // printf("leftCLOSE\n");
    }

    adjI = i;
    adjJ = j + 1;
    // printf("right\n");
    if (validForNext(board, color, adjI, adjJ, status, visited))
    {

        count += countContinuCell(board, adjI, adjJ, status, visited);
        // printf("rightCLOSE\n");
    }

    return count;
}

bool Continuity(unsigned short board, unsigned short status, short whites, short blacks)
{

    short firstI = 0;
    short firstJ = 0;
    bool color;

    short i = 0, j = 0;

    // Find the first filled cell
    bool condition = false;
    for (i; i < ROWS; i++)
    {
        for (j = 0; j < COLS; j++)
        {
            if (!isEmpty(i, j, status))
            {
                color = coord(board, i, j);
                condition = true;
                break;
            }
        }
        if (condition)
            break;
    }

    short numberOfCell = countContinuCell(board, i, j, status, 0);
    // printf("1-->board %d, color %d, numberof %d, white %d, blacks %d, i %d, j %d\n", board, color, numberOfCell, whites, blacks, i, j);

    if (!checkCountedCell(color, numberOfCell, whites, blacks))
        return false;

    color = !color;
    condition = false;
    for (i; i < ROWS; i++)
    {
        for (j = 0; j < COLS; j++)
        {
            if (!isEmpty(i, j, status) && coord(board, i, j) == color)
            {
                condition = true;
                break;
            }
        }
        if (condition)
            break;
    }

    numberOfCell = countContinuCell(board, i, j, status, 0);

    // printf("2-->board %d, color %d, numberof %d, white %d, blacks %d, i %d, j %d\n", board, color, numberOfCell, whites, blacks, i, j);

    //     printf("-in check2x2->%d, %d\n", board, status);

    // printf("color %d, numberof %d, white %d, blacks %d, i %d, j %d\n", color, numberOfCell, whites, blacks, i, j);

    if (!checkCountedCell(color, numberOfCell, whites, blacks))
        return false;

    return true;
}

// Give a head of a linkList to store each board that generated
int count = 0;
int temp = 0;
void generate(Node **head, unsigned short board, unsigned short status, unsigned short blacks, unsigned short whites)
{
    temp++;
    if (status == (1 << 16) - 1)
    { // 16 ^ 2 -1
        insert(head, board, status, blacks, whites);
        printf("\n");

        showBoard(board);

        // printf("%d, -------%d\n", ++count, temp);

        return;
    }

    unsigned short newStatus, blackCell, whiteCell;

    for (short x = 0; x < ROWS; x++)
    {
        for (short y = 0; y < COLS; y++)
        {

            if (coord(status, x, y) == 0)
            {

                newStatus = changeBit(status, x, y, 1);

                whiteCell = board;
                if (check2x2(whiteCell, newStatus) && Continuity(whiteCell, newStatus, whites + 1, blacks))
                {
                    generate(head, whiteCell, newStatus, blacks, whites + 1);
                }
                blackCell = changeBit(board, x, y, 1);
                if (check2x2(blackCell, newStatus) && Continuity(blackCell, newStatus, whites, blacks + 1))
                {
                    // printf("--------------------------------------%d\n", ++count);
                    generate(head, blackCell, newStatus, blacks + 1, whites);
                }
            }
        }
    }
}

void showList(Node *head)
{
    Node *p = head;
    int count = 0;

    while (p != NULL)
    {
        if (Continuity(p->board, p->status, p->whites, p->blacks))
        {
            printf("status is: %d board is: %d, black: %d, white:%d\n", p->status, p->board, p->blacks, p->whites);
        }
        count++;

        p = p->next;
    }
    printf("\n%d\n", count);
}

void showBoard(short board)
{

    for (short i = 0; i < ROWS; i++)
    {
        for (short j = 0; j < COLS; j++)
        {
            bool cell = coord(board, i, j);

            if (cell)
                printf(RED "#" RESET);
            else
                printf(CYN "O" RESET);
        }
        printf("\n");
    }
}

void showBoards(Node *head, unsigned short num)
{

    Node *p = head;
    short count = 0;

    while (p != NULL)
    {

        // if (num != 0 && count++ == num) {
        //     return;
        // }
        // if (Continuity(p->board, 16, p->whites, p->blacks)) {
        printf("-------------------------\n");

        printf("status is: %d board is: %d, black: %d, white:%d\n", p->status, p->board, p->blacks, p->whites);
        showBoard(p->board);
        // }

        p = p->next;
    }
}

int main()
{
    Node *head = NULL;
    generate(&head, 0, 0, 0, 0);
    // insert(&head, 1, 1);
    // insert(&head, 2, 2);
    // insert(&head, 3, 3);
    // showList(head);
    // printf("%d",check2x2(8143, 63996));
    // printf("%d, %d", isEmpty(1, 0, 63996), coord(63996, 1,0));

    // printf("%d\n", countContinuCell(1, 0, 0, 0, 0));
    // printf("%d\n", Continuity(0, 22307, 8, 0));

    // showBoards(head, 0);
}
