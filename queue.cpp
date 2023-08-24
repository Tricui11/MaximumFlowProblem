#ifndef QUEUE_H
#define QUEUE_H
#define QUEUESIZE 1000

#include <stdio.h>

struct Queue
{
    int q[QUEUESIZE+1];
    int first;
    int last;
    int count;

public:
    void init_queue()
    {
        first = 0;
        last = QUEUESIZE-1;
        count = 0;
    }

    void enqueue(int x)
    {
        if (count >= QUEUESIZE)
        {
            printf("Warning: queue overflow enqueue x=%d\n",x);
        }
        else
        {
            last = (last+1) % QUEUESIZE;
            q[last] = x;
            count++;
        }
    }

    int dequeue()
    {
        int x = 0;
        if (count <= 0)
        {
            printf("Warning: empty queue dequeue.\n");
        }
        else
        {
            x = q[first];
            first = (first+1) % QUEUESIZE;
            count--;
        }

        return(x);
    }
};

#endif // QUEUE_H
