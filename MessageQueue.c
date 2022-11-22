#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <mqueue.h>
#include <sys/stat.h>
int main() {

    return 0;
}

void receive(char *input_queue, char *data) {

    mqd_t queue = mq_open(input_queue, O_RDONLY);
    if((mq_receive(queue, (char *)data, sizeof(data), NULL)) == 1) {
        perror("Failed to read from queue");
    }

    // Test data
    int rows = 5;
    int columns = 100;
    char buffer[254];

    for(int i = 0; i < rows; i++) 
    {
        for(int j = 0; j < columns; j++)
        {
            mq_receive(queue, (char*) &buffer, sizeof(buffer), NULL);
            // Future copy from file data
        }
    }
}

void send(char *input_queue, char *data, size_t size) {

    mqd_t queue = mq_open(input_queue, O_CREAT | O_WRONLY, 0600, NULL);
    mq_send(queue, data, size, 1);

    // Temporary testing data
    int rows = 5;
    int columns = 100;
    char file_data[rows][columns];
    char buffer[254];
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < columns; j++)
        {
            // Copy file data into buffer here
            mq_send(queue, (char *)buffer, sizeof(buffer), 1);
        }   
    }
    mq_close(queue);
    mq_unlink(input_queue);

}