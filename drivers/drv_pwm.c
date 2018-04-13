#include <rtthread.h>
#include <rthw.h>
#include <rtdevice.h>

#include "drv_pwm.h"

#include "stm32f4xx_hal.h"

/*user should change this to adapt specific board*/
#define pwm1_PIN                 GPIO_PIN_5
#define pwm1_PORT                GPIOE
#define pwm1__PORT_CLK_ENABLE()   __HAL_RCC_GPIOE_CLK_ENABLE()

#define pwm2_PIN                 GPIO_PIN_6
#define pwm2_PORT                GPIOE
#define pwm2__PORT_CLK_ENABLE()   __HAL_RCC_GPIOE_CLK_ENABLE()

#define radar_pwm_PIN                 GPIO_PIN_15
#define radar_pwm_PORT                GPIOA
#define radar_pwm__PORT_CLK_ENABLE()   __HAL_RCC_GPIOA_CLK_ENABLE()


TIM_HandleTypeDef TIMx_Handler;         //��ʱ��3PWM��� 
TIM_OC_InitTypeDef TIMx_CHxHandler;	    //��ʱ��3ͨ��4���
//TIM9 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM_2_9_PWM_Init(rt_uint16_t arr,rt_uint16_t psc)
{ 
    TIMx_Handler.Instance=TIM9;            //��ʱ��3
    TIMx_Handler.Init.Prescaler=psc;       //��ʱ����Ƶ
    TIMx_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//���ϼ���ģʽ
    TIMx_Handler.Init.Period=arr;          //�Զ���װ��ֵ
    TIMx_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&TIMx_Handler);       //��ʼ��PWM    
    TIMx_CHxHandler.OCMode=TIM_OCMODE_PWM1; //ģʽѡ��PWM1
    TIMx_CHxHandler.Pulse=arr/2;            //���ñȽ�ֵ,��ֵ����ȷ��ռ�ձȣ�Ĭ�ϱȽ�ֵΪ�Զ���װ��ֵ��һ��,��ռ�ձ�Ϊ50%
    TIMx_CHxHandler.OCPolarity=TIM_OCPOLARITY_LOW; //����Ƚϼ���Ϊ�� 
    HAL_TIM_PWM_ConfigChannel(&TIMx_Handler,&TIMx_CHxHandler,TIM_CHANNEL_1);//����TIM3ͨ��4
		HAL_TIM_PWM_ConfigChannel(&TIMx_Handler,&TIMx_CHxHandler,TIM_CHANNEL_2);//����TIM3ͨ��4
		HAL_TIM_PWM_Start(&TIMx_Handler,TIM_CHANNEL_1);//����PWMͨ��4
		HAL_TIM_PWM_Start(&TIMx_Handler,TIM_CHANNEL_2);//����PWMͨ��4
		
    TIMx_Handler.Instance=TIM2;            //��ʱ��3
    HAL_TIM_PWM_Init(&TIMx_Handler);       //��ʼ��PWM
    HAL_TIM_PWM_ConfigChannel(&TIMx_Handler,&TIMx_CHxHandler,TIM_CHANNEL_1);//����TIM3ͨ��4
    HAL_TIM_PWM_Start(&TIMx_Handler,TIM_CHANNEL_1);//����PWMͨ��4	
}


//��ʱ���ײ�������ʱ��ʹ�ܣ���������
//�˺����ᱻHAL_TIM_PWM_Init()����
//htim:��ʱ�����
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef GPIO_Initure;
		__HAL_RCC_TIM9_CLK_ENABLE();			//ʹ�ܶ�ʱ��9
    pwm1__PORT_CLK_ENABLE();			//����GPIOEʱ��
	  pwm2__PORT_CLK_ENABLE();			//����GPIOEʱ��

	
    GPIO_Initure.Pin=pwm1_PIN|pwm2_PIN;           	//PB1
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//�����������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
		GPIO_Initure.Alternate= GPIO_AF3_TIM9;	//����ΪTIM9_CH1`2
    HAL_GPIO_Init(GPIOE,&GPIO_Initure);
	
		__HAL_RCC_TIM2_CLK_ENABLE();			//ʹ�ܶ�ʱ��2
    radar_pwm__PORT_CLK_ENABLE();			//����GPIOAʱ��
		GPIO_Initure.Pin=radar_pwm_PIN;           	//PB1
		GPIO_Initure.Pull=GPIO_PULLUP;          //����
		GPIO_Initure.Alternate= GPIO_AF1_TIM2;	//����ΪTIM9_CH1`2
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
}

//����TIM9ͨ��1 2��ռ�ձ�
//compare:�Ƚ�ֵ
void TIM_SetTIM2Compare(rt_uint32_t compare)
{
	TIM2->CCR1=compare; 
}


//����TIM9ͨ��1 2��ռ�ձ�
//compare:�Ƚ�ֵ
void TIM_SetTIM9Compare(rt_uint32_t compare1,rt_uint32_t compare2)
{
	TIM9->CCR1=compare1; 
	TIM9->CCR2=compare2;
}


	

int hw_PWM_init(void)
{
	TIM_2_9_PWM_Init(2000-1,9-1);      //180M/9=20M�ļ���Ƶ�ʣ��Զ���װ��Ϊ2000����ôPWMƵ��Ϊ20M/2000=10kHZ

  TIM_SetTIM9Compare(300,300);
	TIM_SetTIM2Compare(300);
	return RT_EOK;
}
INIT_BOARD_EXPORT(hw_PWM_init);
/* end of i2c driver */

