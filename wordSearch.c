//
// Created by Carter Boyd on 6/29/21.
//
#include "wordSearch.h"
#include <time.h>

/**
 * Checks the word for any of the word is in the dictionary
 * @param word the word being checked for in the dictionary
 */
static void checkLine(char *word) {
    struct letterList *letterPtr;
    struct dictionary *dictPtr;
    for (letterPtr = list; letterPtr != NULL; letterPtr = letterPtr->nextLetter)
        if (letterPtr->letter == *word)
            for (dictPtr = letterPtr->list; dictPtr != NULL; dictPtr = dictPtr->next)
                if (strcmp(dictPtr->word, word) == 0) {
                    printf("%s\n", dictPtr->word);
                    return;
                }
}

/**
 * takes the line and cuts it off at length then re adds it after the line is checked
 * @param length the length of the line that is going to be cut off
 * @param line the line being cut
 */
static void lineChange(char *line) {
    char temp;
    int length;
    for (length = MIN_SIZE; *(line + length - 1); ++length) {
        temp = line[length];
        line[length] = '\0';
        checkLine(line);
        line[length] = temp;
    }
}

/**
 * Goes through the grid from a left to right top to bottom method
 * @param grid the grid that will be parsed
 */
static void leftToRight(char **grid) {
    char *linePtr, **arrayPtr;
    printf("\nLooking for words going left to right\n");
    for (arrayPtr = grid; *arrayPtr; ++arrayPtr)
        for (linePtr = *arrayPtr; *(linePtr + MIN_SIZE - 1); ++linePtr)
            lineChange(linePtr);
}

/**
 * Goes through the grid from a left to right top to bottom method
 * @param grid the grid that will be parsed
 */
static void rightToLeft(char **grid) {
    char *linePtr, **arrayPtr, backwards[strlen(*grid) + 1], *ptr;
    int i;
    printf("\nLooking for words going right to left\n");
    for (arrayPtr = grid; *arrayPtr; ++arrayPtr) {
        for (linePtr = *arrayPtr, i = 0; i < height; ++i)
            backwards[i] = linePtr[height - i - 1];
        backwards[strlen(backwards)] = '\0';
        for (ptr = backwards; *(ptr + MIN_SIZE - 1); ++ptr)
            lineChange(ptr);
    }
}

/**
 * Goes through the grid from a left to right top to bottom method
 * @param grid the grid that will be parsed
 */
static void topToBottom(char **grid) {
    char line[strlen(*grid) + 1], *ptr;
    int i, j;
    printf("\nLooking for words going top to bottom\n");
    for (j = 0; j < width; ++j) {
        for (i = 0; i < height; ++i)
            line[i] = grid[i][j];
        for (ptr = line; *(ptr + MIN_SIZE - 1); ++ptr)
            lineChange(ptr);
    }
}

/**
 * Goes through the grid from a left to right top to bottom method
 * @param grid the grid that will be parsed
 */
static void bottomToTop(char **grid) {
    char line[width + 1], *ptr;
    int i, j;
    printf("\nLooking for words going bottom to top\n");
    for (j = 0; j < width; ++j) {
        for (i = height; i > 0; --i)
            line[i - 1] = grid[height - i][j];
        for (ptr = line; *(ptr + MIN_SIZE - 1); ++ptr)
            lineChange(ptr);
    }
}

/**
 * Goes through the grid from a diagonal top left to bottom right method
 * @param grid the grid that will be parsed
 */
static void leftToRightDiag(char **grid) {
    char line[width + 1], *ptr;
    int i, j, k;
    printf("\nLooking for words going diagonally left to right\n");
    for (i = MIN_SIZE - 1; i < width; ++i) {
        for (j = i, k = 0; j >= 0; --j, ++k) {
            ptr = grid[j];
            line[k] = grid[j][k];
        }
        for (ptr = line; *(ptr + MIN_SIZE - 1); ++ptr)
            for (i = MIN_SIZE; *(ptr + i); ++i)
                lineChange(ptr);
    }
}

/**
 * Goes through the grid from a diagonal top right to bottom left method
 * @param grid the grid that will be parsed
 */
static void rightToLeftDiag(char **grid) {
    //TODO implement code so that diagonal code will work
}

/**
 * Makes sure that the file exists
 * @param pFile the file
 * @param string the name of the file
 */
static void fileExists(FILE *pFile, char *string) {
    if (pFile == NULL) {
        fprintf(stderr, "%s file not found\n", string);
        exit(EXIT_FAILURE);
    }
}

/**
 * creates the data structure that will be used to find words
 * @param link the dictionary link for the program
 */
void createDictionary(char *link) {
    int letter;
    char *line = NULL;
    size_t sz;
    FILE *ptr = fopen(link, "r");
    fileExists(ptr, link);
    struct dictionary *newDict;
    for (letter = 97; letter < 123; ++letter) {
        struct letterList *newLetter = malloc(sizeof(struct letterList));
        newLetter->letter = (char) letter;
        newLetter->nextLetter = NULL;
        newLetter->list = NULL;
        if (line != NULL) {
            newDict = malloc(sizeof(struct dictionary));
            newDict->word = strdup(line);
            newDict->next = newLetter->list;
            newLetter->list = newDict;
        }
        while (getline(&line, &sz, ptr) != EOF) {
            if (*line >= 65 && *line <= 90)
                *line += 32;
            line[strlen(line) - 1] = '\0';
            if (*line != (char) letter)
                break;
            newDict = malloc(sizeof(struct dictionary));
            newDict->word = strdup(line);
            if (newLetter->list == NULL) {
                newLetter->list = newDict;
            } else {
                newDict->next = newLetter->list;
                newLetter->list = newDict;
            }
        }
        if (letter == 97)
            list = newLetter;
        else {
            struct letterList *letterPtr = list;
            while (letterPtr->nextLetter != NULL)
                letterPtr = letterPtr->nextLetter;
            letterPtr->nextLetter = newLetter;
        }
    }
    fclose(ptr);
}

/**
 * Frees all the malloced memory
 * @param piss the dictionary
 */
static void freeAll() {
    struct letterList *next;
    struct dictionary *linkedListNext, *linkedListTemp;
    while (list != NULL) {
        next = list->nextLetter;
        linkedListNext = list->list;
        while (linkedListNext != NULL) {
            linkedListTemp = linkedListNext->next;
            free(linkedListNext->word);
            free(linkedListNext);
            linkedListNext = linkedListTemp;
        }
        free(list);
        list = next;
    }
}

/**
 * prints the board of the grid
 * @param grid the grid being printed
 */
static void printPuzzle(char **grid) {
    printf("\nThis is the puzzle\n\n");
    char **gridPtr;
    for (gridPtr = grid; *gridPtr; ++gridPtr)
        printf("%s\n", *gridPtr);
}

char **createGrid(char *link) {
    char **wordSearchGrid, letter[3], *line;
    FILE *gridFile;
    gridFile = fopen(link, "r");
    fileExists(gridFile, link);
    fscanf(gridFile, "%d %d", &width, &height);
    wordSearchGrid = malloc(width * height * sizeof(wordSearchGrid));
    for (int i = 0; i < height; ++i) {
        line = malloc(height * sizeof(char));
        for (int j = 0; j < height; ++j) {
            fscanf(gridFile, "%2s", letter);
            line[j] = *letter;
        }
        line[height] = '\0';
        *(wordSearchGrid + i) = line;
    }
    *(*(wordSearchGrid) + width) = '\0';
    fclose(gridFile);
    return wordSearchGrid;
}

int main(int argc, char *argv[]) {
    clock_t begin = clock();

    argc = 3;
    argv[1] = "/Users/carterslaptop/CLionProjects/wordSearch/fourtyexample.txt";
    argv[2] = "/Users/carterslaptop/CLionProjects/wordSearch/dictionary.txt";
    if (argc - 1 == 2) {
        createDictionary(argv[2]);
        char **grid = createGrid(argv[1]);
        printPuzzle(grid);
        leftToRight(grid);
        rightToLeft(grid);
        topToBottom(grid);
        bottomToTop(grid);
//        leftToRightDiag(grid);
//        rightToLeftDiag(grid);
        freeAll();

        clock_t end = clock();
        double time_spent = (double) (end - begin) / CLOCKS_PER_SEC;
        printf("%f", time_spent);
        return EXIT_SUCCESS;
    }
    fprintf(stdout, "requires 2 arguments, there are %d\n", argc - 1);
    return EXIT_FAILURE;
}