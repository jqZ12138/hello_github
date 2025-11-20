#ifndef QUEUE_H
#define QUEUE_H
#include <stdint.h>


typedef struct {
	uint8_t* data; 
	uint32_t head; //Index in the array of the oldest element.
	uint32_t tail; //Index in the array of the youngest element.
	uint32_t size; //Size of the data array.
} Queue;


int queue_init(Queue *queue, uint32_t size);
int queue_enqueue(Queue *queue, uint8_t item);
int queue_dequeue(Queue *queue, uint8_t *item);
int queue_is_full(Queue *queue);
int queue_is_empty(Queue *queue);

#endif // QUEUE_H
