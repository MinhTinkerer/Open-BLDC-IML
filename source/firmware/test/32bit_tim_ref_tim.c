/*
 * Open-BLDC - Open BrushLess DC Motor Controller
 * Copyright (C) 2009 by Piotr Esden-Tempski <piotr@esden.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file   32bit_tim_ref_tim.c
 * @author Piotr Esden-Tempski <piotr@esden.net>
 * @date   Tue Aug 17 01:48:56 2010
 *
 * @brief  32bit timer reference timer implementation
 *
 * This is used by the 32bit timer test
 */

#include <stm32/rcc.h>
#include <stm32/misc.h>
#include <stm32/tim.h>
#include <stm32/gpio.h>

#include "types.h"
#include "test/32bit_tim_ref_tim.h"
#include "driver/32bit_tim.h"

#include "driver/led.h"

/**
 * Timer frequency
 */
volatile u16 test_tim_freq = 50000;

/**
 * Initialize the test timer peripheral.
 */
void test_tim_init(void)
{
	NVIC_InitTypeDef nvic;
	TIM_TimeBaseInitTypeDef tim_base;
	TIM_OCInitTypeDef tim_oc;
	GPIO_InitTypeDef gpio;

	/* TIM4 CH1 gpio pin setup */
	gpio.GPIO_Pin = GPIO_Pin_6;
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio);

	/* TIM4 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	/* Enable the TIM4 gloabal interrupt */
	nvic.NVIC_IRQChannel = TIM4_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0;
	nvic.NVIC_IRQChannelSubPriority = 0;
	nvic.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&nvic);

	/* TIM4 time base configuration */
	tim_base.TIM_Period = 65535;
	tim_base.TIM_Prescaler = 344;
	tim_base.TIM_ClockDivision = 0;
	tim_base.TIM_CounterMode = TIM_CounterMode_Up;
	tim_base.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM4, &tim_base);

	/* TIM4 Output Compare Timing Mode configuration: Channel1 */
	tim_oc.TIM_OCMode = TIM_OCMode_Toggle;
	tim_oc.TIM_OutputState = TIM_OutputState_Enable;
	tim_oc.TIM_Pulse = test_tim_freq;
	tim_oc.TIM_OCPolarity = TIM_OCPolarity_High;

	/* Not necessary for TIM4 because it is not an advanced timer
	 * but we are trying to make lint happy here.
	 */
	tim_oc.TIM_OutputNState = TIM_OutputNState_Disable;
	tim_oc.TIM_OCNPolarity = TIM_OCNPolarity_High;
	tim_oc.TIM_OCIdleState = TIM_OCIdleState_Set;
	tim_oc.TIM_OCNIdleState = TIM_OCNIdleState_Set;

	TIM_OC1Init(TIM4, &tim_oc);

	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

	/* TIM4 interrupt enable */
	TIM_ITConfig(TIM4, TIM_IT_CC1, ENABLE);

	TIM_Cmd(TIM4, ENABLE);
}

/**
 * Update the test timer based on frequency.
 *
 * @param freq Frequency...
 */
void test_tim_update(u16 freq)
{
	TIM_SetCompare1(TIM4, freq);
	test_tim_freq = freq;
}

/**
 * Test timer irq handler.
 */
void tim4_irq_handler(void)
{

	if (TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET) {
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);

		TIM_GenerateEvent(TIM4, TIM_EventSource_Update);

		tim_update();
	}
}
