#include "stdafx.h"
#include<stdlib.h>
#include<math.h>

//This file is used for live testing of the data

int recognize() {
	extractStableFrame("live",-1,1);
		
	char fileName[200];
	sprintf(fileName,"input_extractedFrames.txt");
	FILE *fp = fopen(fileName,"r");
	int count=0;
	ld temp;
	int frames =0;
	ld sample[FRAME_SIZE+1];
	int observation[MAX_FRAME+1];
	while(!feof(fp)) {
		count++;
		fscanf(fp,"%Lf",&temp);
		tempArr1[count] = temp;
		if(count==FRAME_SIZE) {
			frames++;

			for(int i=1;i<=FRAME_SIZE;i++)
				sample[i]=tempArr1[i];
			ObservationSeqGen_calculateC(sample);
				
			observation[frames] = generateObservationSequence();
				
			count = 0;
			
		}
	}
	printf("\n");		
	for(int l=1;l<=frames;l++)
		printf("%d ",observation[l]);
	printf("\n");
	ld maxProb = 0;
	int result = -1;
	for(int i=0;i<MAX_WORDS;i++) {
		char fileNameA[200];
		char fileNameB[200];
		sprintf(fileNameA,"..\\..\\%s\\HMM\\Iteration3\\A_%s.txt",profile,words[i]);
		sprintf(fileNameB,"..\\..\\%s\\HMM\\Iteration3\\B_%s.txt",profile,words[i]);
		ld temp = forwardProcedure(frames,observation,fileNameA,fileNameB);
		if(temp>maxProb) {
			maxProb = temp;
			result = i;
		}
	}
	if(maxProb==0)
		printf("\nSorry! word cannot be recognized\n");
	else
		printf("\nThe word is %s\n",words[result]);
		
	fclose(fp);
	return result;
}

//Main of liveTesting.cpp

void liveTesting()
{
	computeDCShift();
	readCodeBook();
	int i=0;
	int flag = 0; //0 for close, 1 for open, 2 for slideshow
	while(true){
		//system("SP-project--main\\speak.vbs");
		
		printf("\n******************************************************************\n");
		printf("\nSpeak open to open the presentation\n");
		printf("\nSpeak close to close the presentation and the application\n");
		printf("\nSpeak present to go to full screen\n");
		printf("\nSpeak goto and page number to go to a specific slide\n");
		printf("\nSpeak back to go to previous slide of the presentation\n");
		printf("\nSpeak next to go to next slide of the presentation\n");
		
		system("Recording_Module\\Recording_Module.exe 3 input.wav input.txt");
		system("SP-project--main\\enter_console.vbs");
		i = recognize();
		
		if(i==0 && flag) {//goto
			if(flag!=2 && flag==1)
			{
				system("SP-project--main\\start_slideshow.vbs");
				flag = 2;
			}
			printf("\Speak page Number\n");
			system("Recording_Module\\Recording_Module.exe 3 input.wav input.txt");
			i = recognize();
			if(i==6)
                system("SP-project--main\\slideshow_goto2.vbs");
            else if(i==7)
                system("SP-project--main\\slideshow_goto3.vbs");
            else if(i==8)
                system("SP-project--main\\slideshow_goto4.vbs");
            else if(i==9)
                system("SP-project--main\\slideshow_goto5.vbs");
            else if(i==10)
                system("SP-project--main\\slideshow_goto6.vbs");
            else
                printf("\nSorry! word cannot be recognized\n");
		}
		else if(i==1) {//open
            system("SP-project--main\\open_ppt.vbs");
            flag = 1;
        }
        else if(i==2 && flag==1) {//next
            system("SP-project--main\\next_slide.vbs");
        }
        else if(i==3 && flag!=0) {//back
            if(flag==1)
                system("SP-project--main\\previous_slide.vbs");
            else
                system("SP-project--main\\slideshow_back.vbs");
        }
        else if(i==4 && flag!=0) {//close
            system("SP-project--main\\close_ppt.vbs");
            flag = 0;
        }
        else if(i==10 && flag==1) {//present
            system("SP-project--main\\start_slideshow.vbs");
                flag=2;
        }
        else if(i==4 && flag==0) {//close
            break;
        }
	}
	
	system("pause");
//	return 0;
}

