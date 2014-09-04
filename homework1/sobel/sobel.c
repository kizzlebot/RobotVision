#include <stdio.h>                          /* Sobel.c */
#include <stdlib.h>                          /* Sobel.c */

#include <math.h>

int pic[256][256];
int outpicx[256][256];
int outpicy[256][256];

int maskx[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};
int masky[3][3] = {{1,2,1},{0,0,0},{-1,-2,-1}};

double ival[256][256],maxival;

int main(int argc, char **argv){

    int i,j, p, q, mr, sum1, sum2;

    double threshold;
    FILE *fo1, *fo2, *fo3,
         *fp1, *fopen();

    char *foobar;

    argc--;
    argv++;

    foobar = *argv;
    fp1 = fopen(foobar,"rb");

    argc--;
    argv++;
    foobar = *argv;
    fo1 = fopen(foobar,"wb");

    argc--; argv++;
    foobar = *argv;
    threshold = atof(foobar);

    //---------------------------------
    // Read input/output file (pgn)
    //---------------------------------
    fo2 = fopen("lowthr.pgm", "wb");
    fo3 = fopen("highthr.pgm", "wb");



    for (i=0;i<256;i++){
        for (j=0;j<256;j++){
            pic[i][j]  =  getc (fp1);
            pic[i][j]  &= 0377;
        }
    }

    mr = 1;
    for (i=mr;i<256-mr;i++){
        for (j=mr;j<256-mr;j++){
            sum1 = 0;
            sum2 = 0;
            for (p=-mr;p<=mr;p++){
                for (q=-mr;q<=mr;q++){
                    sum1 += pic[i+p][j+q] * maskx[p+mr][q+mr];
                    sum2 += pic[i+p][j+q] * masky[p+mr][q+mr];
                }
            }
            outpicx[i][j] = sum1;
            outpicy[i][j] = sum2;
        }
    }

    maxival = 0;
    for (i=mr;i<256-mr;i++){
        for (j=mr;j<256-mr;j++){
            ival[i][j]=sqrt((double)((outpicx[i][j]*outpicx[i][j]) +
                        (outpicy[i][j]*outpicy[i][j])));

            if (ival[i][j] > maxival) maxival = ival[i][j];
        }
    }



    fprintf(fo1, "P5\n256 256\n255\n");

    for (i=0;i<256;i++){
        for (j=0;j<256;j++){
            ival[i][j] = (ival[i][j] / maxival) * 255;
            fprintf(fo1,"%c",(char)((int)(ival[i][j])));
        }
    }

    fprintf(fo2, "P5\n256 256\n255\n");
    fprintf(fo3, "P5\n256 256\n255\n");

    // This is what specifies the low & high thresholds.
    // low = 50% lower than middle threshold which was entered
    // as an argument
    // high = 50% higher than the entered threshold
    double lowthr = threshold - (threshold * .5);
    double highthr = threshold + (threshold * .5);

    for ( i = 0 ; i < 256; i++ ){
        for( j = 0; j < 256; j++ ){
            // Low Threshold
            fprintf(fo2, "%c", ( ( ival[i][j] > lowthr) ? 255 : 0 ) );
            // High Threshold
            fprintf(fo3, "%c", ( ( ival[i][j] > highthr) ? 255 : 0 ) );
        }
    }
}
