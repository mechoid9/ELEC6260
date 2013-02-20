/*******************************************************/
/* Brian Arnberg                                       */
/* Problem Set #5 - System Tick Timer                  */
/*  tick_timer.c                                       */
/*******************************************************/
/*  Activates once every half second                   */
/*  Calls output_handler (iff LEDS should change)      */
/*******************************************************/
/*  ``state'' is a global variable.                    */
/*******************************************************/


interrupt void SysTick_Handler ( void ) {
	if (state != 1) { 		// if not state 1
		output_handler(); 	// write to LEDS 
	}				// else, return
}
