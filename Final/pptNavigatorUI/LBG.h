#include "stdafx.h"
#include<stdlib.h>
#include<math.h>

typedef long double ld;
#define p 12//We have 12 c values(cepstral coeeficients)
#define k 32//codebook size
#define deltaLBG 0.0001
#define epsilon 0.03
ld codeBook[k][p] = {{0.0}};
ld universe[p] = {0.0};
//FILE *logFp = NULL;
ld tokhuraWeight[] = {1.0, 3.0, 7.0, 13.0, 19.0, 22.0, 25.0, 33.0, 42.0, 50.0, 56.0, 61.0};

//this file is used for the generation of codebook using LBG algorithm

ld kMeans(ld codeBook[k][p],int codeBookSize) { 
	int i=0,j=0,l=0,y=0;
	int regionSize[k] = {0};
	ld diff = 0.0,minDist = 0.0;
	ld currentDistortion = 0.0;
	char universename[200];
	sprintf(universename,"..\\..\\%s\\Universe.txt",profile);
	FILE *universeFp = fopen(universename,"r");
	int rows = -1;
	ld newCodeBook[k][p] = {{0.0}};
	
	//calculating distance between each universe vector and codebook vector to map universe vector in a cluster and calculate total distortion with this codebook
	while(!feof(universeFp)) {
		rows++;
		ld distance[k] = {0.0};
		for(l=0;l<p;l++) 
			fscanf(universeFp,"%Lf",&universe[l]);
		for(j=0;j<codeBookSize;j++) {
			for(l=0;l<p;l++) {
				diff = universe[l] - codeBook[j][l];
				distance[j] += tokhuraWeight[l]*diff*diff;
			}

		}

		//y stores index of minimum codebook vector for a universe vector
		minDist = distance[0];
		y = 0;
		for(j=1;j<codeBookSize;j++) {
			if(minDist>distance[j]) {
				minDist = distance[j];
				y = j;
			}
		}


		//updating region size
		regionSize[y]++;


		//updating current distortion
		currentDistortion+=minDist;

		//updating new codebook
		for(l=0;l<p;l++)
			newCodeBook[y][l]+=universe[l];

	}
	currentDistortion/=(rows*1.000000);//currentDistortion holds avg distortion with this codebook
	fprintf(logFp,"\nCluster size\n");
	for(int i=0;i<codeBookSize;i++)
		fprintf(logFp,"%d ",regionSize[i]);

	
	
	//calculate centroid of each region and update codebook
	for(i=0;i<codeBookSize;i++) {   
		for(l=0;l<p;l++) {
			newCodeBook[i][l] = newCodeBook[i][l]/regionSize[i];
			codeBook[i][l] = newCodeBook[i][l];
			
		}
	}
	

	//removing empty cell problem
	for(i=0;i<codeBookSize;i++) {
		if(regionSize[i]==0) {
			int largeRegion=0,largeRegionSize = regionSize[0];
			for(j=1;j<codeBookSize;j++)
				if(regionSize[j]>largeRegionSize) {
					largeRegionSize = regionSize[j];
					largeRegion = j;
				}

			for(j=0;j<p;j++) {
				codeBook[i][j] = codeBook[largeRegion][j]*(1+epsilon);
				codeBook[largeRegion][j] = codeBook[largeRegion][j]*(1-epsilon);
			}
				

		}
	}
	fclose(universeFp);
	return currentDistortion;
}



void LBG()
{
	int i=0,j=0,row=0,m=0;
	int codeBookSize = 1;
	ld oldCodeBook[k][p] = {0};
	ld prevDistortion = 0.0;
	ld currentDistortion = 0.0;
	int rows = -1;
	FILE *fpd = fopen("DistortionLBG.txt","w");
	logFp = fopen("log_LBG.txt","w");

	/*Reading universe data in a matrix*/
	char universename[200];
	sprintf(universename,"..\\..\\%s\\Universe.txt",profile);
	FILE *universeFp = fopen(universename,"r");
	if(!universeFp) {
		fprintf(logFp,"\nOops! universe file not found");
		fclose(fpd);
		fclose(logFp);
		return;
	}
	//initial codebook
	while(!feof(universeFp)) {
		rows++;
		for(j=0;j<p;j++) {
			fscanf(universeFp,"%Lf",&universe[j]);
			codeBook[0][j]+=universe[j];
		}
	}
	fclose(universeFp);
	
	
	
	for(j=0;j<p;j++) {
		codeBook[0][j] /= (long double) rows;
	}

	//printing initial codebook
	fprintf(logFp,"\nPrinting initial codebook\n");
	for(i=0;i<p;i++)
		fprintf(logFp,"%Lf ",codeBook[0][i]);

	printf("\n");

	//distortion when codebook size is 1,codebook contains centroid of the universe
	currentDistortion = kMeans(codeBook,codeBookSize);
	fprintf(logFp,"\nPrinting overall distortion %Lf\n",currentDistortion);
	fprintf(fpd,"%Lf\n",currentDistortion);
	fprintf(logFp,"--------------------------------------------------------");

	prevDistortion = 9999.0;

	//LBG algorithm
	while(codeBookSize<k) {

		//old code book
		for(i=0;i<codeBookSize;i++)
			for(j=0;j<p;j++)
				oldCodeBook[i][j] = codeBook[i][j];
		

		//double the codebook
		row=0;
		for(i=0;i<codeBookSize;i++) {
			for(j=0;j<p;j++) {
				codeBook[row][j] = oldCodeBook[i][j]*(1+epsilon);
				codeBook[row+1][j] = oldCodeBook[i][j]*(1-epsilon);
			}
			row+=2;
		}
			
	     codeBookSize*=2;

		 //K means
		 
		 while(true) { //or while(true) will be more appropriate
			//printing codebook
			fprintf(logFp,"\nPrinting code book \n");
			for(i=0;i<codeBookSize;i++) {
				for(j=0;j<p;j++)  {
					fprintf(logFp,"%Lf ",codeBook[i][j]);
				}
				fprintf(logFp,"\n");
			}
			currentDistortion = kMeans(codeBook,codeBookSize);
			m=m+1;

			fprintf(logFp,"\nPrinting overall distortion %Lf\n",currentDistortion);
			fprintf(fpd,"%Lf\n",currentDistortion);
			fprintf(logFp,"--------------------------------------------------------");

			if(abs(prevDistortion-currentDistortion)<=deltaLBG)
				break;
			else 
				prevDistortion = currentDistortion;

			

		}

		
	}
	fprintf(logFp,"\nPrinting number of iterations %d\n",m);



	//Printing final codebook
	fprintf(logFp,"\nPrinting final codebook\n");
	for(i=0;i<k;i++){
		for(j=0;j<p;j++) 
			fprintf(logFp,"%Lf ",codeBook[i][j]);
		fprintf(logFp,"\n");
	}
	char cbookname[200];
	sprintf(cbookname,"..\\..\\%s\\LBG_Codebook.txt",profile);
	FILE *fp = fopen(cbookname, "w");
	if(!fp) {
		fprintf(logFp,"\nOops! file could not be opened for writing codebook");
		fclose(fpd);
		fclose(logFp);
		return;
	}

	//Printing final codebook
	for(i=0;i<k;i++){
		for(j=0;j<p;j++) 
			fprintf(fp,"%Lf ",codeBook[i][j]);
		fprintf(fp,"\n");
	}
	
	fclose(fp);
	fclose(fpd);
	fclose(logFp);


	return;
}

