/*
 * Open-BLDC - Open BrushLess DC Motor Controller
 * Copyright (C) 2010 by Piotr Esden-Tempski <piotr@esden.net>
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
 * @file   cp_error.c
 * @author Tobias Fuchs <twh.fuchs@gmail.com>
 * @date   Thu Aug 19 15:59:42 2010
 *
 * @brief  Controller process error strategy
 *
 * Default control process implementation for error state.
 */

#include "cp_error.h"
#include "control_process.h"

#include "types.h"
#include "comm_tim.h"
#include "pwm/pwm.h"
#include "comm_process.h"
#include "driver/led.h"

/**
 * Trigger source for error state.
 */
static bool *control_process_error_trigger = &comm_tim_trigger;

static enum control_process_cb_state
control_process_error_cb(struct control_process *cps)
{

	cps = cps;

	/**
	 * @todo: Error handling for undefined
	 * control process state here.
	 */
	return cps_cb_continue;
}

/**
 * Initialization of the error callback process.
 * Registers control_process_error_cb as handler for
 * control process state cps_error.
 */
void cp_error_init(void)
{
	control_process_register_cb(cps_error,
				    control_process_error_trigger,
				    control_process_error_cb, NULL, NULL);
}

/**
 * Reset function of the error state callback process,
 * currently empty.
 */
void cp_error_reset(void)
{
}
