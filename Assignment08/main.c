/*******************************************************/
/* Brian Arnberg                                       */
/* Problem Set #5 - Main Program                       */
/*  main.c                                             */
/*******************************************************/
/*  Continuous Loop                                    */
/*  Calls input_handler each loop                      */
/*******************************************************/
/*  ``state'' is a global variable.                    */
/*  ``pressed'' is also a global variable              */
/*******************************************************/

int state = 1; 				// initialize state
int pressed = 0;			// initialize pressed


void main (void) {                     
	
	for (;;) { 			// loop forever
	input_handler();		// call input_handler
	}
}
