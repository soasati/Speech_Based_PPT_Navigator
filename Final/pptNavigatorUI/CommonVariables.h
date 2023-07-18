#include "stdafx.h"
#include<stdlib.h>
#include<math.h>
typedef long double ld;

//The common variables that are being used all over the project

char profile[20];
char wordForTraining[100];

#define MAX_WORDS 11
#define MAX_UTTERANCE 25
#define FRAME_SIZE 320
#define MAX_FRAME 1000

#define N 5
#define Tmax 1000
#define M 32
#define threshold 1.0e-030

//The word array that stores the words currently in the project
char words[MAX_WORDS][100] = {"goto","open","slide","back","close","two","three","four","five","six","present"};

ld tempArr1[FRAME_SIZE+1];

ld pi[N+1] = {0.0};
ld A[N+1][N+1] = {{0.0}};
ld B[N+1][M+1] = {{0.0}};
ld A_New[N+1][N+1] = {{0.0}};
ld B_NewG[N+1][M+1] = {{0.0}};
ld alpha[Tmax+1][N+1] = {{0.0}};
ld beta[Tmax+1][N+1] = {{0.0}};
ld gamma[Tmax+1][N+1] = {{0.0}};
ld delta[Tmax+1][N+1] = {{0.0}};
int q[Tmax+1] = {0};
int psi[Tmax+1][N+1] = {{0}};
ld pStarPrev = 0.0;
ld pStarCurr = 0.0;
int qStar[Tmax+1] = {0};
ld xi[Tmax+1][N+1][N+1] = {{{0.0}}};
ld probabilityOfOGivenLambda = 0.0;
FILE *logFp = NULL;

long double maximum(long double a,long double b)
{
    if(a>b)
        return a;
    return b;
}



long double minimum(long double a,long double b)
{
    if(a<b)
        return a;
    return b;
}