#include "stdafx.h"
#include<stdlib.h>
#include<math.h>
#include <Windows.h>

#include "mmsystem.h"
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "winmm.lib")
#define SAMPLERATE 16000
#define CHANNELS 1
#define RESOLUTION 16
#define COMMAND_DURATION 3
#define WORD_DURATION 3

//This file is used for live testing of the data

/*
	Start recording for particular duration length
*/
static void  StartRecord(short int *data,int length)
{
    printf("Start Recording...\n");
//	fprintf(fp_log, "Start Recording...\n");
	HWAVEIN hWaveIn;
	WAVEHDR WaveInHdr;

	WAVEFORMATEX pFormat;
	pFormat.wFormatTag=WAVE_FORMAT_PCM;                         // simple, uncompressed format
	pFormat.nChannels=CHANNELS;                                 //  1=mono, 2=stereo
	pFormat.nSamplesPerSec=SAMPLERATE;                          // 16000
	pFormat.nAvgBytesPerSec=SAMPLERATE * 2;                     // = nSamplesPerSec * n.Channels * wBitsPerSample/8
	pFormat.wBitsPerSample=RESOLUTION;                          //  16 for high quality, 8 for telephone-grade
	pFormat.nBlockAlign=CHANNELS * RESOLUTION / 8;              // = n.Channels * wBitsPerSample/8
	pFormat.cbSize=0;

	waveInOpen(&hWaveIn, WAVE_MAPPER,&pFormat,0L, 0L, WAVE_FORMAT_DIRECT);

	WaveInHdr.lpData = (LPSTR)data;
	WaveInHdr.dwBufferLength = length * 2;
	WaveInHdr.dwBytesRecorded=0;
	WaveInHdr.dwUser = 0L;
	WaveInHdr.dwFlags = 0L;
	WaveInHdr.dwLoops = 0L;
	waveInPrepareHeader(hWaveIn, &WaveInHdr, sizeof(WAVEHDR));

	waveInAddBuffer(hWaveIn, &WaveInHdr, sizeof(WAVEHDR));

	waveInStart(hWaveIn);
	while(waveInUnprepareHeader(hWaveIn,&WaveInHdr,sizeof(WAVEHDR)) == WAVERR_STILLPLAYING);
	waveInClose(hWaveIn);
}

/*
	Save recording in text file to predict given command
*/
void static SaveRecording(short int *data, int size, char *file)
{
    printf("Saving...\n");
//	fprintf(fp_log, "Saving...\n");
    FILE *f=fopen(file,"w");
    for(int i=0; i<size; i++){
        fprintf(f, "%hi\n", data[i]);
    }
    fclose(f);
}

/*
	Record a command from user
*/
static void  recording(char *file, int duration)
{
    int size = (SAMPLERATE * duration);
	short int *data = (short int *)malloc(sizeof(short int)*size);
    StartRecord(data, size);
    SaveRecording(data, size, file);
}



//code for recognizing word

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
	/*for(int l=1;l<=frames;l++)
		printf("%d ",observation[l]);
	printf("\n");*/
	ld maxProb = 0;
	int result = -1;
	for(int i=0;i<MAX_WORDS;i++) {
		char fileNameA[200];
		char fileNameB[200];
		sprintf(fileNameA,"..\\..\\%s\\HMM\\Iteration3\\A_%s.txt",profile,words[i]);
		sprintf(fileNameB,"..\\..\\%s\\HMM\\Iteration3\\B_%s.txt",profile,words[i]);
		ld temp = forwardProcedure(frames,observation,fileNameA,fileNameB);
		//printf("\n%.32e",temp);
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
	switch(result) {
		case -1:
			ShellExecuteA(NULL, "open", "SP-project--main\\speak_notrecog.vbs", NULL, NULL, SW_HIDE);
			break;
		case 0:
			ShellExecuteA(NULL, "open", "SP-project--main\\speak_goto.vbs", NULL, NULL, SW_HIDE);
			break;
		case 1:
			ShellExecuteA(NULL, "open", "SP-project--main\\speak_open.vbs", NULL, NULL, SW_HIDE);
			break;
		case 2:
			ShellExecuteA(NULL, "open", "SP-project--main\\speak_slidenext.vbs", NULL, NULL, SW_HIDE);
			break;
		case 3:
			ShellExecuteA(NULL, "open", "SP-project--main\\speak_back.vbs", NULL, NULL, SW_HIDE);
			break;
		case 4:
			ShellExecuteA(NULL, "open", "SP-project--main\\speak_close_recog.vbs", NULL, NULL, SW_HIDE);
			break;
		case 5:
			ShellExecuteA(NULL, "open", "SP-project--main\\speak_two.vbs", NULL, NULL, SW_HIDE);
			break;
		case 6:
			ShellExecuteA(NULL, "open", "SP-project--main\\speak_three.vbs", NULL, NULL, SW_HIDE);
			break;
		case 7:
			ShellExecuteA(NULL, "open", "SP-project--main\\speak_four.vbs", NULL, NULL, SW_HIDE);
			break;
		case 8:
			ShellExecuteA(NULL, "open", "SP-project--main\\speak_five.vbs", NULL, NULL, SW_HIDE);
			break;
		case 9:		
			ShellExecuteA(NULL, "open", "SP-project--main\\speak_six.vbs", NULL, NULL, SW_HIDE);
			break;
		case 10:
			ShellExecuteA(NULL, "open", "SP-project--main\\speak_present.vbs", NULL, NULL, SW_HIDE);
			break;

		
		
	}

	Sleep(5000);
	return result;
}

//Main of liveTesting.cpp

void liveTesting()
{
	computeDCShift();
	readCodeBook();
	int i=0;
	char file[100] = "input.txt";
	int flag = 0; //0 for close, 1 for open, 2 for slideshow
	int count=7;
	while(count--){
		//system("SP-project--main\\speak.vbs");
		ShellExecuteA(NULL, "open", "SP-project--main\\speak.vbs", NULL, NULL, SW_HIDE);
		//ShellExecuteA(NULL, "open", "SP-project--main\\wait1.vbs", NULL, NULL, SW_HIDE);
		Sleep(5000);//wait for 2s
		Beep(523,500);
		//system("Recording_Module\\Recording_Module.exe 3 input.wav input.txt");
		recording(file, COMMAND_DURATION);

		
		i = recognize();
		
		if(i==0 && flag) {//goto
			if(flag!=2 && flag==1)
			{
				//system("SP-project--main\\start_slideshow.vbs");
				ShellExecuteA(NULL, "open", "SP-project--main\\start_slideshow.vbs", NULL, NULL, SW_HIDE);
				flag = 2;
			}
			//printf("\nSpeak page Number\n");
			//system("Recording_Module\\Recording_Module.exe 3 input.wav input.txt");
			ShellExecuteA(NULL, "open", "SP-project--main\\speak.vbs", NULL, NULL, SW_HIDE);
			Sleep(3000);
			Beep(523,500);
			//system("Recording_Module\\Recording_Module.exe 3 input.wav input.txt");
			recording(file, COMMAND_DURATION);
		//	system("SP-project--main\\enter_console.vbs");
		//	ShellExecuteA(NULL, "open", "SP-project--main\\enter_console.vbs", NULL, NULL, SW_HIDE);
			i = recognize();
			if(i==5)
            //  system("SP-project--main\\slideshow_goto2.vbs");
				ShellExecuteA(NULL, "open", "SP-project--main\\slideshow_goto2.vbs", NULL, NULL, SW_HIDE);
            else if(i==6)
             //   system("SP-project--main\\slideshow_goto3.vbs");
				  ShellExecuteA(NULL, "open", "SP-project--main\\slideshow_goto3.vbs", NULL, NULL, SW_HIDE);
            else if(i==7)
            //   system("SP-project--main\\slideshow_goto4.vbs");
				 ShellExecuteA(NULL, "open", "SP-project--main\\slideshow_goto4.vbs", NULL, NULL, SW_HIDE);
            else if(i==8)
             //   system("SP-project--main\\slideshow_goto5.vbs");
				  ShellExecuteA(NULL, "open", "SP-project--main\\slideshow_goto5.vbs", NULL, NULL, SW_HIDE);
            else if(i==9)
                //system("SP-project--main\\slideshow_goto6.vbs");
				ShellExecuteA(NULL, "open", "SP-project--main\\slideshow_goto6.vbs", NULL, NULL, SW_HIDE);
            else {
				//printf("\nThis operation is cancelled\n");
				//system("SP-project--main\\wait.vbs");
				Sleep(5000);//wait for 5s
			}
                
		}
		else if(i==1) {//open
           // system("SP-project--main\\open_ppt.vbs");
			ShellExecuteA(NULL, "open", "SP-project--main\\open_ppt.vbs", NULL, NULL, SW_HIDE);
            flag = 1;
        }
		else if(i==2 && flag!=0) {//next
            if(flag==1)
                //system("SP-project--main\\next_slide.vbs");
				ShellExecuteA(NULL, "open", "SP-project--main\\next_slide.vbs", NULL, NULL, SW_HIDE);
            else
               // system("SP-project--main\\slideshow_next.vbs");
				ShellExecuteA(NULL, "open", "SP-project--main\\slideshow_next.vbs", NULL, NULL, SW_HIDE);
        }
        else if(i==3 && flag!=0) {//back
            if(flag==1)
                //system("SP-project--main\\previous_slide.vbs");
				ShellExecuteA(NULL, "open", "SP-project--main\\previous_slide.vbs", NULL, NULL, SW_HIDE);
            else
               // system("SP-project--main\\slideshow_back.vbs");
				ShellExecuteA(NULL, "open", "SP-project--main\\slideshow_back.vbs", NULL, NULL, SW_HIDE);
        }
        else if(i==4 && flag!=0) {//close
			//printf("\nSpeack close again to close presentation application\n");
			//system("SP-project--main\\speak_close.vbs");
			ShellExecuteA(NULL, "open", "SP-project--main\\speak_close.vbs", NULL, NULL, SW_HIDE);
			Sleep(5000);//wait for 8s
			Beep(523,500);
			//system("Recording_Module\\Recording_Module.exe 3 input.wav input.txt");
			recording(file, COMMAND_DURATION);
			//system("SP-project--main\\wait.vbs");
			
			//system("SP-project--main\\enter_console.vbs");
			//ShellExecuteA(NULL, "open", "SP-project--main\\enter_console.vbs", NULL, NULL, SW_HIDE);
			//ShellExecuteA(NULL, "open", "SP-project--main\\wait.vbs", NULL, NULL, SW_HIDE);
			i = recognize();
			if(i==4) {
				//system("SP-project--main\\close_ppt.vbs");
				ShellExecuteA(NULL, "open", "SP-project--main\\close_ppt.vbs", NULL, NULL, SW_HIDE);
				flag = 0;
			}
			else {
				printf("\nThis operation is cancelled\n");
				//system("SP-project--main\\wait.vbs");
			}

            
        }
        else if(i==10 && flag==1) {//present
            //system("SP-project--main\\start_slideshow.vbs");
			ShellExecuteA(NULL, "open", "SP-project--main\\start_slideshow.vbs", NULL, NULL, SW_HIDE);
                flag=2;
        }
        else if(i==4 && flag==0) {//close
			//printf("\nSpeak close again to close application\n");
			//system("SP-project--main\\speak_close.vbs");
			
			ShellExecuteA(NULL, "open", "SP-project--main\\speak_close.vbs", NULL, NULL, SW_HIDE);
			Sleep(5000);//wait for 8s
			Beep(523,500);
			//system("Recording_Module\\Recording_Module.exe 3 input.wav input.txt");
			recording(file, COMMAND_DURATION);
			//system("SP-project--main\\enter_console.vbs");
		//	ShellExecuteA(NULL, "open", "SP-project--main\\enter_console.vbs", NULL, NULL, SW_HIDE);
			//ShellExecuteA(NULL, "open", "SP-project--main\\wait.vbs", NULL, NULL, SW_HIDE);
			//system("SP-project--main\\wait.vbs");
			Sleep(5000);//wait for 5s
			i = recognize();
			if(i==4) {
				break;
			}
			else {
				//printf("\nThis operation is cancelled\n");
				//system("SP-project--main\\wait.vbs");
				Sleep(5000);//wait for 5s
				//ShellExecuteA(NULL, "open", "SP-project--main\\wait.vbs", NULL, NULL, SW_HIDE);
			}
            
        }

		else {
			if(flag!=0)
				//system("SP-project--main\\wait.vbs");
				Sleep(5000);//wait for 5s
				//ShellExecuteA(NULL, "open", "SP-project--main\\wait.vbs", NULL, NULL, SW_HIDE);
		}
	}
	
	system("pause");
	//return;
}