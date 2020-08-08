#include <stdint.h>

#include "os_task.h"
#include "os_msg_q.h"

#include "co_printf.h"
#include "user_task.h"
#include "button.h"
#include "os_timer.h"
#include "driver_pmu.h"
#include "stdio.h"
#include "simple_gatt_service.h"

uint16_t user_task_id;
uint8_t data[100] = {
						0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
						0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
						0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
						0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
						0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
						0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
						0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
						0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
						0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
						0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		};


static int user_task_func(os_event_t *param)
{
    switch(param->event_id)
    {
        case USER_EVT_BUTTON:
            {
                struct button_msg_t *button_msg;
                const char *button_type_str[] = {
                                                    "BUTTON_PRESSED",
                                                    "BUTTON_RELEASED",
                                                    "BUTTON_SHORT_PRESSED",
                                                    "BUTTON_MULTI_PRESSED",
                                                    "BUTTON_LONG_PRESSED",
                                                    "BUTTON_LONG_PRESSING",
                                                    "BUTTON_LONG_RELEASED",
                                                    "BUTTON_LONG_LONG_PRESSED",
                                                    "BUTTON_LONG_LONG_RELEASED",
                                                    "BUTTON_COMB_PRESSED",
                                                    "BUTTON_COMB_RELEASED",
                                                    "BUTTON_COMB_SHORT_PRESSED",
                                                    "BUTTON_COMB_LONG_PRESSED",
                                                    "BUTTON_COMB_LONG_PRESSING",
                                                    "BUTTON_COMB_LONG_RELEASED",
                                                    "BUTTON_COMB_LONG_LONG_PRESSED",
                                                    "BUTTON_COMB_LONG_LONG_RELEASED",
                                                };

                button_msg = (struct button_msg_t *)param->param;
                
                co_printf("KEY 0x%08x, TYPE %s.\r\n", button_msg->button_index, button_type_str[button_msg->button_type]);
            }
            break;
    }

    return EVT_CONSUMED;
}


static void test_time_fn(void *param) 
{
	//co_printf("1s timer\r\n");
	static int i = 0;
	static unsigned int toggle_value = 0;
	//LED2 toggle
	if(i ++ == 50)
	{
			i = 0;
			toggle_value = ~toggle_value;
			//co_printf("toggle_value = %d\r\n", toggle_value);
			//pmu_set_led2_as_pwm();
			//pmu_set_led2_value(toggle_value);
	}
	//发送数据
	//simple_gatt_notify_send(data, 100);
}   

os_timer_t test_timer; 
void user_task_init(void)
{
		os_timer_init(&test_timer,test_time_fn, NULL); 
    os_timer_start(&test_timer, 10, true);    //启动一个 1s 的定时器。 
    user_task_id = os_task_create(user_task_func);
}