
#include <linux/module.h>	
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/timer.h>
#include <linux/init.h>
#include <linux/gpio.h>

static int 			 levels[5] 			= { 0, 0};
static int 			 outputs[5] 		= { 5, 6};
static int 			 togglespeeds[5] 	= { 0, 0};
static int 			 count_levels 		= 0;
static int 			 count_outputs 		= 0;
static int 			 count_togglespeeds = 0;


module_param_array(levels, int, &count_levels, 0000);
MODULE_PARM_DESC(levels, "An array of Levels");
module_param_array(outputs, int, &count_outputs, 0000);
MODULE_PARM_DESC(outputs, "An array of Outputs");
module_param_array(togglespeeds, int, &count_togglespeeds, 0000);
MODULE_PARM_DESC(togglespeeds, "An array of ToggleSpeeds");

static struct timer_list blink_timer0, blink_timer1;
static long data=0;
static struct gpio leds[] = {
		{ 5, GPIOF_OUT_INIT_HIGH, "LED 1" },
		{ 6, GPIOF_OUT_INIT_HIGH, "LED 2" }
				};
static void blink_timer_func0(struct timer_list* t)
{
	printk(KERN_INFO "%s\n", __func__);

	gpio_set_value(leds[0].gpio, levels[0]);
	levels[0]=!levels[0]; 
	
	/* schedule next execution */
	//blink_timer.data = !data;						// makes the LED toggle 
	blink_timer0.expires = jiffies + (togglespeeds[0]*HZ); 		// 1 sec.
	add_timer(&blink_timer0);
}
static void blink_timer_func1(struct timer_list* t)
{
	printk(KERN_INFO "%s\n", __func__);

	gpio_set_value(leds[1].gpio, levels[1]);
	levels[1]=!levels[1]; 
	
	/* schedule next execution */
	//blink_timer.data = !data;						// makes the LED toggle 
	blink_timer1.expires = jiffies + (togglespeeds[1]*HZ); 		// 1 sec.
	add_timer(&blink_timer1);
}

static int __init gpiomod_init(void)
{
	int i;
	for (i = 0; i < (sizeof levels / sizeof (int)); i++)
	{
		leds[i].gpio = outputs[i];
		if(levels[i]){
			leds[i].flags=GPIOF_OUT_INIT_HIGH;
		}
		else{
			leds[i].flags=GPIOF_OUT_INIT_LOW;
		}
		if(togglespeeds[i]!=0){

		}
	}

	int ret = 0;

	printk(KERN_INFO "%s\n", __func__);
	// register LED GPIOs, turn LEDs on


	ret = gpio_request_array(leds, ARRAY_SIZE(leds));
	if(togglespeeds[0]!=0){
		timer_setup(&blink_timer0, blink_timer_func0, 0);
		blink_timer0.function = blink_timer_func0;
		blink_timer0.expires = jiffies + (1*HZ); 		// 1 sec.
		add_timer(&blink_timer0);
	}
	if(togglespeeds[1]!=0){
		timer_setup(&blink_timer1, blink_timer_func1, 0);
		blink_timer1.function = blink_timer_func1;
		blink_timer1.expires = jiffies + (1*HZ); 		// 1 sec.
		add_timer(&blink_timer1);
	}
		
	if (ret) {
		printk(KERN_ERR "Unable to request GPIOs: %d\n", ret);
	}

	return ret;
}

/*
 * Module exit function
 */
static void __exit gpiomod_exit(void)
{
	int i;

	printk(KERN_INFO "%s\n", __func__);

	// turn all LEDs off
	for(i = 0; i < ARRAY_SIZE(leds); i++) {
		gpio_set_value(leds[i].gpio, 0); 
	}
	del_timer_sync(&blink_timer0);
	del_timer_sync(&blink_timer1);
	// unregister all GPIOs
	gpio_free_array(leds, ARRAY_SIZE(leds));
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mourad Essebane");
MODULE_DESCRIPTION("Opdracht 6: DEEL2");

module_init(gpiomod_init);
module_exit(gpiomod_exit);
