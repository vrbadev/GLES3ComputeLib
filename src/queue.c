/// \file queue.c
/// \author Vojtech Vrba (vrba.vojtech [at] fel.cvut.cz)
/// \date May 2023
/// \brief Implementation of simple dynamicly allocated queue of pointers.
/// \copyright GNU Public License.

#include "queue.h"

queue_t* queue_create(int capacity)
{
    queue_t* new = (queue_t*) malloc(sizeof(queue_t));
    new->start_pos = 0;
    new->end_pos = 0;
    new->max_size = capacity;
    new->content = (void**) malloc(capacity * sizeof(void*));
    return new;
}

void queue_delete(queue_t* queue)
{
    free(queue->content);
    free(queue);
}

/// Dynamically reallocates the queue internal pointer storage to a new size. All pointers are at first copied to the beginning of the queue storage.
/// \param queue Queue instance that shall be resized.
/// \param resize_always If false, the queue is resized only if the internal storage is depleted.
/// \param newsize New number of pointers to be stored in the queue.
/// \return true on success.
static bool queue_resize(queue_t* queue, bool resize_always, int newsize)
{
    int size = queue_size(queue);
    // move elements to the beginning
    if (queue->start_pos > 0) {
        for (int i = 0; i < size; i++) {
            queue->content[i] = queue->content[queue->start_pos + i];
        }
        queue->max_size += queue->start_pos;
        queue->start_pos = 0;
        queue->end_pos = size;
    }
    // reallocate memory
    if (resize_always || size == queue->max_size) {
        queue->max_size = newsize;
        void** new_content = (void**) realloc(queue->content, queue->max_size * sizeof(void*));
        if (new_content == NULL) {
            return false;
        }
        queue->content = new_content;
    }

    return true;
}

bool queue_push(queue_t* queue, void* data)
{
    int size = queue_size(queue);
    if (size == queue->max_size) {
        if (!queue_resize(queue, false, queue->max_size * 2)) {
            return false;
        }
    }
    queue->content[queue->end_pos++] = data;
    return true;
}

void* queue_pop(queue_t* queue)
{
    int size = queue_size(queue);
    if (size > 0) {
        if (3 * size < queue->max_size && queue->max_size >= 3 * QUEUE_MIN_SIZE) {
            queue_resize(queue, true, (queue->max_size + 2) / 3);
        }

        queue->max_size--;
        return queue->content[queue->start_pos++];
    }
    return NULL;
}

void* queue_get(queue_t* queue, int i)
{
    if (i >= 0 && i < queue_size(queue)) {
        return queue->content[queue->start_pos + i];
    }
    return NULL;
}

int queue_size(queue_t* queue)
{
    return queue->end_pos - queue->start_pos;
}

