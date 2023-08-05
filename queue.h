#ifndef QUEUE_H
#define QUEUE_H
#define QUEUESIZE 1000

struct Queue
{
    int q[QUEUESIZE+1];
    int first;
    int last;
    int count;
};

#endif // QUEUE_H
