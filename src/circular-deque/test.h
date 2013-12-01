#ifndef __TEST_H__
#define __TEST_H__
#include "deque.h"

struct deque *m_deque;
size_t iterations = 10000;

void testDequeReset();

void testDequeAddHead();
void testDequeAddTail();
void testDequeInsert();

void testDequeAddHeadRange();
void testDequeAddTailRange();
void testDequeInsertRange();

void testDequeRemoveHead();
void testDequeRemoveTail();
void testDequeRemoveAt();

void testGet();
void testSet();
#endif /* __TEST_H__ */
