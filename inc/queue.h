/// \file queue.h
/// \author Vojtech Vrba (vrba.vojtech [at] fel.cvut.cz)
/// \date May 2023
/// \brief This header file contains definitions for simple dynamicly allocated queue of pointers.
/// \copyright GNU Public License.

#pragma once

#ifndef GLES3COMPUTELIB_QUEUE_H
#define GLES3COMPUTELIB_QUEUE_H

#include <stdbool.h>
#include <stdlib.h>

/// Minimum size of the queue that can be allocated.
#define QUEUE_MIN_SIZE 32

/// Structure of queue instance.
typedef struct queue_s {
    /// Storage index of the first element in the queue.
    int start_pos;
    /// Storage index of the last element in the queue.
    int end_pos;
    /// Virtual maximum capacity of the queue.
    int max_size;
    /// Pointer to the internal storage of queue elements (pointers).
    void** content;
} queue_t;

/// Allocates and initializes a queue instance for void* pointers.
/// \param capacity Initial number of pointers for allocation of internal storage.
/// \return Initialized queue_t* instance.
queue_t* queue_create(int capacity);

/// Deallocates queue resources (using internal storage and the instance pointers).
/// \param queue Queue instance that shall be deallocated.
void queue_delete(queue_t* queue);

/// Pushes new pointer into the queue.
/// \param queue Queue instance.
/// \param data Pointer to the new queue element.
/// \return true on success.
bool queue_push(queue_t* queue, void* data);

/// Pops the first element from the queue. This decreases the virtual capacity of the queue.
/// \param queue Queue instance.
/// \return Pointer to the first element of the queue.
void* queue_pop(queue_t* queue);

/// Reads i-th element in the queue without decreasing the queue's capacity.
/// \param queue Queue instance.
/// \param i Index of the element in the queue.
/// \return Pointer to the i-th element in the queue.
void* queue_get(queue_t* queue, int i);

/// Calculates the current size of the queue.
/// \param queue Queue instance.
/// \return Size of the queue.
int queue_size(queue_t* queue);

#endif // GLES3COMPUTELIB_QUEUE_H
