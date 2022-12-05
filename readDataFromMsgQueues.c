#include<stdlib.h>
#include<stdio.h> 
#include<string.h>  
#include<unistd.h>
#include<sys/types.h>  
#include<sys/ipc.h>  
#include<sys/msg.h>
#include "file_options.h"
#define max_text_length 1024  //maximum length of the data that has to be sent over the message queue

//buffer structure defined for the message queue 
struct msg_bfr{  
        long int msg_text_type;  
        char msg_text[max_text_length];  
};  

//function defined to read the value to be tracked by the process
char*** readDataFromMsgQueues(int key)
{
        
        int msg_id;  
        struct msg_bfr msg_que; 
        char* msg_str;
        file_options file_data;

        // msgget() function used to create a message queue and returns the identifier
        msg_id = msgget(key, 0666 | IPC_CREAT);

        if (msg_id == -1) // -1 value indicates that the message queue is not created  
        {  
                printf("Error occured while creating message queue!!\n");  
                exit(0);  
        }  

        //msgrcv() function used to receive the data from the message queue
        msg_str = (char*)msgrcv(msg_id, (char *)&file_data, sizeof(msg_que), 1, 0);

        char ***file = calloc(file_data.num_rows, sizeof(char**));
        for(int r = 0; r < file_data.num_rows; r++)
        {
                file[r] = calloc(file_data.num_columns, sizeof(char*));
        }

        char buffer[1024];
        for(int r = 0; r < file_data.num_rows; r++)
        {
                for(int c = 0; c < file_data.num_columns; c++)
                {
                        if(msgrcv(msg_id, &buffer, sizeof(buffer), 1, 0) == -1)
                        {
                                printf("MQ failed to receive\n");
                        }
                        file[r][c] = calloc(strlen(buffer) + 1, sizeof(char));
                        strcpy(file[r][c], buffer);
                }
        }

        //msgctl() function used to destroy the message queue
        msgctl(msg_id, IPC_RMID, NULL);

        return file;
}

int main() {
        return 0;
}