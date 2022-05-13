#ifndef AUDIO_PROCESSING_H
#define AUDIO_PROCESSING_H

#include <hal.h>

#define FFT_SIZE 	1024
#define MIN_VALUE_THRESHOLD	10000
#define PI 					3.14
#define MIN_FREQ			10		//we don't analyze before this index to not use resources for nothing
#define FREQ				26			//406Hz
#define MAX_FREQ			30		//we don't analyze after this index to not use resources for nothing
#define FREQ_L			(FREQ-1)
#define FREQ_H			(FREQ+1)
#define PHASE_THRESHOLD		0.3 	//The robot don't turn if the sound is close from the middle
#define MODULO_THRESHOLD	1	//The phase is modulo 2*pi
#define SPEED_TURN			600
#define SPEED_FORWARD		600

typedef enum {
	//2 times FFT_SIZE because these arrays contain complex numbers (real + imaginary)
	LEFT_CMPLX_INPUT = 0,
	RIGHT_CMPLX_INPUT,
	FRONT_CMPLX_INPUT,
	BACK_CMPLX_INPUT,
	//Arrays containing the computed magnitude of the complex numbers
	LEFT_OUTPUT,
	RIGHT_OUTPUT,
	FRONT_OUTPUT,
	BACK_OUTPUT
} BUFFER_NAME_t;

extern float Left_Phase;
extern float Right_Phase;
extern float Front_Phase;
extern float Back_Phase;
extern int16_t max_norm_index;

void processAudioData(int16_t *data, uint16_t num_samples);

/*
*	put the invoking thread into sleep until it can process the audio datas
*/
void wait_send_to_computer(void);

/*
*	Returns the pointer to the BUFFER_NAME_t buffer asked
*/
float* get_audio_buffer_ptr(BUFFER_NAME_t name);

#endif /* AUDIO_PROCESSING_H */
