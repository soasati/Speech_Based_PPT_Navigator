#include "stdafx.h"
#include<stdlib.h>
#include<math.h>
//#include "CommonVariables.h"
//#include "ObservationSequenceGenerator.h"
//#include "generateUniverse.h"
//#include "LBG.h"
//#include "hmm.h"


//This file is used to do the live training of the data

void liveTraining() {
	
	char word[100],fileName[500];
	printf("\nEnter word to train:\n");
	scanf("%s",&word);
	//Record 10 times and form universe and codebook and HMM
	for(int i=1;i<=10;i++) {
		
		system("Recording_Module\\Recording_Module.exe 3 input.wav input.txt");
		system("SP-project--main\\enter_console.vbs");
		FILE *fptr1, *fptr2;
		char filename[100], c;
  
		// Open one file for reading
		fptr1 = fopen("input.txt", "r");
		if (fptr1 == NULL)
		{
			printf("Cannot open recorded file \n");
			exit(0);
		}
		sprintf(filename,"..\\..\\%s\\Dataset\\%s_%d.txt",profile,word,i);
		fptr2 = fopen(filename, "w");
		if (fptr2 == NULL)
		{
			printf("Cannot open file %s \n", filename);
			exit(0);
		}
  
		// Read contents from file
		c = fgetc(fptr1);
		while (c != EOF)
		{
			fputc(c, fptr2);
			c = fgetc(fptr1);
		}
  
   
  
		fclose(fptr1);
		fclose(fptr2);
		
		extractStableFrame(word,i,1);//1 for live data
	}
	

	//Generate universe
	//creating universe from extracted data
	char universename[200];
	sprintf(universename,"..\\..\\%s\\Universe.txt",profile);
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
	hmm();

}