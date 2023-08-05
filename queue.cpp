#include <queue.h>
#include <stdio.h>

void init_queue(Queue *q)
{
    q->first = 0;
    q->last = QUEUESIZE-1;
    q->count = 0;
}

void enqueue(Queue *q, int x)
{
    if (q->count >= QUEUESIZE)
    {
        printf("Warning: queue overflow enqueue x=%d\n",x);
    }
    else
    {
        q->last = (q->last+1) % QUEUESIZE;
        q->q[q->last] = x;
        q->count = q->count + 1;
    }
}

int dequeue(Queue *q)
{
    int x = 0;
    if (q->count <= 0)
    {
        printf("Warning: empty queue dequeue.\n");
    }
    else
    {
        x = q->q[q->first];
        q->first = (q->first+1) % QUEUESIZE;
        q->count = q->count - 1;
    }

    return(x);
}

void print_queue(Queue *q)
{
    int i = q->first;
    while (i != q->last)
    {
        printf("%c ",q->q[i]);
        i = (i+1) % QUEUESIZE;
    }
    printf("%2d ",q->q[i]);
    printf("\n");
}
