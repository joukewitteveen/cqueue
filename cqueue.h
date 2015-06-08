/*
 * CQueue - A queue backed by a circular buffer
 * Copyright (c) 2015, Jouke Witteveen <j.witteveen@gmail.com>
 *
 * This software may be distributed under the terms of the BSD license.
 */

#ifndef __CQUEUE_H
#define __CQUEUE_H

#include <stdbool.h>

typedef void* CPointer;
typedef struct _CQueue CQueue;

CQueue*         cqueue_new          ();
CQueue*         cqueue_sized_new    ( unsigned int reserved_size );
void            cqueue_free         ( CQueue*      queue );
void            cqueue_shrink       ( CQueue*      queue );
void            cqueue_push         ( CQueue*      queue,
                                      CPointer     data );
CPointer        cqueue_pop          ( CQueue*      queue );
CPointer        cqueue_peek         ( CQueue*      queue,
                                      int          index );
bool            cqueue_is_empty     ( CQueue*      queue );
unsigned int    cqueue_length       ( CQueue*      queue );
unsigned int    cqueue_capacity     ( CQueue*      queue );

#endif /* __CQUEUE_H */
