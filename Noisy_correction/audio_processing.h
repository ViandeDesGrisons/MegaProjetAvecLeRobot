#ifndef AUDIO_PROCESSING_H
#define AUDIO_PROCESSING_H

#include <hal.h>

#define FFT_SIZE 			1024
#define MIN_VALUE_THRESHOLD	15000
#define PI 					3.14

#define MIN_FREQ			10			//we don't analyze before this index to not use resources for nothing
#define MAX_FREQ			30			//we don't analyze after this index to not use resources for nothing*

#define FREQ_1				16			//250Hz
#define FREQ_1_L			(FREQ_1-1)
#define FREQ_1_H			(FREQ_1+1)
#define FREQ_2				19			//296Hz
#define FREQ_2_L			(FREQ_2-1)
#define FREQ_2_H			(FREQ_2+1)
#define FREQ_3				23			//359Hz
#define FREQ_3_L			(FREQ_3-1)
#define FREQ_3_H			(FREQ_3+1)
#define FREQ_4				26			//406Hz
#define FREQ_4_L			(FREQ_4-1)
#define FREQ_4_H			(FREQ_4+1)

#define PHASE_THRESHOLD		0.3 		//The robot don't turn if the sound is close from the middle
#define MODULO_THRESHOLD	1			//The phase is modulo 2*pi

#define CLEAR				0

#define INITIAL_MAX_NORM   -1

#define NB_MIC				4

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

extern struct data_phase {
	float left, right, front, back;
	int16_t max_norm_index;
} phase;

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
