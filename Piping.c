// Group: Group A
// Author: Nick Schirloff
// Email: nick.schirloff@okstate.edu
// Date: 11/11/22
// Description: Groundwork on piping between processes. Needs to implement file information processing/storing.
//              Takes reference id, and command to conduct on id. Finds the id within a given file/string array,
//              and operates on the item based on the command.
//  gcc Piping.c -o ./a.out
//  a.out
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
                    ret_val = sprintf(ret_ptr, "%s%5s", &file_data[rw][cl], " ");
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
                if(strcmp(&file_data[rw][0], ref) == 0)
                {
                    for(int cl = 0; cl < columns; cl++)
                    {
                        ret_val = sprintf(ret_ptr, "%s%5s", &file_data[rw][cl], " ");
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
                if(strcmp(&file_data[rw][0], ref) == 0)
                {
                    // Getting int value of 
                    int count = (atoi(&file_data[rw][6])) - 1;
                    char *temp;
                    sprintf(temp, "%d%5s", count, " ");
                    strcpy(&file_data[rw][6], temp);
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
                if(strcmp(&file_data[rw][0], ref) == 0)
                {
                    // Getting int value of 
                    int count = (atoi(&file_data[rw][6])) + 1;
                    char *temp;
                    sprintf(temp, "%d%5s", count, " ");
                    strcpy(&file_data[rw][6], temp);
                    break;
                }
            }
            rw_result = write(output_pipe[1], buffer, BUFFER_SIZE);
        }
    }
    printf("Piping finished.\n");
}

int main() {

    // Temp, test data
    // char *data[] = {
    //     "B4084600",	"rings","Love ring	yellow gold", "Love ring, 18K yellow gold. Width: 5.5mm.",	"2","1650"
    // };
    // char *command = "view";

    // int ip[2];
    // int op[2];
    // pipe(ip);
    // pipe(op);
    // char buffer[254];
    // if(fork() == 0) {
    //     close(op[1]);
    //     write(ip[1], command, sizeof(command));
    //     piping("B4084600", ip, op, 6, 150, data);
    //     //write(ip[0], command, sizeof(command));
    //     printf("Wrote successfully\n");
    //     wait(NULL);
    // } else {
    //     exit(0);

    // }

    /*
        Thoughts for forking:
        - Keeping track of queue names
        - Maybe passing client ID
        char unique_name[100] = "";
        char temp_buffer[100];
        sprintf(temp_buffer, "%d", clientID);
        strcat(unique_name, "Queue#");
        strcat(unique_name, temp_buffer);


    
    
    
    */


    printf("Ran successfully.\n");
    return 0;
}