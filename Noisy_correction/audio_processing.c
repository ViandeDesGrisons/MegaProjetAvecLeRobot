#include <audio/microphone.h>
#include <audio_processing.h>
#include <arm_math.h>
#include "ch.h"
#include <fft.h>
#include "hal.h"

/*******************************SEMAPHORES****************************************/
static BSEMAPHORE_DECL(sendToComputer_sem, TRUE);


/*****************************STATIC VARIABLES************************************/
//2 times FFT_SIZE because these arrays contain complex numbers (real + imaginary)
static float micLeft_cmplx_input[2 * FFT_SIZE];
static float micRight_cmplx_input[2 * FFT_SIZE];
static float micFront_cmplx_input[2 * FFT_SIZE];
static float micBack_cmplx_input[2 * FFT_SIZE];
//Arrays containing the computed magnitude of the complex numbers
static float micLeft_output[FFT_SIZE];
static float micRight_output[FFT_SIZE];
static float micFront_output[FFT_SIZE];
static float micBack_output[FFT_SIZE];
/****************************END STATIC VARIABLES*********************************/

/*****************************GLOBAL VARIABLES************************************/
float Left_Phase = 0;
float Right_Phase = 0;
float Front_Phase = 0;
float Back_Phase = 0;
int16_t max_norm_index = -1;
/***************************END GLOBAL VARIABLES**********************************/

/***************************INTERNAL FUNCTIONS************************************/
/*	params :
*	float* dataLeft				This buffer contain the magnitude of the FFT of the Left Microphone.
*								It is used to find the max frequency in this micro (should be the
*								same for each microphone).
*	float* dataLeft_cmplx		This buffer contain the complex numbers of each frequency of the the Left
*								microphones. Used to compute the phase of the max frequency.
*	float* dataRight_cmplx		Same thing for Right microphone
*	float* dataFront_cmplx		Same thing for Front microphone
*	float* dataBack_cmplx		Same thing for Back microphone
*/

void find_sound(float* dataLeft, float* dataLeft_cmplx, float* dataRight_cmplx, float* dataFront_cmplx, float* dataBack_cmplx){

	float max_norm = MIN_VALUE_THRESHOLD;
	//static because we need to keep the values in order to compute an average
	//If average index = 2, we do the average. Else, the motors keep their old value.

	//search for the highest peak of the Left mic
	for(uint16_t i = MIN_FREQ ; i <= MAX_FREQ ; i++){
		if(dataLeft[i] > max_norm){
			max_norm = dataLeft[i];
			max_norm_index = i;
		}
	}

	//find the real and imaginary part of each mic in order to compute the phase
	float max_Left_real = dataLeft_cmplx[2*max_norm_index];
	float max_Left_im = dataLeft_cmplx[2*max_norm_index + 1];
	float max_Right_real = dataRight_cmplx[2*max_norm_index];
	float max_Right_im = dataRight_cmplx[2*max_norm_index + 1];
	float max_Front_real = dataFront_cmplx[2*max_norm_index];
	float max_Front_im = dataFront_cmplx[2*max_norm_index + 1];
	float max_Back_real = dataBack_cmplx[2*max_norm_index];
	float max_Back_im = dataBack_cmplx[2*max_norm_index + 1];

	//Compute the phase of the mics to see from where the sound is coming
	Left_Phase = atan2(max_Left_im, max_Left_real);
	Right_Phase = atan2(max_Right_im, max_Right_real);
	Front_Phase = atan2(max_Front_im, max_Front_real);
	Back_Phase = atan2(max_Back_im, max_Back_real);

	//We want to take into account the fact that the phase is modulo 2*pi.
	if((Right_Phase - Left_Phase) >= MODULO_THRESHOLD){
		Right_Phase = Right_Phase - 2*PI;
	}
	if((Left_Phase - Right_Phase) >= MODULO_THRESHOLD){
		Left_Phase = Left_Phase - 2*PI;
	}
	if((Front_Phase - Back_Phase) >= MODULO_THRESHOLD){
		Front_Phase = Front_Phase - 2*PI;
	}
	if((Back_Phase - Front_Phase) >= MODULO_THRESHOLD){
		Back_Phase = Back_Phase - 2*PI;
	}
}
/*************************END INTERNAL FUNCTIONS**********************************/

/****************************PUBLIC FUNCTIONS*************************************/
/*
*	Callback called when the demodulation of the four microphones is done.
*	We get 160 samples per mic every 10ms (16kHz)
*	
*	params :
*	int16_t *data			Buffer containing 4 times 160 samples. the samples are sorted by micro
*							so we have [micRight1, micLeft1, micBack1, micFront1, micRight2, etc...]
*	uint16_t num_samples	Tells how many data we get in total (should always be 640)
*/
void processAudioData(int16_t *data, uint16_t num_samples){

	/*
	*
	*	We get 160 samples per mic every 10ms
	*	So we fill the samples buffers to reach
	*	1024 samples, then we compute the FFTs.
	*
	*/
	static uint16_t nb_samples = 0;

	//loop to fill the buffers
	for(uint16_t i = 0 ; i < num_samples ; i+=4){
		//construct an array of complex numbers. Put 0 to the imaginary part
		micRight_cmplx_input[nb_samples] = (float)data[i + MIC_RIGHT];
		micLeft_cmplx_input[nb_samples] = (float)data[i + MIC_LEFT];
		micBack_cmplx_input[nb_samples] = (float)data[i + MIC_BACK];
		micFront_cmplx_input[nb_samples] = (float)data[i + MIC_FRONT];

		nb_samples++;

		micRight_cmplx_input[nb_samples] = 0;
		micLeft_cmplx_input[nb_samples] = 0;
		micBack_cmplx_input[nb_samples] = 0;
		micFront_cmplx_input[nb_samples] = 0;

		nb_samples++;

		//stop when buffer is full
		if(nb_samples >= (2 * FFT_SIZE)){
			break;
		}
	}

	if(nb_samples >= (2 * FFT_SIZE)){
		/*	FFT proccessing
		*
		*	This FFT function stores the results in the input buffer given.
		*	This is an "In Place" function. 
		*/

		doFFT_optimized(FFT_SIZE, micRight_cmplx_input);
		doFFT_optimized(FFT_SIZE, micLeft_cmplx_input);
		doFFT_optimized(FFT_SIZE, micFront_cmplx_input);
		doFFT_optimized(FFT_SIZE, micBack_cmplx_input);

		/*	Magnitude processing
		*
		*	Computes the magnitude of the complex numbers and
		*	stores them in a buffer of FFT_SIZE because it only contains
		*	real numbers.
		*
		*/
		arm_cmplx_mag_f32(micRight_cmplx_input, micRight_output, FFT_SIZE);
		arm_cmplx_mag_f32(micLeft_cmplx_input, micLeft_output, FFT_SIZE);
		arm_cmplx_mag_f32(micFront_cmplx_input, micFront_output, FFT_SIZE);
		arm_cmplx_mag_f32(micBack_cmplx_input, micBack_output, FFT_SIZE);

		nb_samples = 0;

		find_sound(micLeft_output, micLeft_cmplx_input, micRight_cmplx_input, micFront_cmplx_input, micBack_cmplx_input);
	}
}

void wait_send_to_computer(void){
	chBSemWait(&sendToComputer_sem);
}

float* get_audio_buffer_ptr(BUFFER_NAME_t name){
	if(name == LEFT_CMPLX_INPUT){
		return micLeft_cmplx_input;
	}
	else if (name == RIGHT_CMPLX_INPUT){
		return micRight_cmplx_input;
	}
	else if (name == FRONT_CMPLX_INPUT){
		return micFront_cmplx_input;
	}
	else if (name == BACK_CMPLX_INPUT){
		return micBack_cmplx_input;
	}
	else if (name == LEFT_OUTPUT){
		return micLeft_output;
	}
	else if (name == RIGHT_OUTPUT){
		return micRight_output;
	}
	else if (name == FRONT_OUTPUT){
		return micFront_output;
	}
	else if (name == BACK_OUTPUT){
		return micBack_output;
	}
	else{
		return NULL;
	}
}
/**************************END PUBLIC FUNCTIONS***********************************/
