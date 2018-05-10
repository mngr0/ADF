/*
* routine.c
*
* Created: 03/05/2018 17:31:43
*  Author: COLLAUDI E SVILUPPO
*/

#include "routine.h"
#include "Formats.h"
#include "task.h"

TaskHandle_t      xRoutineTask0;
TaskHandle_t      xRoutineTask1;

static void controller_routine_encoder(machine_configuration_t *gc){
	uint8_t index=gc->index_gun;
	while(1){
		/*
		wait for pressure, requires interrupt
		*/
		vTaskSuspend(NULL);
		uint32_t time_left = 0;
		uint8_t i=0;
		while (i<len(gc->active)){
			time_left=times(gc->active)[i];
			if (overtime(gc->active)>0){
				open_over(index);
				if (overtime(gc->active) < time_left){
					os_sleep(overtime(gc->active));
					time_left= time_left-overtime(gc->active);
				}
				else{
					os_sleep(time_left);
					time_left = 0;
				}
				close_over(index);
			}
			
			if (time_left > 0){
				open_normal(index);
				if ((undertime(gc->active)<0xFFFFFFFF)&& (time_left > undertime(gc->active))){
					close_normal(index);
					open_under(index);
					os_sleep(time_left- undertime(gc->active));
					close_under(index);
				}
				else{
					os_sleep(time_left);
					close_normal(index);
				}
			}
			i++;
			if (i<len(gc->active)){
				os_sleep(times(gc->active)[i]);
				i++;
			}
		}
	}
}




void tratto_tempo(uint32_t tempo,machine_configuration_t *mc){
	uint8_t index=mc->index_gun;
	uint32_t time_left = 0;
	time_left=tempo;
	if (overtime(mc->active)>0){
		open_over(index);
		if (overtime(mc->active) < time_left){
			//os_sleep(overtime(gc->active));
			TickType_t xLastWakeTime = xTaskGetTickCount ();
			vTaskDelayUntil(&xLastWakeTime,overtime(mc->active));
			time_left= time_left-overtime(mc->active);
		}
		else{
			os_sleep(time_left);
			time_left = 0;
		}
		close_over(index);
	}
	
	if (time_left > 0){
		open_normal(index);
		if ((undertime(mc->active)<0xFFFFFFFF)&& (time_left > undertime(mc->active))){
			os_sleep(undertime(mc->active));
			close_normal(index);
			open_under(index);
			os_sleep(time_left- undertime(mc->active));
			close_under(index);
		}
		
		else{
			os_sleep(time_left);
			close_normal(index);
		}
	}
}

void tratto_dot(uint8_t n_tratto, machine_configuration_t *mc){
	uint32_t tempo=0;
	while(tempo < times(mc->active)[n_tratto]){
		tratto_tempo(dot_len(mc->active),mc);
		tempo+=dot_len(mc->active);
		os_sleep(dot_pause(mc->active));
		tempo+=dot_pause(mc->active);
	}
}



static void controller_routine(machine_configuration_t *gc)
{
	
	//WAIT UNTIL!!!!!!!!!!!!!!
	
	//uint8_t conf_set=gc->active;//  ((uint8_t)x) & 0x07;
	uint8_t index=gc->index_gun;// ((uint8_t)x>>4) & 0x01;
	while(1){
		/*
		wait for pressure, requires interrupt
		*/
		
		vTaskSuspend(NULL);
		uint32_t time_left = 0;
		uint8_t i=0;
		while (i<len(gc->active)){
			/*
			time_left=times(gc->active)[i];
			if (overtime(gc->active)>0){
			open_over(index);
			if (overtime(gc->active) < time_left){
			//os_sleep(overtime(gc->active));
			TickType_t xLastWakeTime = xTaskGetTickCount ();
			vTaskDelayUntil(&xLastWakeTime,overtime(gc->active));
			time_left= time_left-overtime(gc->active);
			}
			else{
			os_sleep(time_left);
			time_left = 0;
			}
			close_over(index);
			}
			
			if (time_left > 0){
			open_normal(index);
			if ((undertime(gc->active)<0xFFFFFFFF)&& (time_left > undertime(gc->active))){
			os_sleep(undertime(gc->active));
			close_normal(index);
			open_under(index);
			os_sleep(time_left- undertime(gc->active));
			close_under(index);
			}
			
			else{
			os_sleep(time_left);
			close_normal(index);
			}
			}*/
			
			tratto_tempo(times(gc->active)[i],gc);
			
			i++;
			if (i<len(gc->active)){
				os_sleep(times(gc->active)[i]);
				i++;
			}
		}
	}
}


void start_routine(){
	if (xTaskCreate(controller_routine, "C0", TASK_EXAMPLE_STACK_SIZE,(void*)&machine_confs[0],
	TASK_EXAMPLE_STACK_PRIORITY, &xRoutineTask0)!= pdPASS) {
		while (1) {
			;
		}
	}

	if (xTaskCreate(controller_routine, "C1", TASK_EXAMPLE_STACK_SIZE,(void*)&machine_confs[1],
	TASK_EXAMPLE_STACK_PRIORITY, &xRoutineTask1)!= pdPASS) {
		while (1) {
			;
		}
	}
}