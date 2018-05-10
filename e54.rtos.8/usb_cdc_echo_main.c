
#include <atmel_start.h>
#include <stdlib.h>
#include "rtos_start.h"

#include "SPIManager.h"
#include "Formats.h"
#include "routine.h"
#include "SDmanager.h"

format_t formats[SAVED_CONF];
machine_configuration_t machine_confs[CHANNEL_NUMBER];

static void button_on_PA16_pressed(void)
{
	gpio_toggle_pin_level(LED0);
	//xTaskResumeFromISR(xRoutineTask0);
	xTaskResumeFromISR(xRoutineTask1);
	portYIELD_FROM_ISR( pdTRUE);
}


int main(void)
{
	atmel_start_init();
	//cdcd_acm_example();
	start_sd();
	
	close_under(0);
	close_under(1);
	
	machine_confs[0].index_gun=0;
	machine_confs[0].active=3;
	machine_confs[1].index_gun=1;
	machine_confs[1].active=4;
	
	ext_irq_register(PIN_PA16, button_on_PA16_pressed);
	NVIC_SetPriority(EIC_0_IRQn, 5 );
	SPI_0_example();

	start_routine();
	vTaskStartScheduler();
	
	while (1) {
	}
}

