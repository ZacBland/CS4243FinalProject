// Assignment 01
// Michael Hammerschmidt
// 11/21/2022
// This code reads from a specified file passed in as a param, then saves it into an array within a struct.

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include "readFile.h"

#define CHARUPPERBOUND 10000

// struct catalogStruct
// {
//     int sizeOfRow;
//     int sizeOfCol;
//     char recordArray[1000][7][100];
// };

struct catalogStruct readFile(char *filename)
{

    struct catalogStruct filesData;

    FILE *file;
    char row[CHARUPPERBOUND];
    char *tokenized;

    file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("%s", "Error: The file is Null\n");
        return filesData;
    }

    int countOfRows = 0;
    int colCount = 0;
    while (!(feof(file) == true))
    {
        fgets(row, CHARUPPERBOUND, file);

        // removes extra spaces in the row that is being read in.
        int r, j, length;
        length = strlen(row);
        for (r = 0; r < length; r++)
        {
            if (row[0] == ' ')
            {
                for (r = 0; r < (length - 1); r++)
                    row[r] = row[r + 1];
                row[r] = '\0';
                length--;
                r = -1;
                continue;
            }
            if (row[r] == ' ' && row[r + 1] == '	')
            {
                for (j = r; j < (length - 1); j++)
                {
                    row[j] = row[j + 1];
                }
                row[j] = '\0';
                length--;
                r--;
            }
        }

        // Replaces any occurrence of a space between quotations with an underscore
        for (int i = 0; i < strlen(row); i++)
        {
            if (row[i] == '"')
            {
                for (int r = i + 1; r < strlen(row); r++)
                {
                    i++;
                    if (row[r] == '	' || row[r] == '	')
                    {
                        row[r] = '_';
                    }
                    else if (row[r] == '"')
                    {
                        break;
                    }
                }
            }
        }

        // delineate the rows to be separated by spaces
        char *ptrToRow = NULL;
        tokenized = strtok_r(row, "	", &ptrToRow);

        colCount = 0;
        while (tokenized != NULL)
        {
            // Saves each column within the array in filesData struct instance
            strcpy(filesData.recordArray[countOfRows][colCount], tokenized);

            tokenized = strtok_r(NULL, "	", &ptrToRow);
            colCount++;
        }

        countOfRows++;
    }

    filesData.sizeOfCol = 7;
    filesData.sizeOfRow = countOfRows;

    return filesData;
}

void display(char *printToClient)
{
    // Testing code
    printf("%s", printToClient);
    // Use send code when implemented in server, and replace newSocket with the name of the socket.
    // send(newSocket, printToClient, strlen(printToClient), 0);
}

int main(int argc, char *argv[])
{

    bool end = false;
    char userInput[256];

    // Read file
    char filename[] = "cartier_catalog.txt";
    char *printToClient;
    struct catalogStruct filesData = readFile(filename);

    while (end == false)
    {
        printToClient = "\nEnter the number of the option to select it\n1. Looking at the jewelry menu\n2. Making specific jewelry inquiry\n3. Making purchase\n4. Returning the purchase\n5. Exit\n\n";
        display(printToClient);

        fgets(userInput, 256, stdin);
        char temp[100];
        strcpy(temp, userInput);
        userInput[strcspn(userInput, "\n")] = 0;

        if ((strncmp(userInput, "exit", 3) == 0) || (strncmp(userInput, "Exit", 3) == 0) || (strncmp(userInput, "5", 1) == 0))
        {
            printToClient = "\nExiting program...";
            display(printToClient);
            end = true;
        }
        else if ((strcmp(userInput, "1") == 0))
        {
            printToClient = "\nPrinting jewelry menu:\n";
            display(printToClient);

            // Display the contents of catalog struct
            for (int i = 1; i < filesData.sizeOfRow; i++)
            {
                for (int j = 0; j < filesData.sizeOfCol; j++)
                {
                    // TODO: call display function for each row to be displayed
//                   if(j == 0){
//                     char message1[100];
//                     strcpy(message1, filesData.recordArray[0][j]);

//                     strcat(temp, message1);

//                     printf("%s", temp); // send(newSocket, temp, strlen(temp), 0);
//                   }

                    char message[100];
                    strcpy(message, filesData.recordArray[i][j]);
                    strcat(message, "   ");

                    if(j != 4 && j != 5){
                      printf("%s", message); //send(newSocket, message2, strlen(message2), 0);
                    }
                }
              printf("\n");
            }
        }
        else if ((strcmp(userInput, "2") == 0))
        {
            printToClient = "\nEnter a ref to make an inquiry:\n";
            display(printToClient);
            // TODO: add Nick and Harika's logic to print out the details of the ref selected
            char userRef[256];
            fgets(userRef, 256, stdin);
            char temp[100];
            strcpy(temp, userRef);
            userInput[strcspn(userRef, "\n")] = 0;
            //Print out input
            for (int i = 1; i < filesData.sizeOfRow; i++)
            {
                for (int j = 0; j < filesData.sizeOfCol; j++)
                {
                  if(strncmp(filesData.recordArray[i][0], userRef, strlen(userRef)) == 0){
                    //stuff
                  }
                }
            }
        }
        else if ((strcmp(userInput, "3") == 0))
        {
          
        }
        else if ((strcmp(userInput, "4") == 0))
        {
          
        }
        else
        {
            printf("\nInvlaid input. Please try again.\n");
        }
    }

    return 0;
}
