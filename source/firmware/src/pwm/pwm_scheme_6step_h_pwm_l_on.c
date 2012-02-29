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
 * @file   pwm_scheme_6step_h_pwm_l_on.c
 * @author Piotr Esden-Tempski <piotr@esden.net>
 * @date   Tue Aug 17 01:55:27 2010
 *
 * @brief  Implementation of the 6step H PWM L ON scheme.
 *
 * Table of the pwm scheme zone configurations when driving:
 * @verbatim
 *  | 1| 2| 3| 4| 5| 6|
 * -+--+--+--+--+--+--+
 * A|p+|p+|  |--|--|  |
 * -+--+--+--+--+--+--+
 * B|  |--|--|  |p+|p+|
 * -+--+--+--+--+--+--+
 * C|--|  |p+|p+|  |--|
 * -+--+--+--+--+--+--+
 *  |  |  |  |  |  |  '- 360�
 *  |  |  |  |  |  '---- 300�
 *  |  |  |  |  '------- 240�
 *  |  |  |  '---------- 180�
 *  |  |  '------------- 120�
 *  |  '----------------  60�
 *  '-------------------   0�
 *
 * Legend:
 * p+: PWM on the high side
 * p-: PWM on the low side
 * --: Low side on
 * ++: High side on
 *   : Floating/NC
 * @endverbatim
 *
 * Table of the pwm scheme zone configurations when braking:
 * @verbatim
 *  | 1| 2| 3| 4| 5| 6|
 * -+--+--+--+--+--+--+
 * A|p-|p-|  |  |  |  |
 * -+--+--+--+--+--+--+
 * B|  |  |  |  |p-|p-|
 * -+--+--+--+--+--+--+
 * C|  |  |p-|p-|  |  |
 * -+--+--+--+--+--+--+
 *  |  |  |  |  |  |  '- 360�
 *  |  |  |  |  |  '---- 300�
 *  |  |  |  |  '------- 240�
 *  |  |  |  '---------- 180�
 *  |  |  '------------- 120�
 *  |  '----------------  60�
 *  '-------------------   0�
 *
 * Legend:
 * p+: PWM on the high side
 * p-: PWM on the low side
 * --: Low side on
 * ++: High side on
 *   : Floating/NC
 * @endverbatim
 *
 * Table of the BEMF states throughout the electrical circle:
 * @verbatim
 *  | 1| 2| 3| 4| 5| 6|
 * -+--+--+--+--+--+--+
 * A|++|++|+-|--|--|-+|
 * -+--+--+--+--+--+--+
 * B|+-|--|--|-+|++|++|
 * -+--+--+--+--+--+--+
 * C|--|-+|++|++|+-|--|
 * -+--+--+--+--+--+--+
 *  |  |  |  |  |  |  '- 360�
 *  |  |  |  |  |  '---- 300�
 *  |  |  |  |  '------- 240�
 *  |  |  |  '---------- 180�
 *  |  |  '------------- 120�
 *  |  '----------------  60�
 *  '-------------------   0�
 *
 * Legend:
 * ++: BEMF higher then starpoint
 * --: BEMF lower then starpoint
 * +-: BEMF transitioning from higher to lower then starpoint
 * -+: BEMF transitioning from lower to higher then starpoint
 * @endverbatim
 */

#include <stm32/tim.h>

#include "pwm/pwm_utils.h"
#include "pwm/pwm.h"

#include "pwm_scheme_6step_h_pwm_l_on.h"

/**
 * PWM scheme configuration function
 */
inline void pwm_scheme_6step_h_pwm_l_on(void)
{
	static int pwm_phase = 1;

	if (pwm_mode == PWM_DRIVE) {
		switch (pwm_phase) {
		case 1:		// 000�

			/* Configure step 2 */
			pwm_set_a_hpwm_b_low__c_off();

			pwm_phase++;
			break;
		case 2:		// 060�

			/* Configure step 3 */
			pwm_set_a_off__b_low__c_hpwm();

			pwm_phase++;
			break;
		case 3:		// 120�

			/* Configure step 4 */
			pwm_set_a_low__b_off__c_hpwm();

			pwm_phase++;
			break;
		case 4:		// 180�

			/* Configure step 5 */
			pwm_set_a_low__b_hpwm_c_off();

			pwm_phase++;
			break;
		case 5:		// 220�

			/* Configure step 6 */
			pwm_set_a_off__b_hpwm_c_low();

			pwm_phase++;
			break;
		case 6:		// 280�

			/* Configure step 1 */
			pwm_set_a_hpwm_b_off__c_low();

			pwm_phase = 1;
			break;
		}
	} else { /* pwm_mode == PWM_BRAKE */
		switch (pwm_phase) {
		case 1:		// 000�

			/* Configure step 2 */
			pwm_set_a_lpwm_b_off__c_off();

			pwm_phase++;
			break;
		case 2:		// 060�

			/* Configure step 3 */
			pwm_set_a_off__b_off__c_lpwm();

			pwm_phase++;
			break;
		case 3:		// 120�

			/* Configure step 4 */
			pwm_set_a_off__b_off__c_lpwm();

			pwm_phase++;
			break;
		case 4:		// 180�

			/* Configure step 5 */
			pwm_set_a_off__b_lpwm_c_off();

			pwm_phase++;
			break;
		case 5:		// 220�

			/* Configure step 6 */
			pwm_set_a_off__b_lpwm_c_off();

			pwm_phase++;
			break;
		case 6:		// 280�

			/* Configure step 1 */
			pwm_set_a_lpwm_b_off__c_off();

			pwm_phase = 1;
			break;
		}
	}
}
