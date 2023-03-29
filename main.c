// Program 2: Weaver
// CS 211, UIC, Spring 2023
// System: VScode for MacOS Monterey v12.4
// Author: Adam Yaagoub with starter code provided by Ellen Kidane

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//The checkLength function takes in a pointer to a char array and the number of chars that should be in the string
// and returns whether or not the string is that length. 1 is used for true, 0 for false.
//Inputs:
//	char* str: pointer to a C style string
//	int numChars: the size str should be
//Outputs:
//	returns 1 or 0 to signify true or false
int checkLength(char* str, int numChars){
	if(strlen(str) == numChars){
		return 1;
	}else{
		return 0;
	}
}

//The checkWords function iterates over the entire 2d array of strings of a certain length and returns 1 if it is found.
//Inputs:
//	char* str: pointer to a C style string
//	char** words: 2d array of C style strings of a predesignated length passed throughout the program
//	int size: the number of elements within the 2d array
//Outputs:
//	returns 1 or 0 to signify true or false
int checkWords(char* str, char** words, int size){
	for(int i = 0; i < size; i++){
		if(strcmp(str, words[i]) == 0){
			return 1;
		}
	}
	return 0;
}

//The checkDiff function takes two strings and compares them char by char to see if there is exactly one 
// character different between them.
//Inputs:
//	char str[]: user entered array of characters
//	char prev[]: previously entered array of characters
//Outputs:
//	returns 1 or 0 to signify true or false
int checkDiff(char str[], char prev[]){
	int numDiff = 0;
	int i = 0;
	while(str[i] != '\0'){
		if(str[i] != prev[i]){
			numDiff++;
		}
		i++;
	}
	if(numDiff == 1){
		return 1;
	}else{
		return 0;
	}
}

//The resize function dynamically resizes the words 2d array being used throughout the program. Every 5 elements,
// this function is called and size for 5 more elements are added.
//Inputs: 
//	char ***words: a pointer to the 2d array words used to modify it
//	int size: the number of elements in words
//Outputs:
//	Nothing is returned as we pass by pointer
void resize(char*** words, int size){
	char** newP = (char**) malloc(sizeof(char*) * (size + 5));
	for(int i = 0; i < size; i++){
		newP[i] = malloc(sizeof(char) * 81);
    	strcpy(newP[i], *(*words + i));
		free(*(*words + i));
	}
	for(int i = size; i < size + 5; i++){
		newP[i] = (char*) malloc(sizeof(char) * 81);
	}
	free(*words);
	*words = newP;
}

//The load function reads the words.txt file, checks the length of each string, and adds it to the words 2d array
// if the length matches. This function calls the resize function everytime words is full.
//Inputs:
//	char* filename: a pointer to a char array containing the name of the file being read (words.txt)
//	int numChars: the predefined length each string being added to words must be
//	int* numWords: a pointer to the number of elements in the words 2d array. This is updated when num elements is.
//	int* currSize: a pointer to the size of the words 2d array. This is updated when it is resized
//Outputs:
//	returns char** words to the main function to be used in the game
char** load(char* filename, int numChars, int* numWords, int* currSize){
	*currSize = 5;
	char** words = (char**) malloc(sizeof(char*) * 5);
	for(int i = 0; i < *currSize; i++){
		words[i] = (char*) malloc(sizeof(char) * 81);
	}

	FILE *filePtr  = fopen(filename, "r"); // "r" means we open the file for reading
	
	// Check that the file was able to be opened
	if(filePtr == NULL) {
		printf("Error: could not open %s for reading\n", filename);
		exit(-1);
	}

	// Read each word from file, and print them one by one
	char inputString[ 81];
	while(fscanf(filePtr, "%s", inputString) != EOF) {
		if(strlen(inputString) == numChars){
			if(*numWords == *currSize){
				resize(&words, *numWords);
				*currSize += 5;
			}
			strcpy(words[*numWords], inputString);
			(*numWords)++;
		}
	}

	// Close the file
	fclose(filePtr);

	return words;
}

int main() {
	srand(1);
	char filename[] = "words.txt";
	char** words = NULL;
	int numLetters = 4;
	int totalSize = 0;
	int numWords = 0;
	int menuChoice = 2;

	printf("Weaver is a game where you try to find a way to get from the starting word to the ending word.\n");
	printf("You can change only one letter at a time, and each word along the way must be a valid word.\n");
	printf("Enjoy!\n\n");
	while(menuChoice != 3){
		//Change length of words
		if(menuChoice == 2){
			//Frees the words 2d array everytime the user wants a different number of characters for the game.
			if(words != NULL){
				for(int i = 0; i < totalSize; i++){
    				free(words[i]);
  				}
  				free(words);
				totalSize = 0;
				numWords = 0;
			}
			printf("How many letters do you want to have in the words? ");
			scanf("%d", &numLetters);
			words = load(filename, numLetters, &numWords, &totalSize);
			printf("Number of %d-letter words found: %d.\n\n", numLetters, numWords);
			menuChoice = 1;
		}
		//Play game
		if(menuChoice == 1){
			char starting[81] = "";
			char ending[81] = "";
			while(checkLength(starting, numLetters) == 0 || checkWords(starting, words, numWords) == 0 || checkLength(ending, numLetters) == 0 || checkWords(ending, words, numWords) == 0){
				printf("Enter starting and ending words, or 'r' for either for a random word: ");
				scanf("%s %s", &starting, &ending);	
				if(strcmp(starting,"r") == 0){
					strcpy(starting, words[rand() % numWords]);
				}else{
					if(checkLength(starting, numLetters) == 0){
						printf("Your word, '%s', is not a %d-letter word. Try again.\n", starting, numLetters);
						continue;
					}else if(checkWords(starting, words, numWords) == 0){
						printf("Your word, '%s', is not a valid dictionary word. Try again.\n", starting);
						continue;
					}
				}
				if(strcmp(ending,"r") == 0){
					strcpy(ending, words[rand() % numWords]);
				}else{
					if(checkLength(ending, numLetters) == 0){
						printf("Your word, '%s', is not a %d-letter word. Try again.\n", ending, numLetters);
						continue;
					}else if(checkWords(ending, words, numWords) == 0){
						printf("Your word, '%s', is not a valid dictionary word. Try again.\n", ending);
						continue;
					}
				}
			}
			printf("Your starting word is: %s.\n", starting);
			printf("Your ending word is: %s.\n\n", ending);

			int numMoves = 1;
			char guess[81] = "";
			char prev[81];
			strcpy(prev, starting);
			int fail = 0;
			printf("On each move enter a word of the same length that is at most 1 character different and is also in the dictionary.\n");
    		printf("You may also type in 'q' to quit guessing.\n");
			do{
				printf("\n%d. Previous word is '%s'. Goal word is '%s'. Next word: ", numMoves, prev, ending);
				scanf("%s", &guess);
				if(checkLength(guess, numLetters) != 1 && strcmp(guess, "q") != 0){
					printf("Your word, '%s', is not a %d-letter word. Try again.\n", guess, numLetters);
					continue;
				}else if(checkWords(guess, words, numWords) != 1 && strcmp(guess, "q") != 0){
					printf("Your word, '%s', is not a valid dictionary word. Try again.\n", guess);
					continue;
				}else if(checkDiff(guess, prev) != 1 && strcmp(guess, "q") != 0){
					printf("Your word, '%s', is not exactly 1 character different. Try again.\n", guess);
					continue;
				}
				numMoves++;
				
				if(strcmp(guess, ending) == 0 && checkDiff(prev,guess) == 1){
					break;
				}
				strcpy(prev, guess);
			}while(strcmp(guess, "q") != 0);

			if(strcmp(guess, ending) == 0){
				printf("Congratulations! You changed '%s' into '%s' in %d moves.\n", starting, ending, numMoves - 1);
			}
		}

		printf("\nEnter: \t1 to play again,\n");
    	printf("\t2 to change the number of letters in the words and then play again, or\n");
    	printf("\t3 to exit the program.\n");
    	printf("Your choice --> ");
		scanf("%d", &menuChoice);
	}
	//Final free of the words 2d array
	if(words != NULL){
		for(int i = 0; i < totalSize; i++){
    		free(words[i]);
  		}
  		free(words);
	}
	printf("\nThanks for playing!\nExiting...");
	
	return 0;
}