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
	for (dictPtr = list[(int) *word - ASCII_LOWERCASE]; dictPtr != NULL; dictPtr = dictPtr->next)
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
	char *word, *ptr;
	int length;
	for (ptr = line; *(ptr + MIN_SIZE - 1); ++ptr)
		for (length = MIN_SIZE; *(ptr + length - 1); ++length) {
//			temp = ptr[length];
//			ptr[length] = '\0';
//			checkLine(ptr);
//			ptr[length] = temp;
			word = strndup(ptr, length);
			checkLine(word);
			free(word);
		}
}

/**
 * Goes through the grid from a left to right top to bottom method
 * @param grid the grid that will be parsed
 */
static void leftToRight(char **grid) {
	char **arrayPtr;
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

/**
 * Goes through the grid from a diagonal bottom left to top right method
 * @param grid the grid that will be parsed
 */
static void bottomLeftToTopRight(char **grid) {
	char line[width + 1];
	int i, j, k;
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

/**
 * Goes through the grid from a diagonal top right to a bottom left method
 * @param grid the grid that will be parsed
 */
static void topRightToBottomLeft(char **grid) {
	char line[width + 1];
	int i, j, k;
	for (i = width - MIN_SIZE - 1; i < width; ++i) {
		for (j = i, k = 0; j >= 0; --j, ++k)
			line[k] = grid[j][height - k - 1];
		line[k] = '\0';
		splitLine(line);
	}
	for (i = height - 1; i > 2; --i) {
		for (j = i, k = 0; j >= 0; --j, ++k)
			line[k] = grid[height - k - 1][j - 1];
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
	newWord->next = list[(int) *line - ASCII_LOWERCASE];
	list[(int) *line - ASCII_LOWERCASE] = newWord;
}

/**
 * Creates the data structure that will be used to find words
 * @param link the dictionary link for the program
 */
void createDictionary(char *link) {
	int i;
	for (i = 0; i < ALPHABET_SIZE; ++i)
		list[i] = NULL;

	FILE *fp = fopen(link, "r");
	fileExists(fp, link);
	char *line = NULL;
	size_t sz, ret;
	while ((ret = getline(&line, &sz, fp)) != EOF) {
		if (*line >= UPPERCASE_BEGIN && *line <= UPPERCASE_END)
			*line += TOLOWER_CASE;
		line[ret - 1] = '\0';
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
	for (int i = 0; i < ALPHABET_SIZE; ++i) {
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
	printf("\n");
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

/**
 * Prototype of how the threads can split into the different functions
 * @param grid the grid that will be measured through
 * @param num the designated number of what will be picked
 */
void programPicker(void *arg) {
	args *parameter = (args *) arg;
	for (int i = parameter->start; i < parameter->end; ++i) {
		switch (i) {
			case 0:
				leftToRight(parameter->grid);
				break;
			case 1:
				rightToLeft(parameter->grid);
				break;
			case 2:
				topToBottom(parameter->grid);
				break;
			case 3:
				bottomToTop(parameter->grid);
				break;
			case 4:
				topLeftToBottomRight(parameter->grid);
				break;
			case 5:
				bottomRightToTopLeft(parameter->grid);
				break;
			case 6:
				bottomLeftToTopRight(parameter->grid);
				break;
			case 7:
				topRightToBottomLeft(parameter->grid);
				break;
			default:
				break;
		}
	}
	free(parameter);
}

/**
 * linked function that will go through every algorithm of searching through the graph
 * @param grid the grid that will be searched for
 */
static void findWords(char **grid, int cores) {
	int split = 8 / cores, i;
	pthread_t *threads = malloc(sizeof(pthread_t) * cores);
	for (i = 0; i < cores; ++i) {
		args *param = malloc(sizeof(args));
		param->grid = grid;
		param->thread = i;
		param->start = split * i;
		param->end = i + split;
		pthread_create(&threads[i], NULL, (void *(*)(void *)) programPicker, param);
	}
	for (i = 0; i < cores; ++i)
		pthread_join(threads[i], NULL);
}

int main(int argc, char *argv[]) {
	double elapsedTime;
	struct timeval start, stop;
	gettimeofday(&start, NULL);
	if (argc > 2) {
		createDictionary(argv[2]);
		char **grid = createGrid(argv[1]);
		int cores = 1;
		if (argc > 3) {
			cores = atoi(argv[3]);
			if (atoi(argv[3]) > 8)
				cores = 8;
		}
		printPuzzle(grid);
		findWords(grid, cores);
		freeAll();
		gettimeofday(&stop, NULL);
		elapsedTime = (double) ((stop.tv_sec - start.tv_sec) * 1000000 + (stop.tv_usec - start.tv_usec)) / 1000000.0;
		printf("elapsedTime is %.3f\n", elapsedTime);
		return EXIT_SUCCESS;
	}
	fprintf(stderr, "requires 2 arguments, there are %d\n", argc - 1);
	return EXIT_FAILURE;
}