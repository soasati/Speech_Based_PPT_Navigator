// SPProject.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string.h>
#include "hmm.h"	
#include"ObservationSequenceGenerator.h"
#include "generateUniverse.h"
#include "LBG.h"
#include "ForwardProcedure.h"
#include "liveTesting.h"
#include "controlpanel.h"
#include "liveTraining.h"

int _tmain(int argc, _TCHAR* argv[])
{
	//required for offline training
	//commented as of now as profiles are already created
	int profilename;
	printf("\nEnter your profile name - Datam\nDatav\nDatas\n");
	scanf("%s",&profile);
	printf("\nProfile selected is %s ",profile);
	//controlPanel();
	hmm();
	//liveTesting();
	//liveTraining();*/
	/*int choice;
	char ch[2];
	//int profilename;

	//Main menu
	printf("\nEnter your profile name - Datam\nDatav\nDatas\n");
	scanf("%s",&profile);
	printf("\nProfile selected is %s ",profile);
	do{
		printf("\n**************Menu**********************");
		printf("\n1.Live Training\n2.Live Testing\n");
		scanf("%d",&choice);
		switch(choice)
		{
		case 1:	//Live Training
				printf("\n**************Live Training******************\n");
				printf("\nPlease refer to the below words for vocabulary\n");
				for(int i=0;i<MAX_WORDS;i++)
					printf("%s\n",words[i]);
				
				liveTraining();
				break;
		case 2://Live Testing
				printf("\n**************Live Testing******************");
				liveTesting();
				break;
		
		default:printf("\nWrong choice");
				break;
		}
		printf("\nDo you want to continue(y/n)?\n");
		scanf("%s",&ch);
	}while(strcmp(ch,"y")==0); */
	return 0;
}

