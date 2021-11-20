#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <float.h>
#include <filesystem>

enum class stage{
    TRAIN, 
    TEST,
    RECORD,
};

namespace utils{
    extern std::string project_root;

    extern void update_bar(int, const std::string&);

    //normalizing the input and giving an output file which contains the normalized output
    extern void normalize_input(FILE *input, FILE *output, int limit = 5000);

    //for applying the raised sin window on Ci's
    extern double raisedSin(int m);

    //for applying the hamming window on Si's
    extern double hammingWindow(int m);

    //calculating Ri's and outputing in a file
    extern void calculateR(double *s, FILE *output, double *r);

    //calculating Ri's for more than 1 frames and saving in a matrix
    extern void calculateR(double s[][320], double r[][13]);

    //calculating Ai's and saving in a file
    extern void calculateA(double *r, FILE *output, double *a);

    //calculating Ai's and saving in matrix for more than 1 frame
    extern void calculateA(double r[][13], double a[][13]);

    //calculating Ci's and saving in a file
    extern void calculateC(double sigma, double *a, FILE *output, double *c);

    //calculating Ci's and saving in a matrix after applying raised sin window on it for multiple frame
    extern void calculateC(double r[][13], double a[][13], double c[][13]);

    //skipping frames using frame count
    extern long skipFrames(FILE *input, long skipFrameCnt = 25*320);

    //to find the steady framees and save them in s
    extern void find_stable_frames(FILE *input, double s[][320], int x);
    // finding the tokhura's distance by reading the ci's of the reference file and ci's of the test
    // c_ref saves the reference ci's
    // c save the calculated ci's for test
    extern double tokhuraDistance(double c[][13], double w[], FILE *input);

    extern long double tokhura_distance(double c[13], long double codebook[13], long double w[12]);

    // finding the euclidian's distance by reading the ci's of the reference file and ci's of the test
    // c_ref saves the reference ci's
    // c save the calculated ci's for test
    extern double euclidianDistance(double c[][13], FILE *input);

    //utility function for applying hamming window
    extern void applyHamming(double s[][320]);
};

#endif