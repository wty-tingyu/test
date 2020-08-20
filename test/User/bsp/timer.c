#include "timer.h"
#include "key.h"
u16 backspeed = 0;

/*   ������ģʽ   */
void Encoder_Init(u16 arr, u16 psc)  //TIM4��ch1 PB6 ��ch2 PB7
	{
    /* ��ʼ���ṹ�嶨�� */
    GPIO_InitTypeDef GPIO_InitStructure;            //GPIO��ʼ���ṹ��
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; //TIMʱ�������ʼ���ṹ��
    TIM_ICInitTypeDef  TIM_ICInitStructure;         //TIM����ȽϹ��ܽṹ��
    NVIC_InitTypeDef NVIC_InitStructure;            //NVIC��ʼ���ṹ��
    
    /* ʱ����(RCC)���� */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);// ʹ�ܶ�ʱ��ʱ��  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);// ʹ��GPIOB����ʱ��
    
    /* �˿�(GPIO)���� */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;    //PB6��PB7
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //��������   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       //�ٶ�50MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);                  //��ʼ��GPIOB
    
    /* TIMʱ�������ʼ�� */
    TIM_TimeBaseStructure.TIM_Period = arr;     //����������arr�������
    TIM_TimeBaseStructure.TIM_Prescaler =psc;   //Ԥ��Ƶϵ��
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;//����ʱ�ӷָ�
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//TIM���¼���ģʽ
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //��ʼ��TIMx��ʱ�ӻ�����λ
    
    /* TIM������ģʽ���� */
    TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12,TIM_ICPolarity_BothEdge, TIM_ICPolarity_BothEdge);    //ʹ�ñ�����ģʽ3
//    //ʹ��ȱʡֵ���ٳ�ʼ��
//    TIM_ICStructInit(&TIM_ICInitStructure);     //��TIM_ICInitStruct�е�ÿһ��������ȱʡֵ����
//    TIM_ICInitStructure.TIM_ICFilter = 6;      //�����˲�������
//    TIM_ICInit(TIM4, &TIM_ICInitStructure);     //����TIM_ICInitStruct�Ĳ�����ʼ������TIMx
    
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1; //ʹ��TIMͨ��1
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;//�����ز���
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;//ӳ�䵽TI1��
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;//���������Ƶ������Ƶ
    TIM_ICInitStructure.TIM_ICFilter = 0x01;    //���������˲��������˲�
    TIM_ICInit(TIM4, &TIM_ICInitStructure);     //��ʼ��TIM���벶��
    
    /* �ж�����������(NVIC)���� */
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;         //�ж���
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;      //�����ȼ�1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);                         //��ʼ��NVIC
    
    /* ʹ��TIM */
    TIM_ClearFlag(TIM4, TIM_FLAG_Update);       //���TIMx�Ĵ������־λ
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);  //ʹ��ָ����TIM�ж�
    //TIM_SetCounter(TIM4,0);                     //����TIMx�������Ĵ���ֵ
		TIM3->CNT = 0; //����������
    TIM_Cmd(TIM4, ENABLE);                      //ʹ��TIM4 
}
	

/*           PWM                */
void PWM_init(uint16_t arr,uint16_t psc)  //TIM3��ch2��Ӧ����PA7 ӳ�䵽 PB5
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //TIM3ʱ��
	//����ʱ��ʹ�� ��GPIOBʱ��ʹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
		
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);//����TIM3������ӳ��
  //��ʼ��TIM3
	TIM_TimeBaseStructure.TIM_Period = arr;     //�Զ�װ��ֵ
	TIM_TimeBaseStructure.TIM_Prescaler  = psc; //Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision  = TIM_CKD_DIV1;//����ʱ�ӷָ�
	TIM_TimeBaseStructure.TIM_CounterMode  = TIM_CounterMode_Down;//���¼���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	//����TIM3��ch2ͨ����PWMģʽ��ʹ��CH2���
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;  //ѡ��PWMģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//������Ը�

	TIM_OC2Init(TIM3, &TIM_OCInitStructure); 
 

	TIM_Cmd(TIM3, ENABLE );
}


/*   ��ʱ10ms ȥ������ģʽ��������ֵ */
void TIM6_Int_Init(u16 arr,u16 psc)
{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	  NVIC_InitTypeDef NVIC_InitStructure;
	
	
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); //ʹ��TIM6ʱ��

		/*��������*/
		TIM_TimeBaseStructure.TIM_Period = arr;//����ֵ10000   
		TIM_TimeBaseStructure.TIM_Prescaler = psc;    //Ԥ��Ƶ,��ֵ+1Ϊ��Ƶ�ĳ���
		TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;  //������Ƶ
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���
		TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

		TIM_ITConfig(TIM6,TIM_IT_Update, ENABLE);     //ʹ��TIM6�ж�
	
  	NVIC_InitStructure.NVIC_IRQChannel =TIM6_IRQn;//TIM6�ж�
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//ռ��ʽ���ȼ�����Ϊ1
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //�����ȼ�����Ϊ0
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//�ж�ʹ��
		NVIC_Init(&NVIC_InitStructure);//�жϳ�ʼ
	
		TIM_Cmd(TIM6, ENABLE);     //ʹ�ܶ�ʱ��6
}
/* ��ȡ��������������ֵ��������������������Խ��еĺ����е���������ٶ���Ϣ */
int Read_Encoder(void)
	{
    int Encoder_TIM;
    Encoder_TIM = (short)TIM4 -> CNT;  TIM4 -> CNT=0;
    return Encoder_TIM;
	}



