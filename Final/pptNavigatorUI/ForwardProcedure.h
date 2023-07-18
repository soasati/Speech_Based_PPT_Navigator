
#include "stdafx.h"
#include<stdlib.h>
#include<math.h>
#include"time.h"

//this file is used for calculating the forward procedure for the files

typedef long double ld;

ld forwardProcedure(int T,int O[],char *fileNameA,char *fileNameB) {
	int i,j,t;
	FILE *logFp = fopen("logHMM.txt","w");
	FILE *fp = NULL;
	probabilityOfOGivenLambda = 0;
	//Reading pi
	char piname[50];
	sprintf(piname,"..\\..\\%s\\HMM\\InitialModelPi.txt",profile);
	fp = fopen(piname,"r");
	if(!fp)
		return 0;
	for(i=1;i<=N;i++)
		fscanf(fp,"%Lf",&pi[i]);

	fclose(fp);
	//Reading A
	fp = fopen(fileNameA,"r");
	if(!fp)
		return 0;
	for(i=1;i<=N;i++)
		for(j=1;j<=N;j++)
			fscanf(fp,"%Lf",&A[i][j]);

	fclose(fp);
		


	//Reading B
	fp = fopen(fileNameB,"r");
	if(!fp)
		return 0;
	for(i=1;i<=N;i++)
		for(j=1;j<=M;j++)
			fscanf(fp,"%Lf",&B[i][j]);

	fclose(fp);
	//Forward Procedure
	//initialization
	for(t=1; t<=T; t++){
		for(i=1; i<= N; i++)
			alpha[t][i] = 0;
	}
	for(i=1;i<=N;i++)
		alpha[1][i] = pi[i]*B[i][O[1]];

	//induction
	for(t=1;t<=T-1;t++) {
		for(j=1;j<=N;j++) {
			for(i=1;i<=N;i++) {
				alpha[t+1][j] += (alpha[t][i]*A[i][j]);
			}
			alpha[t+1][j] *= B[j][O[t+1]];
		}
	}

	//termination
	for(i=1;i<=N;i++)
		probabilityOfOGivenLambda+=alpha[T][i];


	//printing probability of O given lambda
	/*time_t ltime; // calendar time 
    ltime=time(NULL); // get current cal time 
    fprintf(logFp,"%s",asctime( localtime(&ltime) ) );*/
	fprintf(logFp,"\nProbability of O given lambda is %.32e\n",probabilityOfOGivenLambda);
	fclose(logFp);
	return probabilityOfOGivenLambda;
}