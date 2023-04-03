#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main (int argc, char* argv[]) {
    // Error if only 1 (executable only) or more than 2 arguments are passed
    // through the command line
    if (argc == 1 || argc > 2) {
        printf("Error: Expected one argument, found %d:", (argc - 1));

        for (int i = 1; i < argc; i++) {
            printf(" \"%s\"", argv[i]);
        }
        printf("\n");

        return -1;
    }

    int fd = open(argv[1], O_RDONLY);
    
    // Error: Either the filename given does not exist or the file cannot
    // be opened in read mode
    if (fd == -1) {
        printf("Error: %s\n", strerror(errno));

        return -1;
    }

    char buffer[1];

    int numLines = 0;
    int numWords = 0;
    int numChar = 0;

    // Check for error in case the file opened was a directory and not a txt file
    // or check if the file is empty.
    int firstRead = read(fd, buffer, 1);
    if ( firstRead == -1 ) {
        printf("Error: %s\n", strerror(errno));

        return -1;
    } else if ( firstRead == 0 ) {
        printf("%d %d %d %s\n", numLines, numWords, numChar, argv[1]);

        return 0;
    }

    char prevChar = *buffer;
    ++numChar;
    // Check if prevChar a new line character;
    // if prevChar is not a new line character, check if it is a whitespace character.
    if (prevChar == '\n') {
        ++numLines;
    } else if ( !isspace(prevChar) ) {
        ++numWords;
    }

    while ( read(fd, buffer, 1) ) {
        ++numChar;

        if ( isspace(prevChar) && !isspace(*buffer) ) {
            ++numWords;
        } else if (*buffer == '\n') {
            ++numLines;
        }

        prevChar = *buffer;
    }

    // Check how many new line characters you have counted
    if (numLines == 0) {
        // The file will have at least one line. Therefore, increment numLines.
        ++numLines;
    } else if (prevChar != '\n') {
        // Check if the line following the last new line character in the file
        // is empty. If it is not (meaning prevChar != '\n'), increment numLines.
        ++numLines;
    }

    printf("%d %d %d %s\n", numLines, numWords, numChar, argv[1]);

    close(fd);

    return 0;
}