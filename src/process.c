#include "3140_concur.h"
#include <stdlib.h>
#include <fsl_device_registers.h>


struct process_state {
	unsigned int *sp;
	struct process_state *next;

};

process_t *current_process = NULL;

/* initialize global variable of queue and keep track of first process in queue */
process_t *queue_begin = NULL; 

void add_to_queue (process_t *new_process) 
{
	/**list is empty, addig to beginning*/
	if (queue_begin == NULL) {
		queue_begin = new_process;
	queue_begin -> next = NULL;
	
	} 
	// list not empty, addin to end
	else {
		process_t *temp = queue_begin;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		// adding new process to the last of queue
		temp->next = new_process;
		new_process->next = NULL;
		
	}
}

/** Remove first element from queue and returns element removed*/
process_t *remove_from_queue (void) {
	if (queue_begin == NULL) return NULL;
	process_t *element = queue_begin;
	queue_begin = queue_begin->next;
	return element;
}
// Start process with a given time
void process_start (void)
{
	SIM->SCGC6 = SIM_SCGC6_PIT_MASK;
	PIT->MCR &= 0;
	PIT->CHANNEL[0].LDVAL = 0x1E7460; 
	PIT->CHANNEL[0].TFLG |= 1;
	PIT->CHANNEL[0].TCTRL |= 1<<0;
	PIT->CHANNEL[0].TCTRL |= 1<<1;
	NVIC_EnableIRQ(PIT0_IRQn);
	process_begin();
}


/* Creates a new process that starts at function f, initial stack size n 
Returns 0 on success, -1 if error 
May require allocating memory for a process_t structure, use malloc() 
 */
int process_create (void (*f) (void), int n)
{
	process_t *start = (process_t*) malloc(sizeof(process_t));
	if (start == NULL) return -1;
	unsigned int *new_sp = process_stack_init(f, n);
	if (new_sp == NULL) return -1;
	start->sp = new_sp;
	add_to_queue(start);
	return 0;
}


unsigned int *process_select(unsigned int * cursp)
{
	if (cursp == NULL) { 
		if (queue_begin == NULL) return NULL;
		process_t *temp = current_process;
		current_process = remove_from_queue();
		process_stack_free(temp->sp, sizeof(temp));
		free(temp);
		// update the current_process; null if queue is empty
		return current_process->sp;
	}
	// The else statement is executed if the process is not completed
	else {
		
		current_process->sp = cursp;
		add_to_queue(current_process);
		current_process = remove_from_queue();
		return current_process->sp;
	}
			
}
