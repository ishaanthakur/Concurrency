/** Test Case-2 


In this test case there exist three processes, two of which have increasing orders of duration while the third process never ends.

In this case P1 is the shortest duration process, hence after, some time of execution
we expect P2 will execute but will not finish. Similarly,P3 will execute and not finish.

Until P1 finishes, P2 will be executed and be interrupted to allow P1 to be executed ( until P1 ends). 
Similarly, until P1 and P2 finish, P3 will be executed and interrupted to allow P1 and P2 to be 
executed until finished. Since P3 never stops executing due to the lack of an upper bound on the for-loop,
we observe the continuous blinking of the red LED, while the green LED is never executed.
(NOTE: Green LED signifies all the processes have terminated)

*/

#include "3140_concur.h"
#include "utils.h"

/* Process-1: Blue LED toggles for a short duration */
void p1 (void)
{
	int i;
	for (i=0; i < 8; i++) {
		delay();
    LEDBlue_Toggle();
	}
}

/* Process-2: Green LED toggles for slightly longer duration */
void p2 (void)
{
	int i;
	for (i=0; i < 14; i++) {
		delay();
    LEDGreen_Toggle();
	}
}

/*Process 3- Red LED toggles forever */
void p3 (void)
{
	int i;
	for (i=0; ; i++) {
		delay();
		LEDRed_Toggle();
	}
}

/* Following test to  see that round robin scheduling is correct.
As P3 takes longer, it should be interrupted and be translated back to the queue 
for P1 and P2 to execute.
Simialrly, as P2 takes longer time than P1, hence, it should be kicked back to the queue,
for P1 to execute.
Green LED at the end tells that all process have finished.

Since P3 never terminates, the Green LED will never be signaled.

 */
int main (void)
 {
 LED_Initialize();
 
 if (process_create (p1,20) < 0) {
 	return -1;
 }

 if (process_create (p2,20) < 0) {
 	return -1;
 }
 
 if (process_create (p3,20) < 0) {
	return -1;
 }
 
 process_start ();
 
 LEDGreen_On(); //  Function never called because P3 never terminates
 
 while (1) ;

 return 0;
}
