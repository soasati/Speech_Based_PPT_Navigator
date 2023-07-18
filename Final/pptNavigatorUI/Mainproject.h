using namespace std;

#include "stdafx.h"
#include <iostream>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <direct.h>
#include <cstdio>
#include "CommonVariables.h"
#include "hmm.h"	
#include"ObservationSequenceGenerator.h"
#include "generateUniverse.h"
#include "LBG.h"
#include "ForwardProcedure.h"
#include "liveTesting.h"
#include "controlpanel.h"
#include "liveTraining.h"



void setProfile(const char *input)
{
	sprintf(profile, "%s", input);
}


void setTrainingWord(const char *input) 
{
	sprintf(wordForTraining, "%s", input);
	liveTraining();
}

void callLiveTesting()
{
	try {
		liveTesting();
	}
	catch(exception e) {
		return;
	}
	//liveTesting();
}