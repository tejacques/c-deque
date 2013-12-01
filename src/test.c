#define __DEBUG__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test.h"
#include "utility.h"

int main()
{
    m_deque = dequeCreate();

    dbgprintf("\n\n========== Deque Add Head Test ==========\n");
    testDequeAddHead();
    testDequeReset();

    dbgprintf("\n\n========== Deque Add Tail Test ==========\n");
    testDequeAddTail();
    testDequeReset();

    dbgprintf("\n\n==========  Deque Insert Test  ==========\n");
    testDequeInsert();
    testDequeReset();

    dbgprintf("\n\n======= Deque Add Head Range Test =======\n");
    testDequeAddHeadRange();
    testDequeReset();

    dbgprintf("\n\n======= Deque Add Tail Range Test =======\n");
    testDequeAddTailRange();
    testDequeReset();

    dbgprintf("\n\n=======  Deque Insert Range Test  =======\n");
    testDequeInsertRange();

    testGet();
    testSet();

    dequeRelease(m_deque);
    return 0;
}

void testDequeReset()
{
    dequeRelease(m_deque);
    m_deque = dequeCreate();
}

void testDequeAddHead()
{
    for (int i = 0; i < iterations; ++i)
    {
        int *j = malloc(sizeof(int));
        *j = i;
        dequeAddHead(m_deque, j);
    }
}

void testDequeAddTail()
{
    for (int i = 0; i < iterations; ++i)
    {
        int *j = malloc(sizeof(int));
        *j = i;
        dequeAddTail(m_deque, j);
    }
}

void testDequeInsert()
{
    int *j = malloc(sizeof(int));
    *j = iterations - 1;
    dequeInsert(m_deque, 0, j);
    for (int i = 0; i < iterations - 1; ++i)
    {
        j = malloc(sizeof(int));
        *j = i;
        dequeInsert(m_deque, m_deque->count - 1, j);
    }
}

void testDequeAddHeadRange()
{
}

void testDequeAddTailRange()
{

}

void testDequeInsertRange()
{
}

void testGet()
{
    
    dbgprintf("\nDeque: startOffset: %d, endOffset: %d, count: %d, length: %d\n",
            m_deque->startOffset,
            m_deque->endOffset,
            m_deque->count,
            m_deque->length);
    long sum = 0;
    for(int i = 0; i < m_deque->count; ++i)
    {
        int *j = dequeGet(m_deque, i);
        if (j)
        {
            dbgprintf("%d ", *j);
            sum += (long)*j;
        }
        else
        {
            dbgprintf("NULL ");
        }
    }
    dbgprintf("\n");
}

void testSet()
{
    for(int i = 0; i < m_deque->count; ++i)
    {
        int *j = malloc(sizeof(int));
        *j = 0;
        dequeSet(m_deque, i, j);
    }
}
