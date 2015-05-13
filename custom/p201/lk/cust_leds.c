//#include <platform/cust_leds.h>
#include <cust_leds.h>
#include <platform/mt_pwm.h>

extern int DISP_SetBacklight(int level);

extern UINT32 DISP_GetScreenWidth(void);

extern int disp_bls_set_backlight(unsigned int level);

// Only support 64 levels of backlight (when lcd-backlight = MT65XX_LED_MODE_PWM)
#define BACKLIGHT_LEVEL_PWM_64_FIFO_MODE_SUPPORT 64 
// Support 256 levels of backlight (when lcd-backlight = MT65XX_LED_MODE_PWM)
#define BACKLIGHT_LEVEL_PWM_256_SUPPORT 256 

// Custom can decide the support type "BACKLIGHT_LEVEL_PWM_256_SUPPORT" or "BACKLIGHT_LEVEL_PWM_64_FIFO_MODE_SUPPORT"
#define BACKLIGHT_LEVEL_PWM_MODE_CONFIG BACKLIGHT_LEVEL_PWM_256_SUPPORT

unsigned int Cust_GetBacklightLevelSupport_byPWM(void)
{
	return BACKLIGHT_LEVEL_PWM_MODE_CONFIG;
}

unsigned int Cust_SetBacklight(int level)
{
	int ret;
	unsigned int  w;

	w = DISP_GetScreenWidth();
#if defined(BUILD_LK)
	printf("Cust_SetBacklight Screen Width : %d\n", w);
#else
	printk("Cust_SetBacklight Screen Width: %d\n", w);
#endif
	
	if(w == 320)
		ret = DISP_SetBacklight(level);
	else if(w == 720)
		ret = disp_bls_set_backlight(level);

	return ret;
}

static struct cust_mt65xx_led cust_led_list[MT65XX_LED_TYPE_TOTAL] = {
 	{"red",               MT65XX_LED_MODE_PMIC, MT65XX_LED_PMIC_NLED_ISINK0,{0}},
        {"green",             MT65XX_LED_MODE_PMIC, MT65XX_LED_PMIC_NLED_ISINK1,{0}},
        {"blue",              MT65XX_LED_MODE_PMIC, MT65XX_LED_PMIC_NLED_ISINK2,{0}},
        {"jogball-backlight", MT65XX_LED_MODE_NONE, -1,{0}},
        {"keyboard-backlight",MT65XX_LED_MODE_NONE, -1,{0}},
        {"button-backlight",  MT65XX_LED_MODE_PMIC, MT65XX_LED_PMIC_NLED_ISINK3,{0}},
        //{"lcd-backlight",     MT65XX_LED_MODE_CUST_BLS_PWM, (int)disp_bls_set_backlight,{0}},
        {"lcd-backlight",     MT65XX_LED_MODE_CUST_BLS_PWM, (int)Cust_SetBacklight,{0}},
};

struct cust_mt65xx_led *get_cust_led_list(void)
{
	return cust_led_list;
}

