#include<stdlib.h>
#include<stdio.h>
#include<string.h>  
#include<unistd.h>
#include<sys/types.h>  
#include<sys/ipc.h>  
#include<sys/msg.h>
#include "file_options.h"
#define max_text_length 1024   //maximum length of the data that has to be sent over the message queue

//structure defined for message queue 
struct msg_bfr{  
        long int msg_text_type;  
        char msg_text[max_text_length];  
};  

//function defined to send the data across the message queue
void sendDataOverMsgQueues(file_options data, int key)
{
	
        int msg_id;  
        struct msg_bfr msg_que; 

        // msgget() function used to create a message queue and returns the identifier
        msg_id = msgget(key, 0666 | IPC_CREAT);

        if (msg_id == -1) // -1 value indicates that the message queue is not created  
        {  
                printf("Error occured while creating message queue!!\n");  
                exit(0);  
        }  

        msg_que.msg_text_type=1;  

        //msg_que.msg_text = data;

        char buffer[1024];
        for(int r = 0; r < data.num_rows; r++)
        {
                for(int c = 0; c < data.num_columns; c++)
                {
                        strcpy(msg_que.msg_text, data.data[r][c]);
                        if(msgsnd(msg_id,&msg_que, sizeof(msg_que), 0) == -1)
                        {
                                printf("Message is not sent.\n");  
                        }
                }
        }

        // //msgrcv() function used to send the data over the message queue
        // if(msgsnd(msg_id,&msg_que, sizeof(msg_que),0)==-1) // -1 value indicates that the message queue is not sent  
        // {  
        //         printf("Message is not sent.\n");  
        // }  

}
