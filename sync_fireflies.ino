/*
the variables a, b, c and d are for the characteristics of f(phiRaw/phiMax)
*/

#define a 0.167
#define b 1.025
#define c 0.00251
#define d 0.995

#define f_CPU 16000000 //clock frequency
#define prescaler 1024 //the prescaler for Timer1 (it's set in the TCCR1B register)
#define phiRaw TCNT1 //the value of the Timer1 is used as the phase variable phi
#define flashLength 200 //in ms


double x = 0; //state x --> x=f(phiRaw/phiMax) 
double epsilon = 0.2 //coupling strength, the amount "x" gets lifted up if a flsh is received

double flashInterval = 3 //in s, the maximum timer value (phiMax) is calculated out of the the interval between two flashes
int phiMax = 0; //compare value for phiRaw, the maximum value Timer1 counts to

void setup()
{
	/*
	Initialize Timer1:
	Timer1 is used to keep track of the flashing rythm if the firefly is not entrained/triggered for another one.
	Therefor the Timer1 is driven in the "Normal-Mode". That means that it only counts up. The counting value is in the register TCNT1 or "phiRaw".
	That register can be compared to the variable "phiMax" to figure out when to flash. The figure "phiMax" can be calculated like that:

	phiMax = (f_CPU * flashInterval) / prescaler

	These values are all defined above!
	*/

	TCCR1A = 0; //for Normal-Mode is no Bit in the TCCR1A register necessary
	TCCR1B = 0b00000101; //the last three bits are to set the prescaler for Timer1. For a prescaler of 1024 (the Timer1 increments it's value at every 1024th clock cycle)
	TIMSK1 = 0; //to ensure that no interrupt request is set
	TCNT1 = 0; //initialize the value of Timer1 with 0 so that it starts from bottom to count up

	phiMax = (f_CPU * flashInterval) / prescaler; //calculation of "phiMax"

	/*
	Initialize Timer2:
	Timer2 is used to modulate the required frequency (~38kHz) for the IR-Receiver. Timer2 is driven in CTC-Mode (Clear Timer on Compare-Match-Mode)
	with the additional setting that if the compare-match is reached the pin OC2A (PB3, Digital Pin 11 --> the IR-LED have to be connected to that pin) is toggled.
	The compare value is stored in the register OCR2A. 
	To calculate the value that have to be stored in OCR2A we have to know witch prescaler N we are using and we need to know the clock frequency f_clk 
	and the frequency f of our signal:

	OCR2A = (f_clk/(2*N*f))-1
	
	*/

	TCCR2A = 0b01000010 //the first "1" chooses the Mode to toggle pin OC2A on compare match. The second "1" is for choosing CTC-Mode
	TCCR2B = 0 //right no the counter is disabled because there is no prescaler set. That is only necessary if a flash is required
	TIMSK2 = 0 //to ensure that no interrupt is set
	OCR2A = 210 //initialize the OCR2A register with the calculated value
	TCNT2 = 0 //initialize the value of the Timer2 so that it start counting from bottom

	/*
	Initialize required pins:
	The IR-LED have to be connected to PB3 (OC2A, Digital Pin 11) like it was mentioned above. The Pin has to be declared as an output. If it's not it won't toggle.

	The visible LED is connected to the PB0 (Digital Pin 8). It has to be an Output.
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	The IR-Receiver has an internal filter circuit that filters out any frequency than 38kHz. It also filters out IR light from DC light sources.
	Thats the reason why the IR-LED has to flash with this certain frequency. That means that the Output of the receiver (witch is internally pulled high) is
	not influenced from ambient light. It in not necessary to take a sample. Thats why the output of the IR-Receiver is connected to PB1 (Digital Pin 9).
	This Pin has to be an Input. And for later: we have to wait until the Pin is Low!!!
	*/

	DDRB |= (1 << DDB0) | (1 << DDB3); //declaring PB0 (Digital Pin 9) and PB3 (OC2A, Digital Pin 11) as Output
	DDRB &= ~(1 << DDB1); //declaring PB1 (Digital Pin 9) as an Input

}


void loop()
{
	
}