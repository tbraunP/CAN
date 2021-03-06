#include "main.h"
#include "config.h"
#include "master/master_io.h"
#include "master/master_main.h"

#include "slave/slave_io.h"
#include "slave/slave_main.h"

/* Private function prototypes -----------------------------------------------*/
void NVIC_Config(void);
void CAN_Config(void);
void Delay(void);

int main(void) {
	/*!< At this stage the microcontroller clock setting is already configured,
	 this is done through SystemInit() function which is called from startup
	 file (startup_stm32f4xx.s) before to branch to application main.
	 To reconfigure the default setting of SystemInit() function, refer to
	 system_stm32f4xx.c file
	 */

	/* NVIC configuration */
	NVIC_Config();

#ifdef MASTER
	GPIO_Master_init();
#else
	GPIO_Slave_init();
#endif

	/* Configures LED 1..4 */
	STM_EVAL_LEDInit(LED5);
	STM_EVAL_LEDInit(LED6);
	STM_EVAL_LEDInit(LED3);
	STM_EVAL_LEDInit(LED4);

	/* Configure Push button key */
	STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_GPIO);

	/* CAN configuration */
	CAN_Config();

	/* Infinite loop */
#ifdef MASTER
	master_main();
#else
	slave_main();
#endif

}

/**
 * @brief  Configures the CAN.
 * @param  None
 * @retval None
 */
void CAN_Config(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	CAN_InitTypeDef CAN_InitStructure;
	CAN_FilterInitTypeDef CAN_FilterInitStructure;

	/* CAN GPIOs configuration **************************************************/
	/* Enable GPIO clock */
	RCC_AHB1PeriphClockCmd(CAN_GPIO_CLK, ENABLE);

	/* Connect CAN pins to AF9 */
	GPIO_PinAFConfig(CAN_GPIO_PORT, CAN_RX_SOURCE, CAN_AF_PORT);
	GPIO_PinAFConfig(CAN_GPIO_PORT, CAN_TX_SOURCE, CAN_AF_PORT);

	/* Configure CAN RX and TX pins */
	GPIO_InitStructure.GPIO_Pin = CAN_RX_PIN | CAN_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(CAN_GPIO_PORT, &GPIO_InitStructure);

	/* CAN configuration ********************************************************/
	/* Enable CAN clock */
	RCC_APB1PeriphClockCmd(CAN_CLK, ENABLE);

	/* CAN register init */
	CAN_DeInit(CANx);

	/* CAN cell init */
	CAN_InitStructure.CAN_TTCM = DISABLE;
	CAN_InitStructure.CAN_ABOM = DISABLE;
	CAN_InitStructure.CAN_AWUM = DISABLE;
	CAN_InitStructure.CAN_NART = DISABLE;
	CAN_InitStructure.CAN_RFLM = DISABLE;
	CAN_InitStructure.CAN_TXFP = DISABLE;

	//CAN_InitStructure.CAN_Mode = CAN_Mode_LoopBack;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;

	// 500 kB/s bei 42 MHz APB1 Clock
	CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq; // 5
	CAN_InitStructure.CAN_BS2 = CAN_BS2_7tq; // 6
	CAN_InitStructure.CAN_Prescaler = 5 + 1; // + 1 since assignment in CAN Init uses CAN_InitStruct->CAN_Prescaler - 1
	CAN_Init(CANx, &CAN_InitStructure);

	/* CAN filter init */
#ifdef  USE_CAN1
	CAN_FilterInitStructure.CAN_FilterNumber = 0;
#else /* USE_CAN2 */
	CAN_FilterInitStructure.CAN_FilterNumber = 14;
#endif  /* USE_CAN1 */
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);

#ifdef MASTER
	/* Enable FIFO 0 message pending Interrupt */
	CAN_ITConfig(CANx, CAN_IT_FMP0, ENABLE);
#endif
}

/**
 * @brief  Configures the NVIC for CAN.
 * @param  None
 * @retval None
 */
void NVIC_Config(void) {
#ifdef MASTER
	NVIC_InitTypeDef NVIC_InitStructure;

#ifdef  USE_CAN1 
	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
#else  /* USE_CAN2 */
	NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
#endif /* USE_CAN1 */

	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif
}

/**
 * @brief  Turn ON/OFF the dedicated led
 * @param  Ledstatus: Led number from 0 to 3.
 * @retval None
 */
void LED_Display(uint8_t Ledstatus) {
	/* Turn off all leds */
	STM_EVAL_LEDOff(LED5);
	STM_EVAL_LEDOff(LED6);
	STM_EVAL_LEDOff(LED3);
	STM_EVAL_LEDOff(LED4);

	switch (Ledstatus) {
	case (0):
		STM_EVAL_LEDOn(LED5);
		break;

	case (1):
		STM_EVAL_LEDOn(LED6);
		break;

	case (2):
		STM_EVAL_LEDOn(LED4);
		break;

	case (3):
		STM_EVAL_LEDOn(LED3);
		break;
	default:
		break;
	}
}

/**
 * @brief  Delay
 * @param  None
 * @retval None
 */
void Delay(void) {
	uint16_t nTime = 0x0000;

	for (nTime = 0; nTime < 0xFFF; nTime++) {
	}
}

#ifdef  USE_FULL_ASSERT

/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while (1)
	{
	}
}

#endif

/**
 * Stubs
 */
void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size) {
}

uint16_t EVAL_AUDIO_GetSampleCallBack(void) {
	return 0;
}
