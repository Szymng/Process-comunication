#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define MAXSIZE 80

struct Message
{
    long mtype;
    int id_klienta;
    char text[MAXSIZE];
};

struct args
{
    struct Message message;
};
int msgqid;

void *Receiver(void *param);

int main(int argc, char *argv[])
{
    int id_klienta = atoi(argv[1]);
    pid_t pid = getpid();
    key_t key;
    struct Message message;
    long type;
    pthread_t tid;
    struct args *arguments = (struct args *)malloc(sizeof(struct args));

    key = ftok(".", 'c');

    if ((msgqid = msgget(key, 0666|IPC_CREAT)) == -1)
    {
        perror("msgget");
        exit(1);
    }

    message.mtype = 1;
    message.id_klienta = id_klienta;
    sprintf(message.text, "PID: %d\tID: %d\tHalo\n", pid, message.id_klienta);

    arguments->message = message;
    
    if(pthread_create(&tid, NULL, Receiver, (void *)arguments))
    {
        perror("thread fail");
    }
    if (msgsnd(msgqid, &message, 80, 0) < 0)
    {
        perror("msgsnd");
        exit(1);
    }
    free(arguments);
}

void *Receiver(void *param)
{
    struct Message message = ((struct args*)param)->message;

    if (msgrcv(msgqid, (struct Message *) &message, 80, 1, 0) < 0)
    {
        perror("msgrcv");
        exit(1);
    }
    printf("Received message: %s\n", message.text);
    pthread_exit(0);
}