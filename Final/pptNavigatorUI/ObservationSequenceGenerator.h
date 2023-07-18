#include "stdafx.h"
#include<algorithm>
#include<cstdio>

//This file is used for generating the observation sequence for the files

//constants
#define FRAME_SIZE 320
#define MAX_FRAME 500
#define M 32//codebook size
#define p 12
#define Q 12
#define slide 240//each frame will be shifted by 1/4th i.e. 320/4 = 80 hence read N-slide=80 samples each time
typedef long double ld;
//global variable declaration
ld DCshift = 0;
ld energyArray[MAX_FRAME+1] = {0};
ld tempArr[FRAME_SIZE+1];
ld currC[p+1] = {0.0};
ld obsSeqGen_tokhuraWeight[] = {1.0, 3.0, 7.0, 13.0, 19.0, 22.0, 25.0, 33.0, 42.0, 50.0, 56.0, 61.0};
//Function declaration
ld computeNormalizationFactor(FILE *fp);
void extractStableFrame(char word[],int utterance,int live);
ld computeDCShift();
void detectWord(ld energyArr[],long totaLframes,int *start,int *end,int live);
//Function definition
ld computeDCShift() {
	char inputFileName[100];
	ld temp,Energy = 0,sampleNumber=0;
	char dcshift[200];
	sprintf(dcshift,"..\\..\\%s\\DC_shift.txt",profile);
	FILE *fp = fopen(dcshift, "r");//recorded this file in silence to capture DC shift

	if(!fp) {
		printf("Sorry input file could not be opened\n");
		return 0;
	}

	while (!feof (fp)) {  
		fscanf (fp, "%Lf", &temp); 
		Energy+=temp;
		sampleNumber++;
	}
	Energy/=sampleNumber; //calculating avg energy as dc shift value
    fclose(fp);
	return Energy;
}
ld computeNormalizationFactor(FILE *fp) {
	ld maxAmplitude = 0;
	if(!feof (fp))
		fscanf (fp, "%Lf", &maxAmplitude); 
	maxAmplitude = maxAmplitude-DCshift;
	while (!feof (fp)){
		ld temp;
		fscanf (fp, "%Lf", &temp); 
		maxAmplitude = maximum(maxAmplitude,abs(temp)-DCshift);
	}
	ld factor = 5000.000000/(maxAmplitude*1.0);  
	return factor;
}
void detectWord(long totaLframes,int *start,int *end) {//start and end are out parameters
	int i,j;
	int startThreshold = 1000,endThreshold = 200;
	int startLimit = 10,endLimit = 15;
	//detect maximum energy frame
	ld maxEnergy = 0;
	int maxEnergyFrame = 1;
	for(i=1;i<=totaLframes;i++) {
		if(energyArray[i]>maxEnergy) {
			maxEnergy = energyArray[i];
			maxEnergyFrame = i;
		}
	}

	//traverse from that frame to start
	 i = maxEnergyFrame;
	 while(i>=1) {
		 ld avgEnergy = 0;
		 for(j=1;j<=startLimit && i>=1;j++) {
			 avgEnergy+=energyArray[i];
			 i--;
		 }
		 avgEnergy/=startLimit;
		 if(avgEnergy<startThreshold || i<1)
			 break;
		 
	 }
	 *start = i+startLimit;

	 //traverse from that frame to end
	 i = maxEnergyFrame;
	 while(i<=totaLframes) {
		 ld avgEnergy = 0;
		 for(j=1;j<=endLimit && i<=totaLframes;j++) {
			 avgEnergy+=energyArray[i];
			 i++;
		 }
		 avgEnergy/=endLimit;
		 if(avgEnergy<endThreshold || i>totaLframes)
			 break;
		 
	 }
	 *end = i-endLimit;

}

void extractStableFrame(char word[],int utterance,int live) {//live =1 for live data
	char filename[200],filename1[200];
	if(utterance==-1)//live testing
		sprintf(filename,"input.txt");
	else
		sprintf(filename,"..\\..\\%s\\Dataset\\%s_%d.txt",profile,word,utterance);
	FILE *fp = fopen(filename,"r");
	//FILE *fp = fopen("input.txt","r");
	int count = 0,frames=0,stableFrames=0,start,end;
	ld temp = 0;
	ld sum = 0,energy =0;
	int flag = 0;//flag 0 means start frame
	if(!fp) {
		printf("\nSorry file could not be opened.\n");
		return;
	}
	ld factor = computeNormalizationFactor(fp);
	rewind(fp);
	energy = 0;
	frames = 0;
	sum = 0;
	count = 0;
	while(!feof(fp)) {
		int i;
		if(flag) {//Read first 320 samples
			for(i=1;i<=FRAME_SIZE && !feof(fp);i++) {
				fscanf(fp,"%Lf",&tempArr[i]);
				tempArr[i] = (tempArr[i]-DCshift)*factor;
			}
			if(i<=FRAME_SIZE) {//small frame size ignoring
				//fprintf(logfp,"\nLast frame ignored as less than 320 samples");
				break;
			}
			flag = false;
		}
		else {//slide by 80
			for(i=1;i<=slide;i++)
				tempArr[i] = tempArr[FRAME_SIZE-(slide-i)];
			for(i=slide+1;i<= FRAME_SIZE && !feof(fp);i++) {
				fscanf(fp,"%Lf",&tempArr[i]);
				tempArr[i] = (tempArr[i]-DCshift)*factor;
			}
			if(i<=FRAME_SIZE) {//small frame size ignoring
				//fprintf(logfp,"\nLast frame ignored as less than 320 samples");
				break;
		}
		}
		frames++;
		energyArray[frames] = 0;
		for(i=1;i<=FRAME_SIZE;i++)
			energyArray[frames] += tempArr[i]*tempArr[i];
		energyArray[frames]/=FRAME_SIZE;
	}
	/*while(!feof(fp)) {
		count++;
		fscanf(fp,"%Lf",&temp);
		temp = (temp-DCshift)*factor;
		sum+=temp*temp;
		if(count==FRAME_SIZE) {
			frames++;
			if(frames>MAX_FRAME)
				break;
			sum/=FRAME_SIZE;
			energyArray[frames] = sum;
			count = 0;
			sum = 0;
		}
	}*/


	/*for(int i=1;i<=frames;i++)
		printf("\nenergy[%d] = %Lf ",i,energyArray[i]);*/
	start = 1;
	end = frames;
	detectWord(frames,&start,&end);//live = 0 for pre recorded files
	//printf("\nStart : %d",start);
	//printf("\nEnd : %d",end);
	//write extracted frames in a file
	if(utterance ==-1)
		sprintf(filename,"input_extractedFrames.txt");
	else
		sprintf(filename,"..\\..\\%s\\extractedFrames\\%s_%d_EF.txt",profile,word,utterance);
	FILE *fp1 = fopen(filename,"w");
	rewind(fp);
	count = 0;
	frames = 0;
	
	/*while(!feof(fp)) {
		count++;
		fscanf(fp,"%Lf",&temp);
		temp = (temp-DCshift)*factor;
		tempArr[count] = temp;
		if(count==FRAME_SIZE) {
			frames++;
			if(frames>=start && frames<=end)
				for(int i=1;i<=FRAME_SIZE;i++)
					fprintf(fp1,"%Lf\n",tempArr[i]);
			if(frames>end)
				break;
			count = 0;
			
		}
	}*/
	while(!feof(fp)) {
		int i;
		if(flag) {//Read first 320 samples
			for(i=1;i<=FRAME_SIZE && !feof(fp);i++) {
				fscanf(fp,"%Lf",&tempArr[i]);
				tempArr[i] = (tempArr[i]-DCshift)*factor;
			}
			if(i<=FRAME_SIZE) {//small frame size ignoring
				//fprintf(logfp,"\nLast frame ignored as less than 320 samples");
				break;
			}
			flag = false;
		}
		else {//slide by 80
			for(i=1;i<=slide;i++)
				tempArr[i] = tempArr[FRAME_SIZE-(slide-i)];
			for(i=slide+1;i<= FRAME_SIZE && !feof(fp);i++) {
				fscanf(fp,"%Lf",&tempArr[i]);
				tempArr[i] = (tempArr[i]-DCshift)*factor;
			}
			if(i<=FRAME_SIZE) {//small frame size ignoring
				//fprintf(logfp,"\nLast frame ignored as less than 320 samples");
				break;
		
		}
		}
		frames++;
		if(frames>=start-5 && frames<=end+5)
		for(int i=1;i<=FRAME_SIZE;i++)
			fprintf(fp1,"%Lf\n",tempArr[i]);
		if(frames>end)
			break;
	}
	fclose(fp1);
	fclose(fp);
}

ld obsSeqGen_codeBook[M+1][p+1];

void ObservationSeqGen_calculateC(ld sample[FRAME_SIZE+1]) {

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
	//fprintf(logfp,"\nCepstral coefficients are\n");
	for(int m=1;m<=p;m++) {
		//fprintf(logfp,"%Lf ",c[m]);
		currC[m] = c[m];
	}
		
	return;
	
}
void readCodeBook() {
	FILE *codebookFp = NULL;
	char cbookname[200];
	sprintf(cbookname,"..\\..\\%s\\LBG_Codebook.txt",profile);
	codebookFp = fopen(cbookname,"r");
	for(int i=1;i<=M;i++) {
		for(int j=1;j<=p;j++) {
			fscanf(codebookFp,"%Lf",&obsSeqGen_codeBook[i][j]);
		}
		
	}
		
			
	fclose(codebookFp);
}
int generateObservationSequence() {//returns observation
	int observation = 1;

	/*Calculating distance for each row in codebook*/
	ld distance[M+1] = {0.0};
	for(int i=1;i<=M;i++) {
		for(int j=1;j<=p;j++) {
			ld diff = currC[j] - obsSeqGen_codeBook[i][j];
			distance[i] += diff*diff*obsSeqGen_tokhuraWeight[j-1];
		}
		//fprintf(logfp,"\ndistance[%d] = %Lf\n",i,distance[i]);
	}

	/*identifying observation*/
	ld minDistance = distance[1];
	for(int i=2;i<=M;i++) {
		if(minDistance>distance[i]) {
			minDistance = distance[i];
			observation = i;
		}
	}

	return observation;
}

