#include "timer.h"
#include "key.h"
u16 backspeed = 0;

/*   编码器模式   */
void Encoder_Init(u16 arr, u16 psc)  //TIM4；ch1 PB6 ：ch2 PB7
	{
    /* 初始化结构体定义 */
    GPIO_InitTypeDef GPIO_InitStructure;            //GPIO初始化结构体
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; //TIM时间基数初始化结构体
    TIM_ICInitTypeDef  TIM_ICInitStructure;         //TIM输入比较功能结构体
    NVIC_InitTypeDef NVIC_InitStructure;            //NVIC初始化结构体
    
    /* 时钟线(RCC)设置 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);// 使能定时器时钟  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);// 使能GPIOB外设时钟
    
    /* 端口(GPIO)设置 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;    //PB6、PB7
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //浮空输入   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       //速度50MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);                  //初始化GPIOB
    
    /* TIM时间基数初始化 */
    TIM_TimeBaseStructure.TIM_Period = arr;     //计数器计数arr次则溢出
    TIM_TimeBaseStructure.TIM_Prescaler =psc;   //预分频系数
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;//设置时钟分割
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//TIM向下计数模式
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //初始化TIMx的时钟基数单位
    
    /* TIM编码器模式设置 */
    TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12,TIM_ICPolarity_BothEdge, TIM_ICPolarity_BothEdge);    //使用编码器模式3
//    //使用缺省值快速初始化
//    TIM_ICStructInit(&TIM_ICInitStructure);     //把TIM_ICInitStruct中的每一个参数按缺省值填入
//    TIM_ICInitStructure.TIM_ICFilter = 6;      //设置滤波器长度
//    TIM_ICInit(TIM4, &TIM_ICInitStructure);     //根据TIM_ICInitStruct的参数初始化外设TIMx
    
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1; //使用TIM通道1
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;//上升沿捕获
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;//映射到TI1上
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;//配置输入分频：不分频
    TIM_ICInitStructure.TIM_ICFilter = 0x01;    //配置输入滤波器：不滤波
    TIM_ICInit(TIM4, &TIM_ICInitStructure);     //初始化TIM输入捕获
    
    /* 中断向量控制器(NVIC)设置 */
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;         //中断线
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;      //子优先级1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);                         //初始化NVIC
    
    /* 使能TIM */
    TIM_ClearFlag(TIM4, TIM_FLAG_Update);       //清除TIMx的待处理标志位
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);  //使能指定的TIM中断
    //TIM_SetCounter(TIM4,0);                     //设置TIMx计数器寄存器值
		TIM3->CNT = 0; //计数器清零
    TIM_Cmd(TIM4, ENABLE);                      //使能TIM4 
}
	

/*           PWM                */
void PWM_init(uint16_t arr,uint16_t psc)  //TIM3的ch2对应引脚PA7 映射到 PB5
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //TIM3时钟
	//复用时钟使能 ，GPIOB时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
		
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);//设置TIM3部分重映射
  //初始化TIM3
	TIM_TimeBaseStructure.TIM_Period = arr;     //自动装载值
	TIM_TimeBaseStructure.TIM_Prescaler  = psc; //预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision  = TIM_CKD_DIV1;//设置时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode  = TIM_CounterMode_Down;//向下计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	//设置TIM3的ch2通道的PWM模式，使能CH2输出
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;  //选择PWM模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//输出极性高

	TIM_OC2Init(TIM3, &TIM_OCInitStructure); 
 

	TIM_Cmd(TIM3, ENABLE );
}


/*   定时10ms 去读编码模式计数器的值 */
void TIM6_Int_Init(u16 arr,u16 psc)
{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	  NVIC_InitTypeDef NVIC_InitStructure;
	
	
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); //使能TIM6时钟

		/*基础设置*/
		TIM_TimeBaseStructure.TIM_Period = arr;//计数值10000   
		TIM_TimeBaseStructure.TIM_Prescaler = psc;    //预分频,此值+1为分频的除数
		TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;  //采样分频
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
		TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

		TIM_ITConfig(TIM6,TIM_IT_Update, ENABLE);     //使能TIM6中断
	
  	NVIC_InitStructure.NVIC_IRQChannel =TIM6_IRQn;//TIM6中断
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//占先式优先级设置为1
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //副优先级设置为0
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//中断使能
		NVIC_Init(&NVIC_InitStructure);//中断初始
	
		TIM_Cmd(TIM6, ENABLE);     //使能定时器6
}
/* 读取编码器计数器的值后清零计数器，在周期性进行的函数中调用来获得速度信息 */
int Read_Encoder(void)
	{
    int Encoder_TIM;
    Encoder_TIM = (short)TIM4 -> CNT;  TIM4 -> CNT=0;
    return Encoder_TIM;
	}



