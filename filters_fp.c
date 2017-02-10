#include <stdio.h>

// Implementation of IIR and FIR filters that
// that take floating point data

#define BUFF_SIZE 64

// http://faraday.ee.emu.edu.tr/EENG420/ince_ppts/IIR_filters.pdf
// http://scistatcalc.blogspot.com/2013/12/fft-calculator.html
// http://damian.pecke.tt/beat-detection-on-the-arduino

float example1(float sample);
float example2(float sample);
float example3(float sample);
float example4(float sample);
void make_impulse(float data[], int size);

int main() {
    float data[BUFF_SIZE];
    float (*filt)(float); // pointer to a filter method
    filt = &example4; // CHANGE this to test different filters

    make_impulse(data,BUFF_SIZE);
    printf("\n\n");
    for(int i=0;i<BUFF_SIZE;i++) {
        float filtdata = filt(data[i]);
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

float example3(float sample) {
    static float xv[7] = {0,0,0,0,0,0,0}, yv[7] = {0,0,0,0,0,0,0};
    const static float av[6] = {-0.05,0.43,-1.63,3.48,-4.42,3.17};
    //const float av[6] = {3.17,-4.42,3.48,-1.63,0.43,-0.05};
    xv[0] = xv[1]; xv[1] = xv[2]; xv[2] = xv[3];
    xv[3] = xv[4]; xv[4] = xv[5]; xv[5] = xv[6];
    xv[6] = sample;
    yv[0] = yv[1]; yv[1] = yv[2]; yv[2] = yv[3];
    yv[3] = yv[4]; yv[4] = yv[5]; yv[5] = yv[6];
    yv[6] = 0.0;
    for(int i=0;i<6;i++) {
        yv[6] += av[i]*yv[i];
    }
    yv[6] += xv[0]+xv[2]-xv[4]-xv[6];
    return yv[6];
}

/*
 * Goodman Carey F8 Filter see filters_int.c
 * Halfband lowpass filter at Fs/2
 */
float example4(float sample) {
    static float xv[8] = {0,0,0,0,0,0,0,0};

    xv[7] = xv[6]; xv[6] = xv[5]; xv[5] = xv[4];
    xv[4] = xv[3]; xv[3] = xv[2]; xv[2] = xv[1];
    xv[1] = xv[0]; xv[0] = sample;

    float ret = 802.0*xv[0]+490.0*xv[1]-116.0*xv[3]+33.0*xv[5]-6.0*xv[7];
    return ret/1203;
}
