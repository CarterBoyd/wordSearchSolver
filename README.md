# wordSearch
Word Search program in C

This is a word search program designed in C that was originally created in 2019 when I was first learning java.
Compared to the java version this one will be able to go diagonally so that it can complete the real wordSearch process.
unlike the java version which takes the dictionary txt file and adds those words to a set this method uses a 2D linked
list. Where the first linked list is just a 27 node linked list from A - Z to signify the first letter for every word.
Inside the alphabet linked list is the real list of every word that starts with that letter.

#Possible changes
I might want to change the way the linked list for the alphabet by remaking it as a binary tree. I just need to change
the data structure.