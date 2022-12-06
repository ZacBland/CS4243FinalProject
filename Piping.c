// Group: Group A
// Author: Nick Schirloff
// Email: nick.schirloff@okstate.edu
// Date: 12/5/22
// Description: Receives optional refID from user, and either displays information, displays a specific item,
//              purchases an item and removes one from its quantity, or returns an item, adding one to its quantitiy.
//              Output pipe is written to based off the required input, whether a full string of the items in the catalog,
//              the item, or its value.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "readFile.h"

int BUFFER_SIZE = 1024;

struct return_data {
    char catalog_info[1000][7][500];
    char specific_item[1024];
    int purchase_price;
    int number_after_return;
};


/*
    @Param
        ref: Ref ID of the item to search for (e.g. "B4084600" for the first item)
        input_pipe[2]: Pipe that listens for input
        output_pipe[2]: Pipe that returns output
        rows: Number of rows in the array
        columns: Number of columns in the array
        file_data: 2D array containing the file information
*/
void piping(char *ref, int input_pipe[2], int output_pipe[2], int rows, int columns, char file_data[1000][7][500]) {

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

        // rw = row, cl = column
        // Option 1 - Display All Items
        if(strcmp(buffer, "display") == 0)
        {
            struct return_data data;
            for(int rw = 1; rw < rows; rw++)
            {
                for(int cl = 0; cl < columns; cl++)
                {
                    // The assignment specifies that general display does not include description or quantity, so we skip those two columns here
                    if((cl == 4) || (cl == 5))
                    {
                        continue;
                    }
                    strcpy(file_data[rw][cl],data.catalog_info[rw][cl]);
                    //ret_val = sprintf(ret_ptr, "%s%5s", file_data[rw][cl], " ");
                }
            }
            //ret_val = sprintf(ret_ptr, "\n");
            rw_result = write(output_pipe[1], &data, sizeof(data));
        }

        // Option 2 - View Specific Item
        if(strcmp(buffer, "specific") == 0)
        {
            for(int rw = 1; rw < rows; rw++)
            {
                printf("COMPARING: %s : %s\n", file_data[rw][0], ref);
                printf("RW: %i\n", rw);
                if(strcmp(file_data[rw][0], ref) == 0)
                {
                    for(int cl = 0; cl < columns; cl++)
                    {
                        ret_val = sprintf(ret_ptr, "%s%5s", file_data[rw][cl], " ");
                        printf("Ret-val return: %s\n", ret_ptr);
                    }
                    break;
                }
            }
            ret_val = sprintf(ret_ptr, "\n");
            struct return_data data;
            strcpy(ret_ptr, data.specific_item);
            printf("WRITING: %s\n", data.specific_item);
            rw_result = write(output_pipe[1], &data, sizeof(data));
        }
    

        // Option 3 - Purchase
        if(strcmp(buffer, "purchase") == 0)
        {
            struct return_data data;
            for(int rw = 1; rw < rows; rw++)
            {
                if(strcmp(file_data[rw][0], ref) == 0)
                {
                    // Getting int value of 
                    int count = (atoi(file_data[rw][5])) - 1;
                    if(count <= 0)
                    {
                        strcpy(buffer, "Out of stock");
                        data.purchase_price = 0;                        
                    } else {
                        char *temp;
                        sprintf(temp, "%d%5s", count, " ");
                        strcpy(file_data[rw][5], temp);
                        data.purchase_price = atoi(file_data[rw][6]);
                        //strcpy(buffer, file_data[rw][6]);
                    }
                    break;
                }
            }
            rw_result = write(output_pipe[1], &data, sizeof(data));
        }
        
    //     // Option 4 - Return
        if(strcmp(buffer, "return") == 0)
        {
            struct return_data data;
            for(int rw = 1; rw < rows; rw++)
            {
                if(strcmp(file_data[rw][0], ref) == 0)
                {
                    // Getting int value of 
                    int count = (atoi(file_data[rw][6])) + 1;
                    char *temp;
                    sprintf(temp, "%d%5s", count, " ");
                    strcpy(file_data[rw][5], temp);
                    data.number_after_return = count;
                    //strcpy(buffer, file_data[rw][6]);
                    break;
                }
            }
            rw_result = write(output_pipe[1], &data, sizeof(data));
        }
    }
    printf("Piping finished.\n");
}

// int main() {


//     printf("Ran successfully.\n");
//     return 0;
// }
