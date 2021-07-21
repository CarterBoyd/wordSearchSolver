# wordSearch
Word Search program in C

This is a word search program designed in C that was originally created in 2019 when I was first learning java.
Compared to the java version this one will be able to go diagonally making it a complete word search. The program
requires two files in the line argument in order to work
[gridExample.txt](https://github.com/CarterBoyd/wordSearch/files/6844801/specExample.txt) is what the grid should
look like in order for the program to work.
[dictionary.txt](https://github.com/CarterBoyd/wordSearch/files/6844802/dictionary.txt) is an add-on of a ton of
words. The size of the dictionary file will be what determines how long the program will run. It took 2 seconds
on my laptop when making this and using a 40 x 40 sized grid.

# Usage
The program requires 2 arguments in order to work, the first argument being the word search txt file you wish to use.
The second argument is the dictionary file you want to use. Any extra Arguments will be ignored. When making a word
search text file make sure that the first two lines are the size of the width and height (in that order) so that the
program can perform boundary checks. The dictionary file make sure every word is on its own line the program will not be
able to separate words on the same line.

# Profiler
![WordSearchSolverProfiler](https://user-images.githubusercontent.com/70919992/126510637-4df6c013-53e7-4e12-9971-7627f1816427.png)
This is what the profiler graph was able to look like with the dictionary file and a 40 x 40 grid
