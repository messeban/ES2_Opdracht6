
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>

static int 			 levels[5] 	= { 0, 0, 0, 0, 0};
static int 			 outputs[5] 	= { 5, 6, 13, 19, 26};
static int 			 count_levels 		= 0;
static int 			 count_outputs 		= 0;


module_param_array(levels, int, &count_levels, 0000);
MODULE_PARM_DESC(levels, "An array of Levels");
module_param_array(outputs, int, &count_outputs, 0000);
MODULE_PARM_DESC(outputs, "An array of Levels");


static struct gpio leds[] = {
		{ 5, GPIOF_OUT_INIT_HIGH, "LED 1" },
		{ 6, GPIOF_OUT_INIT_HIGH, "LED 2" },
		{ 13, GPIOF_OUT_INIT_HIGH, "LED 3" },
		{ 19, GPIOF_OUT_INIT_HIGH, "LED 4" },
		{ 26, GPIOF_OUT_INIT_HIGH, "LED 5" }
		};

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
	}

	int ret = 0;

	printk(KERN_INFO "%s\n", __func__);

	// register LED GPIOs, turn LEDs on
	ret = gpio_request_array(leds, ARRAY_SIZE(leds));

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
	
	// unregister all GPIOs
	gpio_free_array(leds, ARRAY_SIZE(leds));
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mourad Essebane");
MODULE_DESCRIPTION("Opdracht 6: DEEL2");

module_init(gpiomod_init);
module_exit(gpiomod_exit);
