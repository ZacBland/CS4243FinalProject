// Group: Group A
// Author: Nick Schirloff
// Email: nick.schirloff@okstate.edu
// Date: 12/05/22
// Description: Receives optional refID from user, and either displays information, displays a specific item,
//              purchases an item and removes one from its quantity, or returns an item, adding one to its quantitiy.
//              Output pipe is written to based off the required input, whether a full string of the items in the catalog,
//              the item, or its value.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int BUFFER_SIZE = 1024;

/*
    @Param
        ref: Ref ID of the item to search for (e.g. "B4084600" for the first item)
        input_pipe[2]: Pipe that listens for input
        output_pipe[2]: Pipe that returns output
        rows: Number of rows in the array
        columns: Number of columns in the array
        file_data: 2D array containing the file information
*/
void piping(char *ref, int input_pipe[2], int output_pipe[2], int rows, int columns, char ***file_data) {

    int rw_result; // Read/write result
    char buffer[BUFFER_SIZE];
    char item_buffer[BUFFER_SIZE];
    char temp_buffer[BUFFER_SIZE];
    char *ret_ptr = buffer; // return pointer
    int ret_val; // return value from assigning to pointer

    // Begin listening to pipe
    while(1) {
        // Wait until pipe is populated
        while((rw_result = read(input_pipe[0], buffer, BUFFER_SIZE)) == 0) {
            if(rw_result < 1) {
                perror("Error reading from pipe");
            }
            sleep(1);
        }

        printf("Command read from pipe: %s\n", buffer);
        // Error if nothing is read
        if(rw_result < 1) {
            perror("Error reading from pipe");
        }

        /*
            TODO: Get fixed buffer size of appropriate size
        */

        // rw = row, cl = column
        // Option 1 - Display All Items
        if(strcmp(buffer, "display") == 0)
        {
            for(int rw = 0; rw < rows; rw++)
            {
                for(int cl = 0; cl < columns; cl++)
                {
                    // The assignment specifies that general display does not include description or quantity, so we skip those two columns here
                    if((cl == 4) || (cl == 5))
                    {
                        continue;
                    }
                    ret_val = sprintf(ret_ptr, "%s%5s", file_data[rw][cl], " ");
                }
            }
            ret_val = sprintf(ret_ptr, "\n");
            rw_result = write(output_pipe[1], buffer, BUFFER_SIZE);
        }

        // Option 2 - View Specific Item
        if(strcmp(buffer, "specific") == 0)
        {
            for(int rw = 0; rw < rows; rw++)
            {
                if(strcmp(file_data[rw][0], ref) == 0)
                {
                    for(int cl = 0; cl < columns; cl++)
                    {
                        ret_val = sprintf(ret_ptr, "%s%5s", file_data[rw][cl], " ");
                    }
                    break;
                }
            }
            ret_val = sprintf(ret_ptr, "\n");
            rw_result = write(output_pipe[1], buffer, BUFFER_SIZE);
        }

        // Option 3 - Purchase
        if(strcmp(buffer, "purchase") == 0)
        {
            for(int rw = 0; rw < rows; rw++)
            {
                if(strcmp(file_data[rw][0], ref) == 0)
                {
                    // Getting int value of 
                    int count = (atoi(file_data[rw][6])) - 1;
                    char *temp;
                    sprintf(temp, "%d%5s", count, " ");
                    strcpy(file_data[rw][5], temp);
                    strcpy(buffer, file_data[rw][6]);
                    break;
                }
            }
            rw_result = write(output_pipe[1], buffer, BUFFER_SIZE);
        }
        
        // Option 4 - Return
        if(strcmp(buffer, "return") == 0)
        {
            for(int rw = 0; rw < rows; rw++)
            {
                if(strcmp(file_data[rw][0], ref) == 0)
                {
                    // Getting int value of 
                    int count = (atoi(file_data[rw][6])) + 1;
                    char *temp;
                    sprintf(temp, "%d%5s", count, " ");
                    strcpy(file_data[rw][5], temp);
                    strcpy(buffer, file_data[rw][6]);
                    break;
                }
            }
            rw_result = write(output_pipe[1], buffer, BUFFER_SIZE);
        }
    }
    printf("Piping finished.\n");
}

// int main() {


//     printf("Ran successfully.\n");
//     return 0;
// }
