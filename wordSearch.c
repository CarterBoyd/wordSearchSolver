//
// Created by Carter Boyd on 6/29/21.
//
#include "wordSearch.h"

/**
 * Checks the word for any of the word is in the dictionary
 * @param word the word being checked for in the dictionary
 */
static void checkLine(const char *word) {
    struct dictionary *dictPtr;
    for (dictPtr = list[(int) *word - 97]; dictPtr != NULL; dictPtr = dictPtr->next)
        if (strcmp(dictPtr->word, word) == 0) {
            printf("%s\n", dictPtr->word);
            return;
        }
}

/**
 * Takes the line and cuts it off at length then re adds it after the line is checked
 * @param length the length of the line that is going to be cut off
 * @param line the line being cut
 */
static void splitLine(char *line) {
    char temp, *ptr;
    int length;
    for (ptr = line; *(ptr + MIN_SIZE - 1); ++ptr)
        for (length = MIN_SIZE; *(ptr + length - 1); ++length) {
            temp = ptr[length];
            ptr[length] = '\0';
            checkLine(ptr);
            ptr[length] = temp;
        }
}

/**
 * Goes through the grid from a left to right top to bottom method
 * @param grid the grid that will be parsed
 */
static void leftToRight(char **grid) {
    char **arrayPtr;
    printf("\nLooking for words going left to right\n");
    for (arrayPtr = grid; *arrayPtr; ++arrayPtr)
        splitLine(*arrayPtr);
}

/**
 * Goes through the grid from a left to right top to bottom method
 * @param grid the grid that will be parsed
 */
static void rightToLeft(char **grid) {
    char *linePtr, **arrayPtr, backwards[strlen(*grid) + 1];
    backwards[strlen(*grid)] = '\0';
    int i;
    printf("\nLooking for words going right to left\n");
    for (arrayPtr = grid; *arrayPtr; ++arrayPtr) {
        for (linePtr = *arrayPtr, i = 0; i < height; ++i)
            backwards[i] = linePtr[height - i - 1];
        splitLine(backwards);
    }
}

/**
 * Goes through the grid from a left to right top to bottom method
 * @param grid the grid that will be parsed
 */
static void topToBottom(char **grid) {
    char line[height + 1];
    line[height] = '\0';
    int i, j;
    printf("\nLooking for words going top to bottom\n");
    for (j = 0; j < width; ++j) {
        for (i = 0; i < height; ++i)
            line[i] = grid[i][j];
        splitLine(line);
    }
}

/**
 * Goes through the grid from a left to right top to bottom method
 * @param grid the grid that will be parsed
 */
static void bottomToTop(char **grid) {
    char line[width + 1];
    line[width] = '\0';
    int i, j;
    printf("\nLooking for words going bottom to top\n");
    for (j = 0; j < width; ++j) {
        for (i = height; i > 0; --i)
            line[i - 1] = grid[height - i][j];
        splitLine(line);
    }
}

/**
 * Goes through the grid from a diagonal top left to bottom right method
 * @param grid the grid that will be parsed
 */
static void topLeftToBottomRight(char **grid) {
    char line[width + 1];
    int i, j, k;
    printf("\nLooking for words going diagonally left to right\n");
    for (i = MIN_SIZE - 1; i < width; ++i) {
        for (j = i, k = 0; j >= 0; --j, ++k)
            line[k] = grid[j][k];
        line[k] = '\0';
        splitLine(line);
    }
    for (i = 1; i < height - 2; ++i) {
        for (j = i, k = 0; j < height; ++j, ++k)
            line[k] = grid[height - k - 1][j];
        line[k] = '\0';
        splitLine(line);
    }
}

/**
 * Goes through the grid from a diagonal bottom right to top left method
 * @param grid the grid that will be parsed
 */
static void bottomRightToTopLeft(char **grid) {
    char line[width + 1];
    int i, j, k;
    printf("\nLooking for words going diagonally right to left\n");
    for (i = width - MIN_SIZE; i >= 0; --i) {
        for (j = i, k = 0; j < height; ++j, ++k)
            line[k] = grid[j][height - k - 1];
        line[k] = '\0';
        splitLine(line);
    }
    for (i = height - MIN_SIZE - 1; i >= MIN_SIZE - 1; --i) {
        for (j = i, k = 0; j >= 0; --j, ++k)
            line[k] = grid[k][j];
        line[k] = '\0';
        splitLine(line);
    }
}

static void bottomLeftToTopRight(char **grid) {
    char line[width + 1];
    int i, j, k;
    printf("\nLooking for words going diagonally from the bottom left to the top right\n");
    for (i = width - MIN_SIZE; i >= 0; --i) {
        for (j = i, k = 0; j < height; ++j, ++k)
            line[k] = grid[j][k];
        line[k] = '\0';
        splitLine(line);
    }
    for (i = 1; i < height - MIN_SIZE + 1; ++i) {
        for (j = i, k = 0; j < height; ++j, ++k)
            line[k] = grid[k][j];
        line[k] = '\0';
        splitLine(line);
    }
}

static void topRightToBottomLeft(char **grid) { // apd, zwyo ...
    char line[width + 1];
    int i, j, k;
    printf("\nLooking for words going diagonally from the top right to the bottom left\n");
    for (i = width - MIN_SIZE - 1; i < width; ++i) {
        for (j = i, k = 0; j >= 0; --j, ++k)
            line[k] = grid[j][height - k - 1];
        line[k] = '\0';
        splitLine(line);
    }
}

/**
 * Makes sure that the file exists if not then it will end the program
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
 * Adds the line into the linked list of the list
 * @param line the line being added to the array
 */
static void addToList(const char *line) {
    struct dictionary *newWord = malloc(sizeof(struct dictionary));
    newWord->word = strdup(line);
    newWord->next = list[(int) *line - 97];
    list[(int) *line - 97] = newWord;
}

/**
 * Creates the data structure that will be used to find words
 * @param link the dictionary link for the program
 */
void createDictionary(char *link) {
    int i;
    for (i = 0; i < 26; ++i)
        list[i] = NULL;

    FILE *fp = fopen(link, "r");
    fileExists(fp, link);
    char *line = NULL;
    size_t sz;
    while (getline(&line, &sz, fp) != EOF) {
        if (*line >= 65 && *line <= 90)
            *line += 32;
        line[strlen(line) - 1] = '\0';
        addToList(line);
    }
    fclose(fp);
}

/**
 * Frees all the malloced memory
 * @param piss the dictionary
 */
static void freeAll() {
    struct dictionary *listPtr, *temp;
    for (int i = 0; i < 26; ++i) {
        temp = list[i];
        while (temp != NULL) {
            listPtr = temp->next;
            free(temp->word);
            free(temp);
            temp = listPtr;
        }
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

/**
 * Creates the grid that will be used in the word search
 * @param link the txt file containing the list
 * @return a 2d pointer of the grid
 */
static char **createGrid(char *link) {
    char **grid, letter[3], *line;
    FILE *gridFile;
    gridFile = fopen(link, "r");
    fileExists(gridFile, link);
    fscanf(gridFile, "%d %d", &width, &height);
    grid = malloc(width * height * sizeof(grid));
    for (int i = 0; i < height; ++i) {
        line = malloc(height * sizeof(char));
        for (int j = 0; j < height; ++j) {
            fscanf(gridFile, "%2s", letter);
            line[j] = *letter;
        }
        line[height] = '\0';
        *(grid + i) = line;
    }
    *(*(grid) + width) = '\0';
    fclose(gridFile);
    return grid;
}

int main(int argc, char *argv[]) {
    if (argc > 2) {
        createDictionary(argv[2]);
        char **grid = createGrid(argv[1]);
        printPuzzle(grid);
        leftToRight(grid);
        rightToLeft(grid);
        topToBottom(grid);
        bottomToTop(grid);
        topLeftToBottomRight(grid);
        bottomRightToTopLeft(grid);
        bottomLeftToTopRight(grid);
        topRightToBottomLeft(grid);
        freeAll();
        return EXIT_SUCCESS;
    }
    fprintf(stderr, "requires 2 arguments, there are %d\n", argc - 1);
    return EXIT_FAILURE;
}