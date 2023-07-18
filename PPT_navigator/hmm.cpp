// HMMTraining.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "hmm.h"
typedef long double ld;
#define MAX_WORDS 12
#define MAX_UTTERANCE 25
#define FRAME_SIZE 320
#define MAX_FRAME 500
//char words[MAX_WORDS][100] = {"zero","one","two","three","four","five","six","seven","eight","nine"};
//char words[MAX_WORDS][100] = {"one","two","three","four","five","six","seven","nine"};
char words[MAX_WORDS][100] = {"goto","open","next","back","close","one","two","three","four","five","six","seven"};
void calculateAvgModel(char word[],int iteration) {
	const int n=5,m=32;
	ld A[n+1][n+1] = {{0.0}}, B[n+1][m+1] = {{0.0}};
	char fileName[100];
	int i=1,j=1;
	FILE *fp1 = NULL,*fp2 = NULL;
	int occurrence = 1;
	for(occurrence = 1;occurrence<=MAX_UTTERANCE;occurrence++) {
		sprintf(fileName, "..\\..\\Data\\HMM\\Iteration%d\\A_%s_%d.txt",iteration,word,occurrence);	
		fp1 = fopen(fileName,"r");
		
		ld temp = 0.0;
		for(i=1;i<=n;i++) {
			for(j=1;j<=n;j++) {
				fscanf(fp1,"%Lf",&temp);
				A[i][j]+=temp;
			}
		}
		fclose(fp1);
		
		sprintf(fileName, "..\\..\\Data\\HMM\\Iteration%d\\B_%s_%d.txt",iteration,word,occurrence);	
		fp1 = fopen(fileName,"r");
		temp = 0.0;
		for(i=1;i<=n;i++) {
			for(j=1;j<=m;j++) {
				fscanf(fp1,"%Lf",&temp);
				B[i][j]+=temp;
			}
		}
		fclose(fp1);
	}
	for(i=1;i<=n;i++)
		for(j=1;j<=n;j++)
			A[i][j]/=MAX_UTTERANCE;
	for(i=1;i<=n;i++)
		for(j=1;j<=m;j++)
			B[i][j]/=MAX_UTTERANCE;
	//Making A stochastic
	ld maxValue = 0.0,rowSum=0.0;
	int maxColIndex=0;
	for(i=1;i<=n;i++) {
		maxValue = -1.0;
		maxColIndex = 0;
		rowSum = 0.0;
		for(j=1;j<=n;j++) {
			if(A[i][j]>maxValue) {
				maxValue = A[i][j];
				maxColIndex = j;
			}
			rowSum+=A[i][j];

		}
		if(rowSum!=(ld)1)
			A[i][maxColIndex]+=((ld)1-rowSum);
	}
	
	//Making B stochastic
	for(i=1;i<=n;i++) {
		maxValue = -1.0;
		maxColIndex = 0;
		rowSum = 0.0;
		for(j=1;j<=m;j++) {
			if(B[i][j]>maxValue) {
				maxValue = B[i][j];
				maxColIndex = j;
			}
			rowSum+=B[i][j];

		}
		if(rowSum!=(ld)1)
			B[i][maxColIndex]+=((ld)1-rowSum);
	}
	//writing A in file
	sprintf(fileName, "..\\..\\Data\\HMM\\Iteration%d\\A_%s.txt",iteration,word);	
	fp2 = fopen(fileName,"w");
	for(i=1;i<=n;i++) {
		for(j=1;j<=n;j++) {
			fprintf(fp2,"%.32e ",A[i][j]);
		}
		fprintf(fp2,"\n");
	}
	fclose(fp2);
	//writing B in file
	sprintf(fileName, "..\\..\\Data\\HMM\\Iteration%d\\B_%s.txt",iteration,word);	
	fp2 = fopen(fileName,"w");
	for(i=1;i<=n;i++) {
		for(j=1;j<=m;j++) {
			fprintf(fp2,"%.32e ",B[i][j]);
		}
		fprintf(fp2,"\n");
	}
	fclose(fp2);

}
void hmm()
{
	FILE *logfp = fopen("logTraining.txt","w");
	for(int i =0;i<MAX_WORDS;i++){
		int T[MAX_UTTERANCE+1];
		int obseravtion[MAX_UTTERANCE+1][MAX_FRAME+1];
		for(int j =1;j<=MAX_UTTERANCE;j++) {
			//Read observation sequence
			int temp = 0;
			char fileName[200];
			sprintf(fileName,"..\\..\\Data\\ObservationSequence\\%s_%d_OS.txt",words[i],j);
			FILE *fp = fopen(fileName,"r");
			while(!feof(fp) && temp<MAX_FRAME) {
				temp++;
				fscanf(fp,"%d",&obseravtion[j][temp]);
			}
			T[j] = temp-1;
			fclose(fp);
		}
		//HMM
		for(int j =1;j<=MAX_UTTERANCE;j++) {
			//do HMM
			char fileName[200];
			sprintf(fileName, "..\\..\\Data\\Dataset\\%s_%d.txt",words[i],j);	
			fprintf(logfp,"\n--------------HMM on %s--------------",fileName);
			HMM(1,T[j],words[i],j,"..\\..\\Data\\HMM\\InitialModelA.txt","..\\..\\Data\\HMM\\InitialModelB.txt","..\\..\\Data\\HMM\\InitialModelPi.txt",obseravtion[j]);

		}
		//calculate avg of all models
		calculateAvgModel(words[i],1);
		for(int j =1;j<=MAX_UTTERANCE;j++) {
			//do HMM
			char fileName[200];
			char fileName1[200];
			sprintf(fileName, "..\\..\\Data\\Dataset\\%s_%d.txt",words[i],j);	
			fprintf(logfp,"\n--------------HMM on %s--------------",fileName);
			sprintf(fileName, "..\\..\\Data\\HMM\\Iteration1\\A_%s.txt",words[i]);	
			sprintf(fileName1, "..\\..\\Data\\HMM\\Iteration1\\B_%s.txt",words[i]);
			HMM(2,T[j],words[i],j,fileName,fileName1,"..\\..\\Data\\HMM\\InitialModelPi.txt",obseravtion[j]);
		}
		//calculate avg of all models
		calculateAvgModel(words[i],2);
		for(int j =1;j<=MAX_UTTERANCE;j++) {
			//do HMM
			char fileName[200];
			char fileName1[200];
			sprintf(fileName, "..\\..\\Data\\Dataset\\%s_%d.txt",words[i],j);	
			fprintf(logfp,"\n--------------HMM on %s--------------",fileName);
			sprintf(fileName, "..\\..\\Data\\HMM\\Iteration2\\A_%s.txt",words[i]);	
			sprintf(fileName1, "..\\..\\Data\\HMM\\Iteration2\\B_%s.txt",words[i]);
			HMM(3,T[j],words[i],j,fileName,fileName1,"..\\..\\Data\\HMM\\InitialModelPi.txt",obseravtion[j]);
		}
		calculateAvgModel(words[i],3);
	}
	fclose(logfp);
	return 0;
} 

