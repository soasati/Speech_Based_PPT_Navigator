// HMM1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<stdlib.h>
#include<math.h>
//#include "CommonVariables.h"
//This file is used to train the HMM models for the files
void initialize() {

	int i,j,l;

	for(i=0;i<=Tmax;i++) {
		qStar[i] = 0;
		q[i] = 0;
		for(j=0;j<=N;j++) {
			alpha[i][j] = 0.0;
			beta[i][j] = 0.0;
			gamma[i][j] = 0.0;
			delta[i][j] = 0.0;
			psi[i][j] = 0;
			for(l=0;l<=N;l++) {
				xi[i][j][l] = 0.0;
			}
		}
	}

	probabilityOfOGivenLambda = 0.0;
}
//This file is used to train the HMM models for the files

void forwardProcedure(int T,int O[]) {
	int i,j,t;
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
	probabilityOfOGivenLambda = 0.0;
	//termination
	for(i=1;i<=N;i++)
		probabilityOfOGivenLambda+=alpha[T][i];


	//printing probability of O given lambda
	fprintf(logFp,"\nProbability of O given lambda is %.32e\n",probabilityOfOGivenLambda);
}
 
void backwardProcedure(int T,int O[]) {
	int i,j,t,k1;
	ld denominator = 0.0;
	//initialization
	for(i=1;i<=N;i++)
		beta[T][i] = 1.0;

	//Induction
	for(t=T-1;t>=0;t--) {
		for(i=1;i<=N;i++) {
			for(j=1;j<=N;j++) {
				beta[t][i] += (A[i][j]*B[j][O[t+1]]*beta[t+1][j]);
			}
		}
	}


	//Calculating gamma
	for(t=1;t<=T;t++) {
		for(i=1;i<=N;i++) {
			denominator = 0.0;
			for(k1=1;k1<=N;k1++) {
				denominator += alpha[t][k1] * beta[t][k1];
			}
			gamma[t][i] = alpha[t][i] * beta[t][i]/denominator;
		}
	}

	//calculating q
	for(t=1;t<=T;t++) {
		q[t] = 1;
		for(i=1;i<=N;i++) {
			if(gamma[t][i]>gamma[t][q[t]])
				q[t] = i;
		}

	}

}
void makeStochastic(int T) {
	int i,j;
	ld maxValue = 0.0,rowSum=0.0;
	int maxColIndex=0;
	//Making A stochastic
	for(i=1;i<=N;i++) {
		maxValue = -1.0;
		maxColIndex = 0;
		rowSum = 0.0;
		for(j=1;j<=N;j++) {
			if(A_New[i][j]>maxValue) {
				maxValue = A_New[i][j];
				maxColIndex = j;
			}
			rowSum+=A_New[i][j];

		}
		if(rowSum!=(ld)1)
			A_New[i][maxColIndex]+=((ld)1-rowSum);
	}
	
	//Making B_NewG stochastic
	for(i=1;i<=N;i++) {
		maxValue = -1.0;
		maxColIndex = 0;
		rowSum = 0.0;
		for(j=1;j<=M;j++) {
			if(B_NewG[i][j]>maxValue) {
				maxValue = B_NewG[i][j];
				maxColIndex = j;
			}
			rowSum+=B_NewG[i][j];

		}
		if(rowSum!=(ld)1)
			B_NewG[i][maxColIndex]+=((ld)1-rowSum);
	}

}


void viterbiAlgorithm(int T,int O[]) {
	int i,j,t;

	//Viterbi Algorithm
	//Initialization
	for(i=1;i<=N;i++) {
		delta[1][i] = pi[i] * B[i][O[1]];
		psi[1][i] = 0;
	}


	//Recursion
	ld maxDelta = 0.0;
	int index=1;
	for(t=2;t<=T;t++) {
		for(j=1;j<=N;j++) {
			maxDelta = 0.0;
			index=1;
			for(i=1;i<=N;i++) {
				if(delta[t-1][i]*A[i][j]>maxDelta) {
					maxDelta = delta[t-1][i]*A[i][j];
					index = i;
					
				}
				
			}
			delta[t][j] = maxDelta*B[j][O[t]];
			psi[t][j] = index;
		}
	}


	//Termination
	pStarCurr = delta[T][1];
	qStar[T] = 1;
	for(i=1;i<=N;i++) {
		if(delta[T][i]>pStarCurr) {
			pStarCurr = delta[T][i];
			qStar[T] = i;
		}
	}


	//State Sequence (Path) Backtracking
	for(t=T-1;t>=1;t--)
		qStar[t] = psi[t+1][qStar[t+1]];

	//printing qStar in log file
	fprintf(logFp,"\nqStar printing\n");
	for(i=1;i<=T;i++) 
		fprintf(logFp,"%d ",qStar[i]);
	fprintf(logFp,"\nP star is %.32e\n",pStarCurr);

}
void calculateXi(int T,int O[]) {
	int i,j,t;
	ld denominator = 0.0,numerator = 0.0;
	//xi calculation
	denominator=0.0;
	

	for(t=1;t<=T-1;t++) {
		denominator=0.0;
		for(i=1;i<=N;i++) {
			for(j=1;j<=N;j++) {
				denominator+=alpha[t][i]*A[i][j]*B[j][O[t+1]]*beta[t+1][j];
			}
		}
		
		for(i=1;i<=N;i++) {
			for(j=1;j<=N;j++) {
				xi[t][i][j] = alpha[t][i]*A[i][j]*B[j][O[t+1]]*beta[t+1][j]/denominator;
			}
		}
	}
}

void reestimateLambda(int T,int O[]) {
	int i=1,j=1,t=1,k1=1;
	ld denominator=0.0,numerator=0.0,rowSum=0.0,maxValue = -1.0;
	int maxColIndex = 0,den = 0,num = 0;

	//Re estimation of A
	for(i=1;i<=N;i++) {
		for(j=1;j<=N;j++) {
			numerator = 0.0;
			denominator = 0.0;
			for(t=1;t<=T-1;t++) {
				numerator+=xi[t][i][j];
				denominator+=gamma[t][i];
			}
			A_New[i][j] = numerator/denominator;
			
		}
	}

	//B reestimation using gamma
	for(j=1;j<=N;j++) {
		for(k1=1;k1<=M;k1++) {
			denominator = 0.0;
			numerator = 0.0;
			for(t=1;t<=T;t++) {
				denominator+=gamma[t][j];
				//if(O[t]==k1 && qStar[t]==j)
				if(O[t]==k1)
					numerator+=gamma[t][j];
			}
			B_NewG[j][k1]= numerator/denominator;
			
			if(B_NewG[j][k1]<threshold)
				B_NewG[j][k1] = threshold;
		}
	}

}
void HMM(int iteration,int T,char word[],int occurrence,char *fileNameA,char *fileNameB,char *fileNamePi,int O[])
{
	int i=1,j=1,t=1,k1=1;
	FILE *fp = NULL;
	ld denominator=0.0,numerator=0.0,rowSum=0.0,maxValue = -1.0;
	int maxColIndex = 0,den = 0,num = 0;
	char fileName[100];
	
	logFp = fopen("logHMM.txt","a");
	fprintf(logFp,"\n-------------------HMM on %s %d file-------------------\n",word,occurrence);
	//Reading pi
	fp = fopen(fileNamePi,"r");
	if(!fp)
		return;
	for(i=1;i<=N;i++)
		fscanf(fp,"%Lf",&pi[i]);

	fclose(fp);
	//Reading A
	fp = fopen(fileNameA,"r");
	if(!fp)
		return;
	for(i=1;i<=N;i++)
		for(j=1;j<=N;j++)
			fscanf(fp,"%Lf",&A[i][j]);

	fclose(fp);
		


	//Reading B
	fp = fopen(fileNameB,"r");
	if(!fp)
		return;
	for(i=1;i<=N;i++)
		for(j=1;j<=M;j++)
			fscanf(fp,"%Lf",&B[i][j]);

	fclose(fp);

	



	
	int numOfIteration = 1;
	
	while(numOfIteration<=100) {

		fprintf(logFp,"\n---------------------Iteration %d---------------------\n",numOfIteration);
		initialize();
		//forward procedure

		forwardProcedure(T,O);

		//Backward procedure

		backwardProcedure(T,O);


		//Viterbi algorithm
		viterbiAlgorithm(T,O);

		//calculate xi
		calculateXi(T,O);
	
		reestimateLambda(T,O);
	
	
		makeStochastic(T);
		
		/*if(numOfIteration>1 && pStarCurr/pStarPrev<=100)
			break;*/

		
		//update A with A_New
		for(i=1;i<=N;i++)
			for(j=1;j<=N;j++)
				A[i][j] = A_New[i][j];
		//Update B with B_NewG
		for(i=1;i<=N;i++)
			for(j=1;j<=M;j++)
				B[i][j] = B_NewG[i][j];
		if(numOfIteration>=2 && pStarCurr/pStarPrev<=1+1e-9)
			break;
		pStarPrev = pStarCurr;
		
		numOfIteration++;
	}

	//printing A_New in a file
	sprintf(fileName, "..\\..\\%s\\HMM\\Iteration%d\\A_%s_%d.txt",profile,iteration,word,occurrence);	
	fp = fopen(fileName,"w");
	for(i=1;i<=N;i++) {
		for(j=1;j<=N;j++) {
			fprintf(fp,"%.32e ",A_New[i][j]);
		}
		fprintf(fp,"\n");
	}
	fclose(fp);


	//printing B_NewG in a file
	sprintf(fileName, "..\\..\\%s\\HMM\\Iteration%d\\B_%s_%d.txt",profile,iteration,word,occurrence);	
	fp = fopen(fileName,"w");
	for(i=1;i<=N;i++) {
		for(j=1;j<=M;j++) {
			fprintf(fp,"%.32e ",B_NewG[i][j]);
		}
		fprintf(fp,"\n");
	}
	fclose(fp);
	if(iteration==3) {
		sprintf(fileName,"FinalHMM_%s.txt",word);
		fp = fopen(fileName,"w");
		fprintf(fp,"\n-------------------HMM on %s %d file-------------------\n",word,occurrence);
		fprintf(fp,"\nProbability of O given lambda %.32e\n",probabilityOfOGivenLambda);
		fprintf(fp,"\nP star is %.32e\n",pStarCurr);
		fclose(fp);
	}
	
	fclose(logFp);
	return;
}


//HMMTraining.cpp

void calculateAvgModel(char word[],int iteration) {
	const int n=5,m=32;
	ld A[n+1][n+1] = {{0.0}}, B[n+1][m+1] = {{0.0}};
	char fileName[100];
	int i=1,j=1;
	FILE *fp1 = NULL,*fp2 = NULL;
	int occurrence = 1;
	for(occurrence = 1;occurrence<=MAX_UTTERANCE;occurrence++) {
		sprintf(fileName, "..\\..\\%s\\HMM\\Iteration%d\\A_%s_%d.txt",profile,iteration,word,occurrence);	
		fp1 = fopen(fileName,"r");
		
		ld temp = 0.0;
		for(i=1;i<=n;i++) {
			for(j=1;j<=n;j++) {
				fscanf(fp1,"%Lf",&temp);
				A[i][j]+=temp;
			}
		}
		fclose(fp1);
		
		sprintf(fileName, "..\\..\\%s\\HMM\\Iteration%d\\B_%s_%d.txt",profile,iteration,word,occurrence);	
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
	sprintf(fileName, "..\\..\\%s\\HMM\\Iteration%d\\A_%s.txt",profile,iteration,word);	
	fp2 = fopen(fileName,"w");
	for(i=1;i<=n;i++) {
		for(j=1;j<=n;j++) {
			fprintf(fp2,"%.32e ",A[i][j]);
		}
		fprintf(fp2,"\n");
	}
	fclose(fp2);
	//writing B in file
	sprintf(fileName, "..\\..\\%s\\HMM\\Iteration%d\\B_%s.txt",profile,iteration,word);	
	fp2 = fopen(fileName,"w");
	for(i=1;i<=n;i++) {
		for(j=1;j<=m;j++) {
			fprintf(fp2,"%.32e ",B[i][j]);
		}
		fprintf(fp2,"\n");
	}
	fclose(fp2);

}

//main of HMMTraining.cpp

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
			sprintf(fileName,"..\\..\\%s\\ObservationSequence\\%s_%d_OS.txt",profile,words[i],j);
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
			char fileNameA[200];
			char fileNameB[200];
			char fileNamepi[200];
			sprintf(fileName, "..\\..\\%s\\Dataset\\%s_%d.txt",profile,words[i],j);	
			sprintf(fileNameA,"..\\..\\%s\\HMM\\InitialModelA.txt",profile);
			sprintf(fileNameB,"..\\..\\%s\\HMM\\InitialModelB.txt",profile);
			sprintf(fileNamepi,"..\\..\\%s\\HMM\\InitialModelPi.txt",profile);
			fprintf(logfp,"\n--------------HMM on %s--------------",fileName);
			HMM(1,T[j],words[i],j,fileNameA,fileNameB,fileNamepi,obseravtion[j]);

		}
		//calculate avg of all models
		calculateAvgModel(words[i],1);
		for(int j =1;j<=MAX_UTTERANCE;j++) {
			//do HMM
			char fileName[200];
			char fileName1[200];
			char fileNamepi[200];
			sprintf(fileName, "..\\..\\%s\\Dataset\\%s_%d.txt",profile,words[i],j);	
			fprintf(logfp,"\n--------------HMM on %s--------------",fileName);
			sprintf(fileName, "..\\..\\%s\\HMM\\Iteration1\\A_%s.txt",profile,words[i]);	
			sprintf(fileName1, "..\\..\\%s\\HMM\\Iteration1\\B_%s.txt",profile,words[i]);
			sprintf(fileNamepi, "..\\..\\%s\\HMM\\InitialModelPi.txt",profile);
			HMM(2,T[j],words[i],j,fileName,fileName1,fileNamepi,obseravtion[j]);
		}
		//calculate avg of all models
		calculateAvgModel(words[i],2);
		for(int j =1;j<=MAX_UTTERANCE;j++) {
			//do HMM
			char fileName[200];
			char fileName1[200];
			char fileNamepi[200];
			sprintf(fileName, "..\\..\\%s\\Dataset\\%s_%d.txt",profile,words[i],j);	
			fprintf(logfp,"\n--------------HMM on %s--------------",fileName);
			sprintf(fileName, "..\\..\\%s\\HMM\\Iteration2\\A_%s.txt",profile,words[i]);	
			sprintf(fileName1, "..\\..\\%s\\HMM\\Iteration2\\B_%s.txt",profile,words[i]);
			sprintf(fileNamepi, "..\\..\\%s\\HMM\\InitialModelPi.txt",profile);
			HMM(3,T[j],words[i],j,fileName,fileName1,fileNamepi,obseravtion[j]);
		}
		calculateAvgModel(words[i],3);
	}
	fclose(logfp);
//	return 0;
} 

