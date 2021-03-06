#include <stdio.h>

#define BUFF_SIZE 16

// http://faraday.ee.emu.edu.tr/EENG420/ince_ppts/IIR_filters.pdf
// http://scistatcalc.blogspot.com/2013/12/fft-calculator.html
// http://damian.pecke.tt/beat-detection-on-the-arduino

long example1(long sample);
long gc9(long sample);
long gc9_mod(long sample);
void make_impulse(long data[], int size);

int main() {
    long data[BUFF_SIZE];
    long (*filt)(long); // pointer to a filter method
    filt = &gc9_mod; // CHANGE this to test different filters

    make_impulse(data,BUFF_SIZE);
    printf("Real Output:\n");
    for(int i=0;i<BUFF_SIZE;i++) {
        long filtdata = filt(data[i]);
        printf("%ld\n",filtdata);
    }
    printf("\nImaginary Output:\n");
    for(int i=0;i<BUFF_SIZE;i++) {
        printf("0\n");
    }
}

void make_impulse(long data[], int size) {
    data[0] = 1024;
    for(int i=1;i<size;i++) data[i] = 0;
}

/*
 * Goodman Carey F8 Filter
 * Halfband filter meaning we filter out things over Fs/2
 * Taps - note we skip every other one
 * h(0)  h(1)  h(3)  h(5)  h(7)
 * 802   490   -116  33    33
 * DC Gain is 1203
 */
long example1(long sample) {
    static long xv[8] = {0,0,0,0,0,0,0,0};

    xv[7] = xv[6]; xv[6] = xv[5]; xv[5] = xv[4];
    xv[4] = xv[3]; xv[3] = xv[2]; xv[2] = xv[1];
    xv[1] = xv[0]; xv[0] = sample;

    long ret = 802*xv[0]+490*xv[1]-116*xv[3]+33*xv[5]-6*xv[7];
    return ret/1203;
}

/*
 * Goodman Carey F9 Filter
 * Halfband filter meaning we filter out things over Fs/2
 * Taps - note we skip every other one
 * h(0)  h(1)  h(3)  h(5)  h(7)  h(9)
 * 8192  5042  -1277 429   -116  18
 * DC Gain is 12,288
 */
long gc9(long sample) {
    static long xv[10] = {0,0,0,0,0,0,0,0};

    xv[9] = xv[8]; xv[8] = xv[7]; xv[7] = xv[6];
    xv[6] = xv[5]; xv[5] = xv[4]; xv[4] = xv[3];
    xv[3] = xv[2]; xv[2] = xv[1]; xv[1] = xv[0];
    xv[0] = sample;

    long y0 = xv[0] << 13; // Save one multiply since we need 8192*xv[0]
    long ret = y0+5042*xv[1]-1277*xv[3]+429*xv[5]-116*xv[7]+18*xv[9];
    return ret/12288;
}

/*
 * Goodman Carey F9 Filter Modified
 * Halfband filter meaning we filter out things over Fs/2
 * Taps - note we skip every other one
 * h(0)  h(1)  h(3)  h(5)  h(7)  h(9)
 * 8192  5042  -1277 429   -116  18
 * DC Gain is 12,288
 */
long gc9_mod(long sample) {
    static long xv[10] = {0,0,0,0,0,0,0,0};
    long yv[10] = {0,0,0,0,0,0,0,0};

    xv[9] = xv[8]; xv[8] = xv[7]; xv[7] = xv[6];
    xv[6] = xv[5]; xv[5] = xv[4]; xv[4] = xv[3];
    xv[3] = xv[2]; xv[2] = xv[1]; xv[1] = xv[0];
    xv[0] = sample;

    // Set multiplier taps (without sign)
    yv[0] = xv[0] << 13; // 8192 = 8192
    yv[1] = xv[1] << 12; // 4096 ~ 5042
    yv[2] = xv[3] << 10; // 1024 ~ 1277
    yv[3] = xv[5] << 9;  // 512 ~ 429
    yv[4] = xv[7] << 7;  // 128 ~ 116
    yv[9] = xv[9] << 4;  // 16 ~ 18

    // Summing all total gain is 11,668

    // Accululate into yv[0] with sign
    yv[0] += yv[1];
    yv[0] -= yv[2];
    yv[0] += yv[3];
    yv[0] -= yv[4];
    yv[0] += yv[5];

    return yv[0]/11668;
}
