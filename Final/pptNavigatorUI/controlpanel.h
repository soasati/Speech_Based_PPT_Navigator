#include "stdafx.h"
#include<stdlib.h>
#include<math.h>

//Main of controlPanel.cpp

//This file is used to generate the observation sequence for the files

void controlPanel()
{
	computeDCShift();
	//Extract stable frame
	for(int i =0;i<MAX_WORDS;i++)
		for(int j =1;j<=MAX_UTTERANCE;j++)
			extractStableFrame(words[i],j,0);//0 for training
	//creating universe from extracted data
	char universename[50];
	sprintf(universename,"..\\..\\%s\\universe.txt",profile);
	FILE *fp = fopen(universename,"w");
	for(int i =0;i<MAX_WORDS;i++)
		for(int j =1;j<=MAX_UTTERANCE;j++) {
			char fileName[200];
			sprintf(fileName,"..\\..\\%s\\extractedFrames\\%s_%d_EF.txt",profile,words[i],j);
			FILE *fp1 = fopen(fileName,"r");
			ld sample[FRAME_SIZE+1];
			int count = 0,frames=0;
			ld temp;
			while(!feof(fp1)) {
				count++;
				fscanf(fp1,"%Lf",&temp);
				tempArr1[count] = temp;
				if(count==FRAME_SIZE) {
					frames++;

					for(int i=1;i<=FRAME_SIZE;i++)
						sample[i]=tempArr1[i];
					calculateC(sample,fp);
				
					count = 0;
			
				}
			}
			fclose(fp1);
		}
	fclose(fp);

	//creating codebook
	LBG();

	//HMM training
	readCodeBook();
	for(int i =0;i<MAX_WORDS;i++){
		int T[MAX_UTTERANCE+1];
		int obseravtion[MAX_UTTERANCE+1][MAX_FRAME+1];
		ld sample[FRAME_SIZE+1];
		for(int j =1;j<=MAX_UTTERANCE;j++) {
			char fileName[200];
			sprintf(fileName,"..\\..\\%s\\extractedFrames\\%s_%d_EF.txt",profile,words[i],j);
			fp = fopen(fileName,"r");
			int count=0;
			ld temp;
			int frames =0;
			while(!feof(fp)) {
				count++;
				fscanf(fp,"%Lf",&temp);
				tempArr1[count] = temp;
				if(count==FRAME_SIZE) {
					frames++;

					for(int i=1;i<=FRAME_SIZE;i++)
						sample[i]=tempArr1[i];
					ObservationSeqGen_calculateC(sample);
				
					obseravtion[j][frames] = generateObservationSequence();
				
					count = 0;
			
				}
			}
			T[j] = frames;
			for(int l=1;l<=frames;l++)
				printf("%d ",obseravtion[j][l]);
			printf("\n");
			sprintf(fileName,"..\\..\\%s\\ObservationSequence\\%s_%d_OS.txt",profile,words[i],j);
			FILE *fp1 = fopen(fileName,"w");
			for(int l=1;l<=frames;l++)
				fprintf(fp1,"%d ",obseravtion[j][l]);
			
			fclose(fp1);
			fclose(fp);

			
		}

	}
	system("pause");
//	return 0;
}

