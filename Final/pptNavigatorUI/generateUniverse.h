#include "stdafx.h"
#include<algorithm>
#include<cstdio>
//constants
#define FRAME_SIZE 320
#define p 12
#define Q 12
typedef long double ld;

//This files is used to generate the universe file


void calculateC(ld sample[FRAME_SIZE+1],FILE *out) {

	/*Calculating hamming window and applying on sample*/
	ld window[FRAME_SIZE+1];
	for(int i=1;i<=FRAME_SIZE;i++) {
		window[i] = 0.54 - 0.46*cos((2.000000*22.000000/7.000000*(i-1))/(FRAME_SIZE-1));
		sample[i] = sample[i]*window[i];
	}

	//calculating R
	ld R[p+1];
	for(int i=0;i<=p;i++) {
		R[i] = 0;
		for(int j=1;j<=FRAME_SIZE-i;j++)
			R[i] += sample[j]*sample[j+i];
		//R[i]/=frameSize;
		//printf("\nR[%d] = %Lf",i,R[i]);
	}


	/*Declaring all the arrays for alpha calculation*/
	ld E[p+1];
	ld K[p+1];
	ld alpha[p+1][p+1];

	/*Main algorithm*/
	E[0] = R[0];
	for(int i=1;i<=p;i++) {
		K[i] = R[i];
		for(int j=1;j<=i-1;j++) 
			K[i]-=(alpha[i-1][j]*R[i-j]);
		K[i]/=(E[i-1]*1.0);
		alpha[i][i] = K[i];
		for(int j=1;j<=i-1;j++) 
			alpha[i][j] = alpha[i-1][j] - K[i]*alpha[i-1][i-j];
		E[i] = (1-K[i]*K[i])*E[i-1];
		
	}

	/*Declaring all the arrays for c calculation*/
	ld c[p+1];


	c[0] = log(R[0]*R[0])/log(2.000000);
	for(int m=1;m<=p;m++) {
		c[m] = alpha[p][m];
		for(int k=1;k<=m-1;k++)
			c[m]+=((k*1.000000/m*1.000000)*c[k]*alpha[p][m-k]);
		//printf("\nc[%d] = %Lf",m,c[m]);

	}

	/*Applying raised window on c*/
	ld hammingWindow = 0;
	for(int m=1;m<=p;m++) {
		hammingWindow = 1+(Q/2*sin((22.000000/7.000000)*(m/Q)));
		c[m] = c[m] * hammingWindow;
	}

	for(int m=1;m<=p;m++) 
		fprintf(out,"%Lf ",c[m]);
	fprintf(out,"\n");

	
}