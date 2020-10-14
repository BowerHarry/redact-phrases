#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

#pragma warning (disable : 4996)

// This function returns the length of a string.
int mystrlen(char* str) {
    int count = 0;
    // Counts the number of characters before reaching the terminating 0 of a string.
    for (int i = 0; str[i] != 0; i++) {
        count++;
    }
    return count;
}

// This function copies a string onto the heap.
char* mystrcpy(char* s) {
    int len = mystrlen(s);
    // Allocate space on the heap for the string.
    char* copy = calloc(len + 1, 1);
    // Fill this space with the string.
    for (int i = 0; i < len; i++)
    {
        copy[i] = s[i];
    }
    // Add the terminating 0 to the end of the string.
    copy[len] = 0;
    return copy;
}


// This function returns true if two strings are equivalent.
bool equalWord(char* s1, char* s2) {

    if (mystrlen(s1) != (mystrlen(s2))) {
        return false;
    }
    // For every character...
    for (int i = 0; i < mystrlen(s1); i++) {
        // If they are not equal return false.
        if (s1[i] != s2[i]) {
            return false;
        }
    }
    // If every character is equal, return true.
    return true;
}

// This function replaces every character in a string with a '*'.
char* redactWord(char* word) {
    // Allocate space for a string that is the same size as the word.
    char* redacted = calloc(sizeof(char), (mystrlen(word) + 1));
    // For every character in the word, add a '*' to the redacted word.
    for (int i = 0; i < mystrlen(word); i++) {
        redacted[i] = '*';
    }
    return redacted;
}

// This function returns true if the word passed to it is in the list of words to redact.
bool shouldRedact(char* word, char* path) {
    // Open the file containing words.
    FILE* fp = fopen(path, "r");
    // Check if the file is empty / not found.
    if (fp == NULL) {
        printf("Input file not found\n");
        return;
    }
    // Max size of file.
    const int MaxSize = 100;
    // Max number of words.
    const int MaxRedact = 10;
    // Allocate a buffer large enough to handle the file.
    char* buffer = calloc(MaxSize, sizeof(char));
    // Read file.
    int len = fread(buffer, sizeof(char), MaxSize, fp);
    buffer[len] = 0;
    fclose(fp);
    // Space for 10 pointers
    char** redactWords = calloc(MaxRedact, sizeof(char*));
    // Replace any spaces with new lines.
    for (int i = 0; i < mystrlen(buffer); i++) {
        if (buffer[i] == ' ') {
            buffer[i] = '\n';
        }
    }
    // Split the words at new lines.
    char* each = strtok(buffer, "\n");
    int redactCount = 0;
    while (each != NULL) {
        // Add each word to the list of word.
        if (strlen(each) > 0) {
            redactWords[redactCount++] = each;
        }
        each = strtok(NULL, "\n");
    }
    // For every word in the list check to see if it is equal...
    for (int j = 0; j < redactCount; j++) {
        if (equalWord(word, redactWords[j])) {
            free (buffer);
            free (redactWords);
            return true;
        }
    }
    free(buffer);
    free(redactWords);
    return false;
}

// This function imports the text from a given path.
char* importText(char* path) {
    // Open the file containing text.
    FILE* fp = fopen(path, "r");
    // Check if the file is empty / not found.
    if (fp == NULL) {
        printf("Input file not found\n");
        return;
    }
    // Max size of file.
    const int MaxSize = 100000;
    // Allocate a buffer large enough to handle the file.
    char* text = calloc(MaxSize, sizeof(char));
    // Read file.
    int len = fread(text, sizeof(char), MaxSize, fp);
    text[len] = 0;
    fclose(fp);
    char* sentence = mystrcpy(text);
    return sentence;
}

// This function returns true if the given character is only 1 byte.
bool isSingleByteChar(char ch) {
    return ch > 0 && ch < 128;
}

int main() {
    // Text path.
    char* textPath = "";
    // Redactable words path.
    char* redactPath = "";
    // Import the text from file.
    char* text = importText(textPath);
    int len = mystrlen(text);
    // Allocate space for the text with redactions.
    char* newText = calloc(sizeof(char), len);
    // Allocate space for the max length of a word in the text.
    char* word = calloc(15, 1);
    int count = 0;
    // For every character in the text...
    for (int i = 0; i < len; i++) {
        char ch = text[i];
        // If it is a letter, start building a word.
        if (isSingleByteChar(ch) && isalpha(ch)) {
            word[count] = ch;
            count++;
        }
        // If it isn't a letter, add the word to the new text.
        else {
            // Check if the word needs redacting.
            if (shouldRedact(word, redactPath)) {
                // Add the redacted word to the new text.
                strcat(newText, mystrcpy(redactWord(word)));
            }
            else {
                // Add the word to the new text.
                strcat(newText, mystrcpy(word));
            }
            // Add the character that ended the word to the new text.
            newText[i] = ch;
            // Empty the contents of word.
            memset(&word[0], 0, 15);
            count = 0;
        }
    }

    printf("%s", newText);

    free(text);
    free(word);
    free (newText);
}
