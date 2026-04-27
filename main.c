/*--------------------------------------------------------------
Program 3: Spelling Bee
Description: Spelling Bee game program that uses hive letters and required letters to generate the game.
Utilizes binary search, recursion, memory allocation, and dynamic arrays, to make the game function
Course: CS 211 UIC
Author: Isaac Lopez
--------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>

const int MIN_HIVE_SIZE = 2;
const int MAX_HIVE_SIZE = 12;
const int MIN_WORD_LENGTH = 4;

typedef struct WordList_struct {
    char** words;
    int numWords;
    int capacity;
} WordList;

WordList* createWordList() {
    WordList* newList = malloc(sizeof(WordList));
    newList->capacity = 4;
    newList->numWords = 0;
    newList->words = malloc(newList->capacity * sizeof(char*));

    return newList;
}

void appendWord(WordList* thisWordList, char* newWord) {
    //---------------------------------------------------------------------
    /* TODO (Task 1-A): Write appendWord
    - adds newWord to the end of the thisWordList (malloc'ing memory and copying the string; do NOT just copy the input pointer)
    - increments thisWordList's numWords. If there is no space for the new word, reallocate with double the capacity
    - by malloc'ing new memory for the words array and freeing the old memory
    */

    // Checks if we need reallocation
    if(thisWordList->numWords == thisWordList->capacity) { 
        int increaseCapacity = thisWordList->capacity * 2; 

        char** increaseArray = (char**)malloc(sizeof(char*) * increaseCapacity);
        // Copies old pointers to the new array
        for(int i = 0; i < thisWordList->numWords; i++) { 
            increaseArray[i] = thisWordList->words[i];
        }
        // Free's the old pointer
        free(thisWordList->words);
        thisWordList->words = increaseArray; 
        thisWordList->capacity = increaseCapacity; 
    }

    // Allocates memory for the new word and stirng copy 
    char* copyWord = (char*)malloc(sizeof(char) * (strlen(newWord) + 1)); 
    strcpy(copyWord, newWord); 
    // Appends the words and increments numWords 
    thisWordList->words[thisWordList->numWords] = copyWord; 
    thisWordList->numWords++;
}

int buildDictionary(char* filename, WordList* dictionaryList, int minLength) {
    //---------------------------------------------------------------------
    /* TODO (Task 1-B): Write buildDictionary
    - Opens a valid file and adds all words that are at least minLength long to the dictionaryList
    - Returns the length of the longest valid word if all goes well, -1 otherwise
    */

    // Opens the file 
    FILE* openFile = fopen(filename, "r");
    // If openfile == NULL it returns -1 meaning that it is unvalid
    if(openFile == NULL) { 
        return -1;
    }

    int maxLength = 0;
    char word[100];
    
    // Reads the words from the file
    while(fscanf(openFile, "%s", word) == 1) { 
        int length = strlen(word); 
        // Appends to dictionary list and then keeps track of the longest word
        if(length >= minLength) { 
            appendWord(dictionaryList, word);
            if(length > maxLength) { 
                maxLength = length;
            }
        }
    }
    // Closes file
    fclose(openFile);
    // Checks if there are no valid words
    if(dictionaryList->numWords == 0) { 
        return -1;
    }

    return maxLength; //this line is here so that the starter code compiles; replace/modify this line
}

void freeWordList(WordList* list) {
    //---------------------------------------------------------------------
    /* TODO (Task 1-C): Write freeWordList
    - Frees the memory used by the WordList, both the words' char* arrays and the char** words array itself
    */
    if(list == NULL) { 
        return; 
    }
    // Frees strings
    for(int i = 0; i < list->numWords; i++) {
        free(list->words[i]);
    }
    
    // Frees the array along with the struct
    free(list->words);
    free(list);
}

int findLetter(char* str, char aLet) {
    //---------------------------------------------------------------------
    /* TODO (Task 2-A): Write findLetter
    - Returns the index of aLet in str, if it exists
    - Otherwise returns -1
    */
    // Checks for the index of aLet in the string and returns -1 if it is not found
    for(int i = 0; str[i] != '\0'; i++) { 
        if(str[i] == aLet) { 
            return i;
        }
    }
    return -1;
}

void buildHive(char* str, char* hive) {
    //---------------------------------------------------------------------
    /* TODO (Task 2-B): Write buildHive
    - take each character of the string and add it to the hive in alphabetical order
    - no letter should be added twice
    - ensure that hive is null-terminated at the end
      (findLetter is helpful here)
    */
    // The hive is initialized as empty
    hive[0] = '\0';
    int length = 0; 
    // Checks every character in the string 
    for(int i = 0; str[i] != '\0'; i++) { 
        // Stores the current character from the string
        char c = str[i];
        // Checks if the character exists in the hive 
        if(findLetter(hive, c) == -1) { 
            int order = length - 1;
            // Moves the letters to the right until it is ordered
            while(order >= 0 && hive[order] > c) { 
                hive[order + 1] = hive[order];
                order--;
            } 
            // Moves the larger letter to the right, then increases the length of the hive
            hive[order + 1] = c;
            length++;

            hive[length] = '\0';
        }
    }
}

int countUniqueLetters(char* str) {
    //---------------------------------------------------------------------
    /* TODO (Task 3-A): Write countUniqueLetters
    - Counts and returns the number of unique letters in the input string 
      (note: this only needs to work for lower-case alpha characters, i.e. letters, for this project, 
            but it should work generally for all characters)
    */
    // Stores unique letters
    int count = 0;
    // Checks every charactre that is in the string until it reaches '\0'
    for(int i = 0; str[i] != '\0'; i++) { 
        if(findLetter(str, str[i]) == i) { 
            count++;
        }
    }
    // Returns the amount of unique characters 
    return count;
}

WordList* findAllFitWords(WordList* dictionaryList, int hiveSize) {
    // Creates a new word list that will make sure the word fits
    WordList* fitWords = createWordList();
    //---------------------------------------------------------------------
    /* TODO (Task 3-B): Write findAllFitWords
    - Creates a WordList and adds all fit words from dictionaryList to the new WordList 
      (A fit word has exactly hiveSize unique letters)
    */

    // Checks through all of the words in the dictionary
    for(int i = 0; i < dictionaryList->numWords; i++) { 
        // Gives back the current word from dictionary
        char* words = dictionaryList->words[i];
        // Counts the unique letters in the word
        if(countUniqueLetters(words) == hiveSize) { 
            appendWord(fitWords, words);
        }
    }
    // Return valid words that will fit
    return fitWords;
}

bool isValidWord(char* word, char* hive, char reqLet) {
    //---------------------------------------------------------------------
    /* TODO (Task 4-A, part 1 of 2): Write isValidWord 
    - Returns true if word 
      (a) uses ONLY letters that are in the hive, and 
      (b) MUST use the required letter. 
    - Returns false otherwise
    */

    // Checks if the required letter is in the word
    bool requiredLetter = false; 
    // Checks every character in the word
    for(int i = 0; word[i] != '\0'; i++) { 
        // Checks if it exists in the hive
        if(findLetter(hive, word[i]) == -1) { 
            return false;
        }
        // Checks if the current character is the required letter
        if(word[i] == reqLet) { 
            requiredLetter = true; 
        }
    }
    // makes sure that the letter was used and then return true if the letters are valid
    if(!requiredLetter) { 
        return false;
    }
    return true;
}

bool isPangram(char* str, char* hive) {
    //---------------------------------------------------------------------
    /* TODO (Task 4-B): Write isPangram
    - Returns true if str is a pangram (uses all the letters in hive at least once)
    - Returns false otherwise
    */
    // Checks every letter in the hive
    for(int i = 0; hive[i] != '\0'; i++) { 
        // Checks if the letter appears in the word
        if(findLetter(str, hive[i]) == -1) { 
            return false;
        }
    }
    // Returns true if every letter was found 
    return true;
}


void printHive(char* hive, int reqLetInd) {
    printf("  Hive: \"%s\"\n", hive);
    printf("         ");
    for (int i = 0; i < reqLetInd; i++) {
        printf(" ");
    }
    printf("^");
    for (int i = reqLetInd + 1; i < strlen(hive); i++) {
        printf(" ");
    }
    printf(" (all words must include \'%c\')\n\n", hive[reqLetInd]);
}

void printList(WordList* thisWordList, char* hive) {
    //---------------------------------------------------------------------
    /* TODO (Task 4-C): Write printList
    - Outputs the words in the list 
    - Marks pangrams with asterisk and perfect pangrams with three asterisks
    - Displays score for each word as well as total score at the bottom
    See example shown below.
    Word List:
    *** (16) computer
        ( 7) compute
        ( 1) mute
     *  (17) recompute
        ( 5) comet
    Total Score: 46    
    */
        
    printf("  Word List:\n");
    // Keeps track of the total score
    int totScore = 0;
    // Stores the number of letters in the hive
    int length = strlen(hive);
    // Checks each stored word in the list
    for(int i = 0; i < thisWordList->numWords; i++) { 
        // Gives the current word
        char* word = thisWordList->words[i];
        int wordlength = strlen(word);
        // Checks if the word is a pangram that contains all of the letters
        bool pangram = isPangram(word, hive);
        // Checks if the pangram uses all of the letters and have the same length to decide if it is perfect
        bool perfectpangram = pangram && (wordlength == length);

        // 4 letters are worth 1 point otherwise, the score = wordlength
        int points = (wordlength == 4) ? 1 : wordlength;

        // Adds points to pangrams
        if(pangram) { 
            points += 8;
        }
        // Output that the autograder wanted based on the type of pangram
        if(perfectpangram) { 
            printf("  *** ");
        } else if(pangram) { 
            printf("   *  ");
        } else { 
            printf("      ");
        }
        printf("(%d) %s\n", points, word);
        // Adds the points that have been accumulated to the total score
        totScore += points;
    }
    printf("  Total Score: %d\n", totScore);

}

void bruteForceSolve(WordList* dictionaryList, WordList* solvedList, char* hive, char reqLet) {
    //---------------------------------------------------------------------
    /* TODO (Task 5-A): Write bruteForceSolve
    - Goes through each word in the dictionary and determines if that is a solution to the puzzle
    - Adds the word to solvedList if it is a solution
    */

    // Loops through every word in the dictionary
    for(int i = 0; i < dictionaryList->numWords; i++) { 
        // Gives the current word
        char* word = dictionaryList->words[i];
        // Checks if the word is valid
        if(isValidWord(word, hive, reqLet)) { 
            // Appends the word to solved list
            appendWord(solvedList, word);
        }
    }
}

bool isPrefix(char* partWord, char* fullWord) {
    //---------------------------------------------------------------------
    /* TODO (Task 6-A): Write isPrefix
    - checks if partWord is the root/prefix of fullWord, such that 
        partWord is a perfect match to the beginning (arbitrary length) of fullWord
    - returns true is partWord is a root/prefix of fullWord; returns false otherwise 
    */
    // Loops through characters in partword
    for(int i = 0; partWord[i] != '\0'; i++) { 
        // Compares the character in partWord to fullWord
        if(partWord[i] != fullWord[i]) { 
            return false;
        }
    }
    // Returns true when the character in partword matches the beginning of fullWord
    return true; //this line is here so that the starter code compiles; replace/modify this line
}

int findWord(WordList* thisWordList, char* aWord, int loInd, int hiInd) {
    /* TODO (Task 6-B): Complete findWord
    * - Fix all the if statements that say 'if(true)' with the correct binary search cases
    */
    if (hiInd < loInd) { // Base case 2: aWord not found in words[]
        //
        if (loInd < thisWordList->numWords && isPrefix(aWord, thisWordList->words[loInd])) { //TODO: fix the second part of this if statement
            return -1; //words match this root (partial match)
        }
        else {
            return -99; //no more words matching this root (no match)
        }
    }

    int mdInd = (hiInd + loInd) / 2;
    char* tryWord = thisWordList->words[mdInd];
    // Checks if the word matches with the middle word
    if (strcmp(aWord, tryWord) == 0) { // Base case 1: found tryWord at midInd
        return mdInd;
    }
    // Checks if aWord comes after tryWord 
    if (strcmp(aWord, tryWord) > 0) { // Recursive case: search upper half
        return findWord(thisWordList, aWord, mdInd + 1, hiInd);
    }
    // If aWord comes after, check the lower half
    // Recursive case: search lower half
    return findWord(thisWordList, aWord, loInd, mdInd - 1);
}

void findAllMatches(WordList* dictionaryList, WordList* solvedList, char* tryWord, char* hive, char reqLet) {
    //---------------------------------------------------------------------
    /* TODO (Task 6-C): Complete findAllMatches
    */
    // Current length of the word
    int curLen = strlen(tryWord);
    if(curLen == 0) { 
        return;
    }
    // Searches the dictionary to see if tryWord exists using binary searchSearch dictionary using binary search
    int index = findWord(dictionaryList, tryWord, 0, dictionaryList->numWords - 1);
    // Stops searching when the result is -99
    if(index == -99) { 
        for(int length = strlen(tryWord); length > 0; length--) {
            // Gets the index of the last character in the current word
            int lastpos = length - 1;
            // Find the position of the letter in hive
            int hiveind = findLetter(hive, tryWord[lastpos]);
            // If this letter is not the last letter, advance it to the next hive
            if(hiveind < strlen(hive) - 1) {
                tryWord[lastpos] = hive[hiveind + 1];
                tryWord[lastpos + 1] = '\0';
                findAllMatches(dictionaryList, solvedList, tryWord, hive, reqLet);
                return;
            } 
            tryWord[lastpos] = '\0';
        }
        return;
    }
    // Checks if the current tryWord exists in the dictionary and sees if it is the minimum length
    if (index >= 0 && isValidWord(tryWord, hive, reqLet) && curLen >= MIN_WORD_LENGTH) {
        bool duplicate = false; 
        for(int i = 0; i < solvedList->numWords; i++) { 
            if(strcmp(solvedList->words[i], tryWord) == 0) { 
                // Prevents duplicates from being added
                duplicate = true; 
                break;
            }
        }
        if(!duplicate) { 
            appendWord(solvedList, tryWord);
        }
    }

    //call recursive function here ONLY if tryWord is not empty 
    tryWord[curLen] = hive[0];
    tryWord[curLen + 1] = '\0';
    findAllMatches(dictionaryList, solvedList, tryWord, hive, reqLet);
}

// Provided for you, to determine the program settings based on parameters
bool setSettings(int argc, char* argv[], bool* pRandMode, int* pNumLets, char dictFile[100], bool* pPlayMode, bool* pBruteForceMode, bool* pSeedSelection) {
    *pRandMode = false;
    *pNumLets = 0;
    strcpy(dictFile, "dictionary.txt");
    *pPlayMode = false;
    *pBruteForceMode = true;
    *pSeedSelection = false;
    srand((int)time(0));
    //--------------------------------------
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-r") == 0) {
            ++i;
            if (argc == i) {
                return false;
            }
            *pRandMode = true;
            *pNumLets = atoi(argv[i]);
            if (*pNumLets < MIN_HIVE_SIZE || *pNumLets > MAX_HIVE_SIZE) {
                return false;
            }
        }
        else if (strcmp(argv[i], "-d") == 0) {
            ++i;
            if (argc == i) {
                return false;
            }
            strcpy(dictFile, argv[i]);
            FILE* filePtr = fopen(dictFile, "r");
            if (filePtr == NULL) {
                return false;
            }
            fclose(filePtr);
        }
        else if (strcmp(argv[i], "-s") == 0) {
            ++i;
            if (argc == i) {
                return false;
            }
            *pSeedSelection = true;
            int seed = atoi(argv[i]);
            srand(seed);
        }
        else if (strcmp(argv[i], "-p") == 0) {
            *pPlayMode = true;
        }
        else if (strcmp(argv[i], "-o") == 0) {
            *pBruteForceMode = false;
        }
        else {
            return false;
        }
    }
    return true;
}

void printONorOFF(bool mode) {
    if (mode) {
        printf("ON\n");
    }
    else {
        printf("OFF\n");
    }
}

void printYESorNO(bool mode) {
    if (mode) {
        printf("YES\n");
    }
    else {
        printf("NO\n");
    }
}



int main(int argc, char* argv[]) {

    printf("\n----- Welcome to the CS 211 Spelling Bee Game & Solver! -----\n\n");
    // Configuration variables for the program
    bool randMode = false;
    int hiveSize = 0;
    char dict[100] = "dictionary.txt";
    bool playMode = false;
    bool bruteForce = true;
    bool seedSelection = false;
    char hive[MAX_HIVE_SIZE + 1] = {};
    hive[0] = '\0';
    int reqLetInd = -1;
    char reqLet = '\0';

    // Updates settings and processes command-line arguments
    if (!setSettings(argc, argv, &randMode, &hiveSize, dict, &playMode, &bruteForce, &seedSelection)) {
        printf("Invalid command-line argument(s).\nTerminating program...\n");
        return 1;
    }
    else {
        // Displays the program's settings
        printf("Program Settings:\n");
        printf("  random mode = ");
        printONorOFF(randMode);
        printf("  play mode = ");
        printONorOFF(playMode);
        printf("  brute force solution = ");
        printONorOFF(bruteForce);
        printf("  dictionary file = %s\n", dict);
        printf("  hive set = ");
        printYESorNO(randMode);
        printf("\n\n");
    }

    // build word array (only words with desired minimum length or longer) from dictionary file
    printf("Building array of words from dictionary... \n");
    // Creates an empty word dictionary to store words in
    WordList* dictionaryList = createWordList();
    // Builds the dictionary by storing words that meet min length and by reading files 
    int maxWordLength = buildDictionary(dict, dictionaryList, MIN_WORD_LENGTH);
    // If the maxWordLength is not found, the program fails and ends the program
    if (maxWordLength == -1) {
        printf("  ERROR in building word array.\n");
        printf("  File not found or incorrect number of valid words.\n");
        printf("Terminating program...\n");
        return 0;
    }
    printf("   Word array built!\n\n");


    printf("Analyzing dictionary...\n");
    // Dictionary words can not be negative, otherwise, the program ends
    if (dictionaryList->numWords < 0) {
        printf("  Dictionary %s not found...\n", dict);
        printf("Terminating program...\n");
        return 0;
    }

    // end program if file has zero words of minimum desired length or longer
    if (dictionaryList->numWords == 0) {
        printf("  Dictionary %s contains insufficient words of length %d or more...\n", dict, MIN_WORD_LENGTH);
        printf("Terminating program...\n");
        return 0;
    }
    else {
        printf("  Dictionary %s contains \n  %d words of length %d or more;\n", dict, dictionaryList->numWords, MIN_WORD_LENGTH);
    }




    if (randMode) {
        printf("==== SET HIVE: RANDOM MODE ====\n");
        //find number of words in words array that use hiveSize unique letters
        WordList* fitWords = findAllFitWords(dictionaryList, hiveSize);
        int numFitWords = fitWords->numWords;
        //pick one at random
        int pickOne = rand() % numFitWords;
        char* chosenFitWord = fitWords->words[pickOne];

        //and alaphabetize the unique letters to make the letter hive
        buildHive(chosenFitWord, hive);
        freeWordList(fitWords);

        reqLetInd = rand() % hiveSize;
        reqLet = hive[reqLetInd];

    }
    else {
        printf("==== SET HIVE: USER MODE ====\n");
        char temp[100];
        char reqInput[10];
        bool valid = false;

        //---------------------------------------------------------------------
        /* TODO (Task 2-C): Get hive input (part 1 of 2)
         - Write code for randMode OFF, where user enters the hive; use the print statements below in the exact order you see them
         - ask the user to enter a string, following these requirements:
             - must be lowercase letters
             - string length must not be below MIN_HIVE_SIZE or above MAX_HIVE_SIZE
             - must not have any duplicate letters (you might find a helper function useful for this part)
         - If a word is invalid, display the appropriate error below and repeat until there is valid input
         - You may use buildHive() to help with this task
         (Task 2-C is continued a few lines below, getting the required letter)
        */ 
        // Asks the user for the input until it is valid
        while(!valid) { 
            printf("  Enter a single string of lower-case,\n  unique letters for the letter hive... ");
            scanf("%s", temp);
            int length = strlen(temp);
            valid = true;
            // Checks if the length is within the min and max hive size 
            if(length < MIN_HIVE_SIZE || length > MAX_HIVE_SIZE) { 
                printf("  HIVE ERROR: \"%s\" has invalid length;\n  valid hive size is between %d and %d, inclusive\n\n",temp, MIN_HIVE_SIZE, MAX_HIVE_SIZE);
                valid = false;
                continue;
            }  
            // Checks that the letters are in lowercase and if not, it prints the error
            for(int i = 0; i < length && valid; i++) {
                if(!islower(temp[i])) {
                    printf("  HIVE ERROR: \"%s\" contains invalid letters;\n  valid characters are lower-case alpha only\n\n",temp);
                    valid = false;
                    break;
                }
            }
            // Checks for duplicate letters and if there are duplicates, it prints an error message
            for(int i = 0; i < length && valid; i++) { 
                for(int j = i + 1; j < length; j++) {
                    if(temp[i] == temp[j]) {
                        printf("  HIVE ERROR: \"%s\" contains duplicate letters\n\n",temp);
                        valid = false;
                        break;
                    }
                }
            }  
            // Checks if it is valid
            if(valid) {
                hive[0] = '\0';
                buildHive(temp, hive);
                hiveSize = strlen(hive);
            } 
        }
        //---------------------------------------------------------------------
        /* TODO (Task 2-C): Get hive input (part 2 of 2)
        - Ask the user to enter the required letter, using the print statement above
        - If the letter is not in the hive, display the error message below and repeat the prompt until input is valid
        - If the letter the user enters is within the hive, then find the index of that letter in the hive,
        set that value you found for reqLetInd, and change reqLet to the required letter
        - Note that uppercase letters ARE valid here, unlike earlier; you will find it helpful to make them lowercase in code
        */
        // Prompts until a valid letter is entered
        while(1) {
            printf("  Enter the letter from \"%s\"\n   that is required for all words: ", hive);
            scanf("%s", reqInput);
            reqLet = tolower(reqInput[0]);
            reqLetInd = findLetter(hive, reqLet);
            // If the letter exists, exits the loop
            if(reqLetInd != -1) {
                break;
            }
            printf("  HIVE ERROR: \"%s\" does not contain the character \'%c\'\n\n",hive,reqLet); 
        }
    }

    printHive(hive, reqLetInd);

    if (playMode) {
        printf("==== PLAY MODE ====\n");

        char* userWord = (char*)malloc((maxWordLength + 1) * sizeof(char));
        strcpy(userWord, "default");

        WordList* userWordList = createWordList();

        printf("............................................\n");
        printHive(hive, reqLetInd);


        printf("  Enter a word (enter DONE to quit): ");
        scanf("%s", userWord);
        printf("\n");


        while (strcmp(userWord, "DONE")) {

            //---------------------------------------------------------------------
            /* TODO (Task 4-A, part 2 of 2): Validate input from the user (for play mode)
            - userWord is the input from the user (their attempted word)
            - If the word is not valid (it's too short, not in the dictionary, not valid for the hive), 
                or it has already been entered, display the appropriate error message below.
            - If the word is valid, add it to userWordList
            
            - The end of the while loop is already given to you, make sure not to remove that
            - As always, helper functions will benefit you greatly here
            */

            // Initializes the score
            int totalscore = 0;
            bool duplicateWord = false;
            // Checks if the word has been entered
            for(int i = 0; i < userWordList->numWords; i++) { 
                // Compares the stored word with the new word
                if(strcmp(userWordList->words[i], userWord) == 0) { 
                    // If it is a duplicate, then store it as a duplicate
                    duplicateWord = true;
                    break;
                }
            } 
            // Checks if the word is in the dictionary
            bool dictionary = false;
            // Searches through to see if the word is valid
            for(int i = 0; i < dictionaryList->numWords; i++) { 
                // Compares the stored word with the new word
                if(strcmp(dictionaryList->words[i], userWord) == 0) {
                    // If it is found, then return true
                    dictionary = true;
                    break;
                }
            }

            // Checks if the word is shorter than min length
            if(strlen(userWord) < MIN_WORD_LENGTH) {
                printf(" !!! \"%s\" is TOO SHORT (min length = %d) !!!\n", userWord, MIN_WORD_LENGTH);
              // Prints error if the word is not in the dictionary
            } else if(!dictionary) { 
                printf(" !!! \"%s\" is NOT a valid word in the dictionary !!!\n", userWord);
              // Prints error if the word is already in the list
            } else if(duplicateWord) { 
                printf(" !!! \"%s\" is already in your word list !!!\n", userWord);
              // Prints the word if it is not valid
            } else if(!isValidWord(userWord, hive, reqLet)) { 
                printf(" !!! \"%s\" is NOT valid for the hive !!!\n", userWord);
            } else { 
                // Adds the word to the list if it is valid
                appendWord(userWordList, userWord);
                totalscore += strlen(userWord);

            }
            
            
            // Keep the code below this comment, which prints the list and the hive, and gets the next input
            printf("\n");
            printList(userWordList, hive);
            printf("............................................\n");
            printHive(hive, reqLetInd);

            printf("  Enter a word (enter DONE to quit): ");
            scanf("%s", userWord);
            printf("\n");

        }

        // Do until user enters quit request
        //  Display Hive & Word List & Current Score
        //  X Allow user to enter a word
        //  X Check if word is in dict
        //  X Check if word is a duplicate
        //  Check if word is valid for the hive
        //  If all valid, add it to word list, report score  

        freeWordList(userWordList);
        free(userWord);
    }

    printf("==== SPELLING BEE SOLVER ====\n");

    printf("  Valid words from hive \"%s\":\n", hive);
    printf("                         ");
    for (int i = 0; i < reqLetInd; i++) {
        printf(" ");
    }
    printf("^\n");

    WordList* solvedList = createWordList();

    if (bruteForce) { //find all words that work... (1) brute force
        bruteForceSolve(dictionaryList, solvedList, hive, reqLet);
    }
    else {
        char* tryWord = (char*)malloc(sizeof(char) * (maxWordLength + 1));

        tryWord[0] = hive[0];
        tryWord[1] = '\0';
        findAllMatches(dictionaryList, solvedList, tryWord, hive, reqLet);
        free(tryWord);

    }

    int longestSolvedWordLen = 0;
    for (int i = 0; i < solvedList->numWords; i++) {
        if (strlen(solvedList->words[i]) > longestSolvedWordLen) {
            longestSolvedWordLen = strlen(solvedList->words[i]);
        }
    }

    // Helpful variables
    int numValidWords = 0;
    int numPangrams = 0;
    int numPerfectPangrams = 0;
    int totScore = 0;
    int score = 0;
    bool isBingo = true;

    //---------------------------------------------------------------------
    /* TODO (Task 5-B): Display solver results
    - Display list of words solver found, noting pangrams and perfect pangrams with asterisks like printList
    - Calculate the number of valid words, number of pangrams, number of perfect pangrams, 
          total possible score, and whether there is a bingo. Save those values in the corresponding
          variables, or update the printf statements below to dispay those values
    */


    int length = strlen(hive);
    // Checks every valid word in solvedList
    for(int i = 0; i < solvedList->numWords; i++) { 
        char* word = solvedList->words[i];
        int wordlength = strlen(word);
        // 4 letter words are 1 point otherwise the score is wordlength
        int points = (wordlength == 4) ? 1: wordlength;
        // Checks if the word is a pangram
        bool pangram = isPangram(word, hive); 
        // Checks if the word is a perfect pangram
        bool perfectpangram = pangram && (wordlength == length);
        // If it is a pangram, add the length to the points and increment numpangrams by 1
        if(pangram) { 
            points += length;
            numPangrams++; 
        }
        // If it is a perfect pangram, increment numPerfectPangrams by 1
        if(perfectpangram) { 
            numPerfectPangrams++;
        }  
        // Add points to total score and increment numValidWords
        totScore += points;
        numValidWords++;
        // If it is a perfect, print out these statements
        if(perfectpangram) { 
            printf("  *** (%2d) %s\n", points, word);
        } else if(pangram) { 
            printf("   *  (%2d) %s\n", points, word);    
        } else { 
            printf("      (%2d) %s\n", points, word);
        }
    }  
    // Checks if it is a bingo
    for(int i = 0; i < length; i++) { 
        char hivecharacter = hive[i];
        bool found = false;
        // Searches through the words
        for(int j = 0; j < solvedList->numWords; j++) { 
            // Checks if the word starts with current letter
            if(solvedList->words[j][0] == hivecharacter) { 
                found = true;
                break;
            }
        }  
        // If no word starts with the hive letter, bingo is false 
        if(!found) { 
            isBingo = false;
            break;
        }
    }
    


    
    // Additional results are printed here:
    printf("\n");
    printf("  Total counts for hive \"%s\":\n", hive);
    printf("                         ");
    for (int i = 0; i < reqLetInd; i++) {
        printf(" ");
    }
    printf("^\n");
    printf("    Number of Valid Words: %d\n", numValidWords);
    printf("    Number of ( * ) Pangrams: %d\n", numPangrams);
    printf("    Number of (***) Perfect Pangrams: %d\n", numPerfectPangrams);
    printf("    Bingo: ");
    printYESorNO(isBingo);
    printf("    Total Score Possible: %d\n", totScore);

    //---------------------------------------------------------------------
    /* TODO Extra credit: Display frequency table
    - Display a table showing the number of words starting with each letter and duration in a table.
    - For example, with the hive abcde and required letter e:
             Frequency Table:
             let  4  5  6  7  8
             -------------------
              a:  3  1  1  1  0
              b:  3  1  4  0  0
              c:  3  1  0  0  1
              d:  3  0  3  0  0
              e:  0  1  0  0  0
       The table shows that there are 3 words that start with a and are 4 letters long, 
       and 1 each that are 5, 6, and 7 letters long and start with a.
       Note that the lengths 4-8 are shown here because the longest word is 8 letters long,
       but that will change depending on the hive
    */


    freeWordList(dictionaryList);
    freeWordList(solvedList);
    printf("\n\n");
    return 0;
}

