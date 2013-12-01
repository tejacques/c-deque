#ifndef __DEQUE_H__
#define __DEQUE_H__

#define DEQUE_DEFAULT_LEN 8
typedef struct deque {
    int startOffset;
    int endOffset;
    int length;
    int lengthMinusOne;
    int count;
    void** buffer;
    void *(*dup)(void *ptr);
    void (*free)(void *ptr);
    int (*match)(void *ptr, void *key);
} deque;

/* Functions implemented as macros */
#define dequeLength(d) ((d)->length)
#define dequeLengthMinusOne(d) ((d)->lengthMinusOne)
#define dequeCount(d) ((d)->count)
#define dequeFull(d) ((d)->count == (d)->length)
#define dequeEmpty(d) ((d)->count == 0)

#define dequeSetDupMethod(d,m) ((d)->dup = (m))
#define dequeSetFreeMethod(d,m) ((d)->free = (m))
#define dequeSetMatchMethod(d,m) ((d)->match = (m))

#define dequeGetDupMethod(d) ((d)->dup)
#define dequeGetFreeMethod(d) ((d)->free)
#define dequeGetMatchMethod(d) ((d)->match)

/* Prototypes */
deque *dequeCreate(void);
void dequeRelease(deque *deque);

deque *dequeAddHead(deque *deque, void *value);
deque *dequeAddTail(deque *deque, void *value);
deque *dequeInsert(deque *deque, int index, void *value);

deque *dequeAddHeadRange(deque *deque, void **values, int num);
deque *dequeAddTailRange(deque *deque, void **values, int num);
deque *dequeInsertRange(deque *deque, int index, void **values, int num);

void *dequeRemoveHead(deque *deque);
void *dequeRemoveTail(deque *deque);
void *dequeRemoveAt(deque *deque, int index);

void *dequeGet(deque *deque, int index);
deque *dequeSet(deque *deque, int index, void *value);

#endif /* __DEQUE_H__ */
