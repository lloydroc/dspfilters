#include <stdio.h>

#define BUFF_SIZE 32

// http://faraday.ee.emu.edu.tr/EENG420/ince_ppts/IIR_filters.pdf
// http://scistatcalc.blogspot.com/2013/12/fft-calculator.html
// http://damian.pecke.tt/beat-detection-on-the-arduino

float example1(float sample);
float example2(float sample);
void make_impulse(float data[], int size);

int main() {
    float data[BUFF_SIZE];
    make_impulse(data,BUFF_SIZE);
    for(int i=0;i<BUFF_SIZE;i++) {
        printf("%f\n",data[i]);
    }
    printf("\n\n");
    for(int i=0;i<BUFF_SIZE;i++) {
        float filtdata = example2(data[i]);
        printf("%f\n",filtdata);
    }
    printf("\n\n");
    for(int i=0;i<BUFF_SIZE;i++) {
        printf("0.0\n");
    }
}

void make_impulse(float data[], int size) {
    data[0] = 1.0;
    for(int i=1;i<size;i++) data[i] = 0.0;
}

/*
 * fs = 500Hz
 * (i) complete signal rejection at DC and 250Hz.
 * (ii) A narrow passband centered at 125Hz
 * (iii) A 3dB bandwidth of 10Hz.
 */
float example1(float sample) {
    static float xv[3] = {0,0,0}, yv[3] = {0,0,0};
    xv[0] = xv[1]; xv[1] = xv[2];
    xv[2] = sample;
    yv[0] = yv[1]; yv[1] = yv[2];
    yv[2] = -0.877969*yv[0]+xv[2]-xv[0];
    return yv[2];
}

/*
 * Notch frequency 3dB width of notch sampling frequency
 * 50Hz 5Hz 500 Hz
 * Notch frequency 50Hz
 * 3dB width of notch ±5Hz
 * sampling frequency 500Hz
 */
float example2(float sample) {
    static float xv[3] = {0,0,0}, yv[3] = {0,0,0};
    xv[0] = xv[1]; xv[1] = xv[2];
    xv[2] = sample;
    yv[0] = yv[1]; yv[1] = yv[2];
    yv[2] = -0.8783*yv[0]+1.5164*yv[1]+xv[2]-1.6181*xv[1]+xv[0];
    return yv[2];
}
