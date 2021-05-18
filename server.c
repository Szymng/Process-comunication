#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdlib.h>

#define MAXSIZE 80

struct Message
{
    long mtype;
    int id_klienta;
    char text[MAXSIZE];
};

int main(int argc, char *argv[])
{
    key_t key;
    int msgqid;
    struct Message message;
    long type;

    key = ftok(".", 'c');

    if ((msgqid = msgget(key, 0666|IPC_CREAT)) == -1)
    {
        perror("msgget");
        exit(1);
    }

    if(argc < 2)
    {
        printf("Correct execute: %s id_klienta1 id_klienta2 ... \n", argv[0]);
        exit(1);
    }

    while(1)
    {
        if (msgrcv(msgqid, (struct Message*) &message, MAXSIZE, message.mtype, 0) < 0)
        {
            perror("msgrcv");
            exit(1);
        }
        usleep(500000);
        printf("Received message: %s", message.text);
        
        if (msgsnd(msgqid, &message, 80, 0) < 0)
        {
            perror("msgsnd");
            exit(1);
        }
    }
}