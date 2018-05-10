/*
* SDManager.c
*
* Created: 03/05/2018 17:17:07
*  Author: COLLAUDI E SVILUPPO
*/
#include "SDManager.h"
#include "Formats.h"
#include <atmel_start.h>
#include <ff.h>

FATFS FatFs;		/* FatFs work area needed for each volume */
FIL Fil;			/* File object needed for each open file */


void preparesd(){
	uint8_t k;
	for (k=0;k<8;k++){
		formats[k].name[0]='c';
		formats[k].name[1]=48+k;
		formats[k].overtime=50;
		formats[k].undertime=3000;
		formats[k].times[0]=(k+1)*1000;
		formats[k].times[1]=1000;
		formats[k].times[2]=1000;
		formats[k].times[3]=1000;
		formats[k].times[4]=1000;
		formats[k].times[5]=1000;
		formats[k].len=5;
	}
	UINT bw;
	uint32_t buff[ sizeof(format_t)/4];
	f_mount(&FatFs, "", 0);
	if(f_open(&Fil, "config.txt",  FA_WRITE) != FR_OK){
		if(f_open(&Fil, "config.txt",  FA_CREATE_NEW) != FR_OK){
			while(1);
		}
	}
	
	buff[0]=0;
	f_write(&Fil,buff, 1, &bw);
	int i,j;
	for(i=0;i<8;i++){
		
		uint32_t* copier = &formats[i];
		for (j=0;j<sizeof(format_t)/4;j++){
			buff[j]=copier[j];
		}
		f_write(&Fil,buff, sizeof(format_t)/4, &bw);
	}
	f_close(&Fil);
}

void load_times(){
	//carefully, this shit accept conf only with size multiple of 4B
	UINT bw;
	uint32_t buff[ sizeof(format_t)/4];
	f_mount(&FatFs, "", 0);
	
	if (f_open(&Fil, "config.txt", FA_READ | FA_OPEN_EXISTING) == FR_OK) {
		f_read(&Fil,buff, 1, &bw);
		//active0=buff[0];
		int i,j;
		for(i=0;i<8;i++){
			f_read(&Fil,buff, sizeof(format_t)/4, &bw);
			uint32_t* copier = &formats[i];
			for (j=0;j<sizeof(format_t)/4;j++){
				copier[j]=buff[j];
			}
		}
		f_close(&Fil);
	}
	else{
		while(1);
	}
}


static void timed_datalog (){
	while(1){
		//send log message
		//counting items
		//wait until
	}
	
}


static void datalog_routine (){
	UINT bw;
	
	//open fs
	f_mount(&FatFs, "", 0);
	//open file
	f_mount(&FatFs, "", 0);
	if(f_open(&Fil, "config.txt",  FA_WRITE) != FR_OK){
		if(f_open(&Fil, "config.txt",  FA_CREATE_NEW) != FR_OK){
			//TODO find better error handling
			while(1);
			
		}
	}
	while(1){
		//receive message
		//change conf
		//write in file
		f_write(&Fil, "It works!\r\n", 11, &bw);
		//close file?
	}
	
	
}

void get_total_dl_size(){}

void get_dl_from_to(uint32_t from, uint32_t to, dl_item_t* buffer){}



void start_sd(){
	load_times();
	preparesd();
}
