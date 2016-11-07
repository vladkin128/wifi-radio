#include <stdio.h>   /* Standard input/output definitions */
#include <stdlib.h>  /* Standard General Utilities Library */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <signal.h> 
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <sys/poll.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "mpc.h"
#include "display.h"
#include "encoder.h"
#include "term.h"

#define CLRSCR() fputs("\033[H\033[J", stdout);
//#define home() 			fputs(ESC "[H", stdout) //Move cursor to the indicated row, column (origin at 1,1)
#define BUFF_LEN 256


void show_current_track() {
		/*
		get_title (char * buf, int len)//название для всех
		get_name (char * buf, int len) //Это для радиостанций (название радио)
		get_artist (char * buf, int len)  //Это для артистов
		*/
		
		//У дисплея есть режим Specify horizontal scroll mode - разобраться.

		char cmp_buf_title[BUFF_LEN]; 
		static char curent_buf_title[BUFF_LEN];
		static int curent_title_position;
		static int strlen_title;
		
		
		char cmp_buf_name[BUFF_LEN];
		static char curent_buf_name[BUFF_LEN];
		static int curent_name_position;
		static int strlen_name;
		
		char show [21];
		show [20]='\0';
		
		get_title (cmp_buf_title, BUFF_LEN);
		if (strcmp(cmp_buf_title,curent_buf_title) != 0) {
			print_title(cmp_buf_title); //terminal
			strcpy(curent_buf_title, cmp_buf_title);
			curent_title_position=0;
			strlen_title=strlen(curent_buf_title);
		}

		get_name (cmp_buf_name, BUFF_LEN);
		print_name(cmp_buf_name); //terminal
		if (strlen(cmp_buf_name)==0) {
			get_artist(cmp_buf_name, BUFF_LEN);
			print_artist(cmp_buf_name); //terminal
		}

		if (strcmp(cmp_buf_name,curent_buf_name) != 0) {
			strcpy(curent_buf_name, cmp_buf_name);
			curent_name_position=0;
			strlen_name=strlen(curent_buf_name);
		}

	//Show title
		memset (show,' ',20);
		if (((curent_title_position==0) && (strlen_title<=20))) {
//Delete line
			home_scr();
			print_to_scr (show);
//
			strncpy (show, curent_buf_title, 20);
			curent_title_position++;
			home_scr();
			print_to_scr (show);
		}
		if (strlen_title>20) {
			if ((strlen_title-curent_title_position)>=20) {
				strncpy (show, &curent_buf_title[curent_title_position], 20);
			} else strncpy (show, &curent_buf_title[curent_title_position], (strlen_title-curent_title_position));
			curent_title_position++;
			if (curent_title_position==strlen_title)
				curent_title_position=0;
			home_scr();
			print_to_scr (show);
		}
		
	//Show name
		memset (show,' ',20);
		if (((curent_name_position==0) && (strlen_name<=20)) ) {
//Delete line
			set_to_position_scr(1, 2);
			print_to_scr (show);
//
			strncpy (show, curent_buf_name, 20);
			curent_name_position++;
			set_to_position_scr(1, 2);
			print_to_scr (show);
		}
		if (strlen_name>20) {
			if ((strlen_name-curent_name_position)>=20) {
				strncpy (show, &curent_buf_name[curent_name_position], 20);
			} else strncpy (show, &curent_buf_name[curent_name_position], (strlen_name-curent_name_position));
			curent_name_position++;
			if (curent_name_position==strlen_name)
				curent_name_position=0;
			set_to_position_scr(1, 2);
			print_to_scr (show);
		}
		usleep (500000);
		//sleep(1);
	
}

void tunning () {
	char chout='\n';
	int action=1;
	time_t last_time_action, current_time;
	init_encoder_comport();
	init_term();
	while (1) {
		chout=encoder_read();
		if ((chout=='R') || (chout=='L') ||(chout=='a') || (chout=='d')) {
				action=0;
				last_time_action = time(NULL);
				get_cur_position (); //display.c
				tuning_movement(chout); //display.c
				show_current_cursor_pos (); 
		}
		if ((chout=='P') ||(chout=='s') ) {
				//CLRSCR();
				print_button_pressed();
				MUSIC_PAUSE();
		}
		if (action==0) {
			current_time = time(NULL); 
			if ((current_time-last_time_action)>3) {
				action++;
			}
		}
		if (action) {
			show_current_track();
			
			print_display_table (); //terminal
		}
	}

}




