int phi = 0; //current phase of the firefly cycle
int x = 0; //state x --> x=f(phi) 
int epsilon = 0.2 //coupling strength, the amount "x" gets lifted up if a flsh is received

int frequency = 1 //in Hz, the maximum timer value (phi) is calculated out of the frequency

int trigger_threshold = 0 //threshold after that entrainment could occur --> trigger_threshold = x_max - epsilon
int ambient_treshold = 0 //the IR-Receiver has an internal pull-up resistor, so ambient_treshold has to be smaller if a flash is received 

void void setup()
{
	/*
	Initialize Timer1:
	Timer1 is used to keep track of the flashing rythm if the firefly is not entrained/triggered for another one.
	Therefor the Timer1 is driven in the "Normal-Mode". That means that it only counts up. The counting value is in the register TCNT1.
	That register can be compared to the variable "flashing_threshold" to figure out when to flash.
	*/

	TCCR1A = 0; //for Normal-Mode is no Bit in the TCCR1A register necessary
	TCCR1B = 0b00000101; //the last three bits are to set the prescaler for Timer1. For a prescaler of 1024 (the Timer1 increments it's value at every 1024th clock cycle)
	TIMSK1 = 0; //to ensure that no interrupt request is set
	TCNT1 = 0; //initialize the value of Timer1 with 0 so that it starts from bottom to count up

	/*
	Initialize Timer2:
	Timer2 is used to modulate the required frequency (~38kHz) for the IR-Receiver. Timer2 is driven in CTC-Mode (Clear Timer on Compare-Match-Mode)
	with the additional setting that if the compare-match is reached the pin OC2A (PB3, Digital Pin 11) is toggled.
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
}


void void loop()
{
	
}