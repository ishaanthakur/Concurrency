 /* Test Case- 1

There is only one call to [process_create].
Expected output-if only P1 is called, that process gets added to queue
Therefore, we only see blue LED toggle until P1 terminates.
After which the green LED is displayed, which signals the end of all the processes */

#include "3140_concur.h"
#include "utils.h"

// Function P1 toggles Blue LED
void p1 (void)
{
	int i;
	for (i=0; i < 10; i++) {
	delay ();
    	LEDBlue_Toggle();
	}
}

// Function P2 toggles Red LED
void p2 (void)
{
	int i;
	for (i=0; i < 10; i++) {
	delay ();
    	LEDRed_Toggle();
	}
}

/* 
[process_create] only calls on p1
Green LED at end signals all the processes have finished executing. */

int main (void)
 {
 LED_Initialize();
 
 if (process_create (p1,20) < 0) {
 	return -1;
 }

 process_start ();
 
 LEDGreen_On();
 
 while (1) ;

 return 0;
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
