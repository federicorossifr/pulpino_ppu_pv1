/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#define DELAY_LOOP 1000

void task1 (void *pvParameters) {
	
	//printf("task2");
	
	for(;;);
		
	vTaskDelete(NULL);
}
/*
void task2 (void *pvParameters) {
	
	printf("task2");
	for(int i = 0; i < DELAY_LOOP; i++)
		portNOP();

	vTaskDelete(NULL);
}*/


int main( void )
{
	
	xTaskCreate(task1, "Task 1", 90, NULL, 1, NULL);
 	// xTaskCreate(task2, "Task 2", 100, NULL, 1, NULL);
	vTaskStartScheduler();

	//printf("Hello World!\n");

	for(;;);
	return 0;
}