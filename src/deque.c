#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "deque.h"
#include "utility.h"

/* ================== START PRIVATE FUNCTION PROTOTYPES ==================== */
/* Utility helper functions */
static void copyBuffer(
        deque *deque,
        void **buffer,
        int bufferLength,
        int bufferIndex);
static int toBufferIndex(deque *deque, int index);
static bool resizeDeque(deque *deque, int length);
static bool ensureCapacityFor(deque *deque, int numElements);
static void copyMemory(
        void **source,
        int sourceIndex,
        void **dest,
        int destIndex,
        int num);

/* Helper functions for checks */
static bool indexOutOfRange(deque *deque, int index);
static bool argumentsOutOfRange(int length, int offset, int count);

/* Helper functions for shifting offsets */
static int shiftStartOffset(deque *deque, int amount);
/*static int preShiftStartOffset(deque *deque, int amount);*/
static int postShiftStartOffset(deque *deque, int amount);

static int shiftEndOffset(deque *deque, int amount);
static int preShiftEndOffset(deque *deque, int amount);
/*static int postShiftEndOffset(deque *deque, int amount);*/

/* ================== END PRIVATE FUNCTION PROTOTYPES ====================== */

/* ================ START PRIVATE FUNCTION IMPLEMENTATIONS ================= */
void copyBuffer(deque *deque, void **buffer, int bufferLength, int bufferIndex)
{
    if (NULL == buffer)
    {
        return;
    }

    if(argumentsOutOfRange(bufferLength, bufferIndex, deque->count))
    {
        dbgprintf("arguments out of range.");
        return;
    }

    dbgprintf("\nDeque: startOffset: %d, endOffset: %d, count: %d, length: %d\n",
            deque->startOffset,
            deque->endOffset,
            deque->count,
            deque->length);

    if (deque->startOffset != 0 && deque->startOffset >= deque->endOffset)
    {
        int lengthFromStart = deque->length - deque->startOffset;
        int lengthFromEnd = deque->count - lengthFromStart;

        dbgprintf("count: %d, lengthFromStart: %d, lengthFromEnd: %d\n", deque->count,
                lengthFromStart, lengthFromEnd);

        dbgprintf("Copying from %d elements from %d to %d.\n",
                lengthFromStart,
                deque->startOffset,
                deque->length);

        copyMemory(
            deque->buffer,
            deque->startOffset,
            buffer,
            0,
            lengthFromStart);

        dbgprintf("Copying from %d elements from %d to %d.\n",
                deque->count - lengthFromEnd,
                0,
                deque->count - lengthFromEnd);

        copyMemory(
            deque->buffer,
            0,
            buffer,
            lengthFromStart,
            lengthFromEnd);
    }
    else
    {
        dbgprintf("Copying from %d elements from %d to %d.\n",
                deque->count,
                deque->startOffset,
                deque->startOffset + deque->count);

        copyMemory(deque->buffer, deque->startOffset, buffer, 0, deque->count);
    }
}


/* Convert a deque index to the index of it's buffer */
int toBufferIndex(deque *deque, int index)
{
    return (index + deque->startOffset) & deque->lengthMinusOne;
}


bool resizeDeque(deque *deque, int length)
{
    /* If the length is less than or equal to zero, do nothing */
    if (length <= 0)
    {
        return false;
    }
    /* If the new length is less than the number of items in
     * the deque, do nothing */
    else if (length < dequeCount(deque))
    {
        return false;
    }

    /* Set the length to the closest power of two */
    length = power_of_two_greater_than(length);

    /* If the new length is the same as the current length
     * then do nothing */
    if (length == dequeLength(deque))
    {
        return false;
    }

    /* Create a new array and copy the old values */
    void **newBuffer = malloc(sizeof(void*) * length);
    if (!newBuffer)
    {
        return false;
    }

    copyBuffer(deque, newBuffer, length, 0);
    free(deque->buffer);

    /* Reset deque values */
    deque->buffer = newBuffer;
    deque->startOffset = 0;
    deque->endOffset = deque->count;
    deque->length = length;
    deque->lengthMinusOne = length-1;

    return true;
}

/* Reallocate the deque buffer as necessary to ensure
 * capacity for the specified number of elements */
bool ensureCapacityFor(deque *deque, int numElements)
{
    /* If the addition of numElements will cause the deque to exceed its
     * length, then resize the deque to the appropriate size */
    if (deque->count + numElements > deque->length)
    {
        return resizeDeque(deque, deque->count + numElements);
    }

    /* Otherwise it has enough space already */
    return true;
}

/* Copies memory from a source and offset to a destination and offset */
void copyMemory(
        void **source,
        int sourceIndex,
        void **dest,
        int destIndex,
        int num)
{
    dbgprintf ("copy from %p[%d] to %p[%d] for %lu bytes.\n",
            source,sourceIndex,
            dest,destIndex,
            (num * sizeof(void*)));

    dbgprintf("Source: ");
    for (int i = 0; i < num; i++)
    {
        dbgprintf(" %d", *((int *)((void **)source)[sourceIndex+i]));
    }

    memcpy( (void *)(dest+destIndex),
            (const void *)(source+sourceIndex),
            (size_t)(num * sizeof(void*)));

    dbgprintf("\nDest:   ");
    for (int i = 0; i < num; i++)
    {
        dbgprintf(" %d", *((int *)((void **)dest)[destIndex+i]));
    }
    dbgprintf("\n");
}


/* Returns true if the specified index is out of range
 * of the deque */
bool indexOutOfRange(deque *deque, int index)
{
    return (index >= deque->count);
}

bool argumentsOutOfRange(int length, int offset, int count)
{
    if (offset < 0)
    {
        return true;
    }

    if (count < 0)
    {
        return true;
    }
    if (length - offset < count)
    {
        return true;
    }

    return false;
}

int shiftStartOffset(deque *deque, int amount)
{
    return (deque->startOffset = 
            (deque->startOffset + amount) & deque->lengthMinusOne);
}

/*int preShiftStartOffset(deque *deque, int amount)
{
    int offset = deque->startOffset;
    shiftStartOffset(deque, amount);
    return offset;
}*/

int postShiftStartOffset(deque *deque, int amount)
{
    return shiftStartOffset(deque, amount);
}

int shiftEndOffset(deque *deque, int amount)
{
    return (deque->endOffset =
            (deque->endOffset + amount) & deque->lengthMinusOne);
}

int preShiftEndOffset(deque *deque, int amount)
{
    int offset = deque->endOffset;
    shiftEndOffset(deque, amount);
    return offset;
}

/*int postShiftEndOffset(deque *deque, int amount)
{
    return shiftEndOffset(deque, amount);
}*/

/* ================= END PRIVATE FUNCTION IMPLEMENTATIONS ================== */

/* Create a new deque. The created deque can be freed with
 * dequeRelease, but the value of every element in the deque
 * needs to be freed by the user.
 *
 * On error, NULL is returned. Otherwise the pointer to the new deque. */
deque *dequeCreate(void)
{
    struct deque *deque;

    if ((deque = malloc(sizeof(*deque))) == NULL)
    {
        return NULL;
    }

    deque->length = DEQUE_DEFAULT_LEN;
    deque->startOffset = 0;
    deque->endOffset = 0;
    deque->lengthMinusOne = deque->length - 1;
    deque->count = 0;
    deque->dup = NULL;
    deque->free = NULL;
    deque->match = NULL;

    if ((deque->buffer = malloc(sizeof(void *) * deque->length)) == NULL)
    {
        free(deque);
        return NULL;
    }

    return deque;
}

/* Free the deque.
 *
 * This function can't fail. */
void dequeRelease(deque *deque)
{

    if (deque->free)
    {
        for(int i = 0; i < deque->count; ++i)
        {
            void *val = dequeGet(deque, i);
            deque->free(val);
        }
    }

    free(deque->buffer);
    free(deque);
}

/* Add a value to the head of the deque containing the specified
 * value as a pointer.
 *
 * On error, NULL is returned and no operation is performed (i.e.
 * the deque remains unaltered).
 * On success the deque pointer passed to the function is returned. */
deque *dequeAddHead(deque *deque, void *value)
{
    if (!ensureCapacityFor(deque, 1))
    {
        return NULL;
    }

    deque->buffer[postShiftStartOffset(deque, -1)] = value;
    deque->count++;

    return deque;
}

/* Add a value to the head of the deque containing the specified
 * value as a pointer.
 *
 * On error, NULL is returned and no operation is performed (i.e.
 * the deque remains unaltered).
 * On success the deque pointer passed to the function is returned. */
deque *dequeAddTail(deque *deque, void *value)
{
    if (!ensureCapacityFor(deque, 1))
    {
        return NULL;
    }

    deque->buffer[preShiftEndOffset(deque, 1)] = value;
    deque->count++;

    return deque;
}

/* Add a value to the specified index of the deque containing the
 * specified value as a pointer.
 *
 * On error, NULL is returned and no operation is performed (i.e.
 * the deque remains unaltered).
 * On success the deque pointer passed to the function is returned. */
deque *dequeInsert(deque *deque, int index, void *value)
{
    if (!ensureCapacityFor(deque, 1))
    {
        return NULL;
    }

    if (index == 0)
    {
        return dequeAddHead(deque, value);
    }
    else if (index == deque->count)
    {
        return dequeAddTail(deque, value);
    }

    return dequeInsertRange(deque, index, &value, 1);
}

/* Add values to the head of the deque containing the
 * specified values.
 *
 * On error, NULL is returned and no operation is performed (i.e.
 * the deque remains unaltered).
 * On success the deque pointer passed to the function is returned. */
deque *dequeAddHeadRange(deque *deque, void **values, int num)
{
    return dequeInsertRange(deque, 0, values, num);
}

/* Add values to the tail of the deque containing the
 * specified values.
 *
 * On error, NULL is returned and no operation is performed (i.e.
 * the deque remains unaltered).
 * On success the deque pointer passed to the function is returned. */
deque *dequeAddTailRange(deque *deque, void **values, int num)
{
    return dequeInsertRange(deque, deque->count, values, num);
}


/* Add values to the specified index of the deque containing the
 * specified values.
 *
 * On error, NULL is returned and no operation is performed (i.e.
 * the deque remains unaltered).
 * On success the deque pointer passed to the function is returned. */
deque *dequeInsertRange(deque *deque, int index, void **values, int num)
{
    if (indexOutOfRange(deque, index - 1))
    {
        return NULL;
    }

    if (0 == num)
    {
        return NULL;
    }

    /* Make room */
    if (!ensureCapacityFor(deque, num))
    {
        return NULL;
    }

    if (index < deque->count / 2)
    {
        /* Insert into the first half of the buffer */

        if (index > 0)
        {
            /* Move items down:
             * [0, index) ->
             * [length - num, length - num + index)
             */
            int copyCount = index;
            int shiftIndex = deque->length - num;
            for (int i = 0; i < copyCount; ++i)
            {
                deque->buffer[toBufferIndex(deque, shiftIndex + i)] =
                    deque->buffer[toBufferIndex(deque, i)];
            }
        }

        /* Shift the starting offset */
        shiftStartOffset(deque, -num);
    }
    else
    {
        /* Insert into the second half of the buffer */

        if (index < deque->count)
        {
            /* Move items up:
             * [index, deque->count) -> [index + num, num + deque->count)
             */
            int copyCount = deque->count - index;
            int shiftIndex = index + num;
            for (int i = 0; i < copyCount; ++i)
            {
                deque->buffer[toBufferIndex(deque, shiftIndex + i)] =
                    deque->buffer[toBufferIndex(deque, index + i)];
            }
        }

        /* shift the ending offset */
        shiftEndOffset(deque, num);
    }

    /* Copy new items into place */
    int i = index;
    for(int j = 0; j < num; ++j)
    {
        deque->buffer[toBufferIndex(deque, i)] = values[j];
        ++i;
    }

    /* Adjust count */
    deque->count += num;
    return deque;
}

/* Get the value from the deque at the specified index
 *
 * on error, null is returned */
void *dequeGet(deque *deque, int index)
{
    if (indexOutOfRange(deque, index))
    {
        return NULL;
    }

    return deque->buffer[toBufferIndex(deque, index)];
}

/* Set the value from the deque at the specified index
 *
 * on error, null is returned and no operation is performed (i.e.
 * the deque remains unaltered).
 * on success the deque pointer passed to the function is returned. */
deque *dequeSet(deque *deque, int index, void *value)
{
    if (indexOutOfRange(deque, index))
    {
        return NULL;
    }

    deque->buffer[toBufferIndex(deque, index)] = value;

    return deque;
}

