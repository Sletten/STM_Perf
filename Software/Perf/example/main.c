/*-----STM32VLDISCOVERY BOARD-----*/

#include "stm32f10x.h"
#include "stm32f10x_conf.h" /*uncomment header file needed, on stm32f10x_conf.h to enable peripheral header file inclusion */

ADC_InitTypeDef CpuTempAdc;
GPIO_InitTypeDef GpioLedInit;
DMA_InitTypeDef DmaAdcToMemInit;

#define ADC1_DR_ADDRESS                  ((uint32_t)0x4001244C)
#define  DmaBufferSize 32
u16 AdcBuff[DmaBufferSize];
void LedInit(void);
void CpuTempInit(void);
void DmaAdcInit(void);

s16 Temp;
const u16 v25=1925; /*--- (1.4v/3v)*4096(12bit)--*/
const u8 avg_slope=5; /*-- (4.3mv/3v)*4096 --*/
u32	adcVal;

int main(void)
{
	LedInit();
	DmaAdcInit();
	CpuTempInit();

	/*-------------------------------------
	RCC_ClocksTypeDef clockfreq;
	RCC_GetClocksFreq(&clockfreq);
	u32 ReadAdcClock = clockfreq.ADCCLK_Frequency;
	----------------------------------------*/
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	ADC_DMACmd(ADC1,ENABLE);
	//while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));
	//adcVal=ADC_GetConversionValue(ADC1);


	while(1)
    {
		u16 dataCounter;
		dataCounter=DMA_GetCurrDataCounter(DMA1_Channel1);
		u8 a;
		adcVal=0;
		if(DMA_GetFlagStatus(DMA1_FLAG_TC1))
		{
		for(a=0;a<DmaBufferSize;a++)
		{
			adcVal+=(AdcBuff[a]);
		}
		adcVal=adcVal/32;
    	Temp=(v25-adcVal);
    	Temp=Temp/avg_slope;
    	Temp+=25;

    	if(Temp>26)
    	{
    		GPIOC->ODR=(1<<9)|(0<<8);
    	}
    	else if(Temp<=26)
    	{
    		GPIOC->ODR=(0<<9)|(1<<8);
    	}
		}
    }

}

void CpuTempInit(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);

	CpuTempAdc.ADC_ContinuousConvMode=ENABLE;
	CpuTempAdc.ADC_DataAlign=ADC_DataAlign_Right;
	CpuTempAdc.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
	CpuTempAdc.ADC_Mode=ADC_Mode_Independent;
	CpuTempAdc.ADC_NbrOfChannel=1;
	CpuTempAdc.ADC_ScanConvMode=DISABLE;
	ADC_Init(ADC1,&CpuTempAdc);

	ADC_TempSensorVrefintCmd(ENABLE);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_16,1,ADC_SampleTime_41Cycles5);
	ADC_Cmd(ADC1,ENABLE);

	ADC_ResetCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));

}

void LedInit(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);

	GpioLedInit.GPIO_Mode=GPIO_Mode_Out_PP;
	GpioLedInit.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_9;
	GpioLedInit.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOC,&GpioLedInit);
}

void DmaAdcInit(void)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	DmaAdcToMemInit.DMA_BufferSize=DmaBufferSize;
	DmaAdcToMemInit.DMA_DIR=DMA_DIR_PeripheralSRC;
	DmaAdcToMemInit.DMA_M2M=DMA_M2M_Disable;
	DmaAdcToMemInit.DMA_MemoryBaseAddr=(uint32_t)AdcBuff;
	DmaAdcToMemInit.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;
	DmaAdcToMemInit.DMA_MemoryInc=DMA_MemoryInc_Enable;
	DmaAdcToMemInit.DMA_Mode=DMA_Mode_Circular;
	DmaAdcToMemInit.DMA_PeripheralBaseAddr=ADC1_DR_ADDRESS;
	DmaAdcToMemInit.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;
	DmaAdcToMemInit.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	DmaAdcToMemInit.DMA_Priority=DMA_Priority_Medium;
	DMA_Init(DMA1_Channel1,&DmaAdcToMemInit);
	DMA_Cmd(DMA1_Channel1,ENABLE);

}