#include <stdlib.h>
#include <string.h>
#include "cqueue.h"

#define MIN_CAPACITY ( 1 << 2 )

/*
 * The first power of 2 greater than num, or num if such power does not fit in
 * an unsigned int
 */
static unsigned int cqueue_next_size( unsigned int num ){
  unsigned int n = MIN_CAPACITY;
  while( n < num && n > 0 ){
    n <<= 1;
  }
  return n > 0 ? n : num;
}

/*
 * A wrapper around realloc that does not fail: it succeeds or aborts
 */
static void cqueue_realloc( CQueue* queue ){
  queue->data = realloc( queue->data, queue->data_size * sizeof( CPointer ) );
  if( queue->data == NULL ){
    abort();
  }
}

CQueue* cqueue_new(){
  return cqueue_sized_new( 0 );
}

/*
 * A new queue with a minimum capacity
 */
CQueue* cqueue_sized_new( unsigned int reserved_size ){
  CQueue* queue = calloc( 1, sizeof( CQueue ) );
  if( queue == NULL ){
    abort();
  }
  queue->data_size = cqueue_next_size( reserved_size + 1 );
  cqueue_realloc( queue );
  return queue;
}

void cqueue_free( CQueue* queue ){
  if( queue != NULL ){
    free( queue->data );
    free( queue );
  }
}

/*
 * Deallocate all unused capacity of a queue
 */
void cqueue_shrink( CQueue* queue ){
  if( queue->front <= queue->back ){
    memmove( queue->data, queue->data + queue->front,
             ( queue->back - queue->front ) * sizeof( CPointer ) );
    queue->back = queue->back - queue->front;
    queue->front = 0;
    queue->data_size = queue->back + 1;
  } else {
    unsigned int n = queue->data_size - queue->front;
    memmove( queue->data + queue->back + 1, queue->data + queue->front,
             n * sizeof( CPointer ) );
    queue->front = queue->back + 1;
    queue->data_size = queue->front + n;
  }
  cqueue_realloc( queue );
}

/*
 * Add an element to a queue, increasing the capacity of the queue if necessary
 */
void cqueue_push( CQueue* queue, CPointer data ){
  queue->data[queue->back++] = data;
  if( queue->back % queue->data_size == queue->front ){
    unsigned int n = queue->data_size - queue->front;
    queue->data_size = cqueue_next_size( queue->data_size + 1 );
    cqueue_realloc( queue );
    if( queue->back == queue->front ){
      memmove( queue->data + queue->data_size - n, queue->data + queue->front,
               n * sizeof( CPointer ) );
      queue->front = queue->data_size - n;
    }
  }
  queue->back %= queue->data_size;
}

/*
 * The oldest member from a queue, which is then removed from the queue, or
 * NULL if the queue is empty
 */
CPointer cqueue_pop( CQueue* queue ){
  CPointer p = NULL;
  if( !cqueue_is_empty( queue ) ){
    p = queue->data[queue->front++];
    queue->front %= queue->data_size;
  }
  return p;
}

/*
 * A queue member at a given offset, or NULL if the offset is out of bounds
 */
CPointer cqueue_peek( CQueue* queue, int index ){
  if( index < 0 ){
    index += cqueue_length( queue );
  }
  if( index >= 0 && index < cqueue_length( queue ) ){
    return queue->data[( queue->front + index ) % queue->data_size];
  }
  return NULL;
}

bool cqueue_is_empty( CQueue* queue ){
  return queue->back == queue->front;
}

unsigned int cqueue_length( CQueue* queue ){
  if( queue->back >= queue->front ){
    return queue->back - queue->front;
  } else {
    return queue->back - queue->front + queue->data_size;
  }
}

unsigned int cqueue_capacity( CQueue* queue ){
  return queue->data_size - 1;
}

