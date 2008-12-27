
/*	
 * queue.h
*/
#ifndef _QUEUE_H_
#define _QUEUE_H_ 
#include <stdint.h>
#include <stdbool.h>

#define QUEUE_BASE_T	uint8_t
#define QUEUE_BASE_T_MAX 0xFF
#define QUEUE_INDEX_T	uint8_t
#define QUEUE_SZ	32

typedef struct {
        QUEUE_BASE_T  buffer[QUEUE_SZ];	// buffer
        QUEUE_INDEX_T first;		// position of first element
        QUEUE_INDEX_T last;		// position of last element
	QUEUE_INDEX_T ct;		// One of these can be eliminated. The other can be calculated from the other 2.
} volatile queue_t;


void q_init(queue_t *q);
int8_t q_push(queue_t *q, QUEUE_BASE_T x);
void q_apush(queue_t *q, const QUEUE_BASE_T x[],QUEUE_INDEX_T sz);
QUEUE_BASE_T q_pop(queue_t *q);
void q_apop(queue_t *q, QUEUE_BASE_T * buffer, QUEUE_INDEX_T sz);
bool q_empty(queue_t *q);
bool q_full(queue_t *q);

#endif
