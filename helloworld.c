#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <time.h> 
#include <stdbool.h> 



#define MAX_NUM_OF_WORDS 100 

bool processGuess(const char* theAnswer, const char* theGuess) {
    //the clue
    char clue[6] = {'-', '-', '-', '-', '-', '\0'};
    //a set of flags indicating if that letter in the answer in used as clue
    bool answerFlags[5] = {false, false, false, false, false};
    
    //first pass, look for exact matches
    for (int i=0; i<5; i++) {
        if(theGuess[i] == theAnswer[i]) {
            clue[i] = 'G';
            answerFlags[i] = true;
        }
    }

    //second pass, look there but elsewhere
    for (int i=0; i<5l; i++) {
        if (clue[i] == '-') { //means no exact match from first pass
            for (int j=0; j<5; j++) {

                if (theGuess[i] == theAnswer[j] && !answerFlags[j]) {
                    //a match at another position and has not been used as clue
                    clue[i] = 'Y';
                    answerFlags[j] = true;
                    break; //end this J-loop we dont want multiple clues from same letter
                }
            }
        }
    } 
	for (int i=0; i<5; i++)
	{
		if  (clue[i]== 'G')
		printf("\033[0;32m %c", theGuess[i]);
		else if  (clue[i] == 'Y')
		printf("\033[0;33m %c", theGuess[i]);
		else 
		printf("\033[0;37m %c", theGuess[i]);

	} 
    printf("\033[0;37m \n");
    return strcmp(clue, "GGGGG") == 0; // Iif match strcmp returns 0
}



int main() {
    //load the words
    char** wordsList = calloc(MAX_NUM_OF_WORDS, sizeof(char*)); 
    int wordCount = 0;
    char* fiveLetterWord = malloc(6*sizeof(char));
    FILE* wordsFile = fopen("words.txt", "r");
    while(fscanf(wordsFile, "%s", fiveLetterWord)  != EOF && wordCount < MAX_NUM_OF_WORDS) {
        wordsList[wordCount] = fiveLetterWord;
        wordCount++;
        fiveLetterWord = malloc(6*sizeof(char));
    }
    fclose(wordsFile);

    //start the game
    //pick a word randomly
    srand(time(NULL));
    char* answer = wordsList[rand()%wordCount];

    //do the loop
    int num_of_guesses = 0;
    bool guessed_correctly = false;
    char* guess = malloc(6*sizeof(char));

    while (num_of_guesses < 6 && !guessed_correctly) {
        //get guess from player
        printf("Input a 5-letter word and press enter: ");
        scanf("%s", guess);
        printf("You have guessed %s\n", guess);
        num_of_guesses +=1;

        //process guess
        guessed_correctly = processGuess(answer, guess);
    }

    //display end of game message
    if(guessed_correctly) {
        printf("Congratulations! You Guessed the correct word in %d times!\n", num_of_guesses);

    } else {
        printf("You have used up your guesses ... the correct word is %s\n", answer);
    }

    for (int i=0; i<wordCount; i++) {
         free(wordsList[i]);
    }
    free(wordsList);
    free(fiveLetterWord);
    free(guess);

     
    return 0;
}
