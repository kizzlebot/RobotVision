#include <stdio.h>                          /* Sobel.c */
#include <stdlib.h>                          /* Sobel.c */
#include <math.h>




int ** alloc(int nByn);
int ** readFile(char ** infile);

int main(int argc, char **argv){


    /* Calculate the low & high thresholds.
     *   low  = 50% lower than middle threshold entered as an argument
     *   high = 50% higher than the entered threshold                   */
    double threshold = atof(*(argv+3)),
           lowthr  = threshold - (threshold * .5),
           highthr = threshold + (threshold * .5);
    int ** pic = readFile(argv+1);


    /*        Covolution?          */
    int outpic[2][0x100][0x100],                // outpicx = outpic[0], outpicy = outpic[1]
        maskx[3][3] = {{-1,0,1},
                       {-2,0,2},
                       {-1,0,1}},
        masky[3][3] = {{ 1, 2, 1},
                       { 0, 0, 0},
                       {-1,-2,-1}};
    // 0,0 -> 2,2
    // 0,1 -> 1,2
    // 0,2 -> 0,2
    // 1,0 -> 2,1
    double ival[0x100][0x100];
    double maxival = 0;


    int mr = 1,
        i = 0 ;
    for (i = mr; i < 0x100-mr; i++){
        int j = 0 ;
        for (j = mr; j < 0x100-mr; j++){
            outpic[0][i][j] = outpic[1][i][j] = 0 ;
            int p = 0 ;
            for (p = -mr; p <= mr; p++){
                int q = 0 ;
                for (q = -mr; q <= mr; q++){
                    outpic[0][i][j] += pic[i+p][j+q] * maskx[(p+mr)%3][(q+mr)%3];   // x
                    outpic[1][i][j] += pic[i+p][j+q] * maskx[(q+mr)][(p+mr)];   // y
                }
            }
            ival[i][j] = sqrt( (double) (pow( outpic[0][i][j], 2) + pow( outpic[1][i][j], 2)) );    // Calculate Magnitude
            maxival = (ival[i][j] > maxival) ? ival[i][j] : maxival;                                // If calculated mag is greatest, replace maxival
        }
    }









    /* Write headers to the output file, lowthr file, and highthr file */
    FILE * fopen(),
         * outputFile       = fopen(    *(argv+2),  "wb"),     // Output file
         * lowThresholdOut  = fopen( "lowthr.pgm",  "wb"),     // Low Threshold file
         * highThresholdOut = fopen("highthr.pgm",  "wb");     // High Threshold File

    fprintf(outputFile,       "P5\n256 256\n255\n");
    fprintf(lowThresholdOut,  "P5\n256 256\n255\n");
    fprintf(highThresholdOut, "P5\n256 256\n255\n");

    /* For each pixel in the image, if calcd magnitude of each pixel is greater than
     * the low/high threshold, write to corresponding file                              */
    for ( i = 0 ; i < 0x100; i++ ){
        int j = 0 ;
        for( j = 0; j < 0x100; j++ ){
            // newMag = (pixelMag / maxMag) * 255
            int outColor = (ival[i][j] / maxival) * 0xFF;                               // Determine output color
            fprintf(outputFile,"%c",(char)((int)(outColor)));                           // Output image

            // If ival[i][j] is greater than low Threshold write a white pixel, else black
            fprintf(lowThresholdOut,  "%c", ( ( outColor > lowthr)  ? 255 : 0 ) );      // Low Threshold
            // If ival[i][j] is greater than high Threshold write a white pixel, else black
            fprintf(highThresholdOut, "%c", ( ( outColor > highthr) ? 255 : 0 ) );      // High Threshold
        }
    }
}


int ** alloc(int nByn){
    int ** ch = malloc(sizeof(int *)*nByn);
    int i = 0 ;
    for ( i = 0 ; i < nByn; i++ ){
        *(ch+i) = malloc(sizeof(int)*nByn);
        int j= 0 ;
        for ( j = 0 ; j < nByn; j++ ){
            *(*(ch+i)+j) = 0;
        }
    }
    return ch ;
}
int ** readFile(char ** infile){
    FILE * fopen(),
         * fp1 = fopen(*(infile),"rb");         // Input file
    int ** pic = alloc(0x100);
    int i = 0 ;
    for (i = 0; i < 0x100; i++){
        int j = 0 ;
        for (j = 0; j < 0x100; j++){
            pic[i][j]  =  getc(fp1);
        }
    }
    return pic;
}

