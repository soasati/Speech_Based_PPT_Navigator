void run_application()
{
	int LiveTestIndex; //this will be the index from livetesting
	int slideShowflag = 0;
	int digit;
	switch(choice)
	{
		case 1:	//ex.open the application
				system("open_ppt.vbs");
				break;
				
		case 2:	//ex. run the slideshow
				system("start_slideshow.vbs");
				slideShowflag = 1;
				break;
		case 3:	//ex. next
				if(slideShowflag == 0)
				{
					system("next_slide.vbs");
				}
				else
				{
					system("slideshow_next.vbs");
				}
				break;
		case 4://ex. back
				if(slideShowflag == 0)
				{
					system("previous_slide.vbs");
				}
				else
				{
					system("slideshow_back.vbs");
				break;
		case 5://ex. close ppt and exit the application
				system("close_ppt.vbs");
				break;
		case 6://ex. goto
				printf("\nEnter the slide number from 1 to 7");
				//reading the slide number into variable digit
				switch(digit){
					case 1:system("slideshow_goto.vbs");
							break;
					case 2:system("slideshow_goto2.vbs");
							break;
					case 3:system("slideshow_goto3.vbs");
							break;
					case 4:system("slideshow_goto4.vbs");
							break;
					case 5:system("slideshow_goto5.vbs");
							break;
					case 6:system("slideshow_goto6.vbs");
							break;
					case 7:system("slideshow_goto7.vbs");
							break;
					default:printf("\nDigit not valid");
							break;
				}
				break;
		case 7://ex. exit to main menu
				return ;
				break;
		default:
				break;
	}
}