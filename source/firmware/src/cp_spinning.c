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
 * @file   cp_spinning.c
 * @author Tobias Fuchs <twh.fuchs@gmail.com>
 * @date   Thu Aug 19 15:59:42 2010
 *
 * @brief  Controller process spinning strategy
 *
 * Default control process implementation for spinning state.
 */

#include "cp_spinning.h"
#include "control_process.h"

#include "types.h"
#include "comm_tim.h"
#include "pwm/pwm.h"
#include "comm_process.h"
#include "driver/led.h"

/**
 * Trigger source for spinning state.
 */
static bool *control_process_spinning_trigger = &comm_tim_trigger;

/** Callback function to be hooked as handler for state
 * cps_spinning in control_process.c.
 * Implements watch process on BEMF signals. If it
 * fails to detect a crossing 10 times in a row, it
 * returns cps_cb_exit_control, thus telling the calling
 * control process to exit the closed loop.
 */
static enum control_process_cb_state
control_process_spinning_cb(struct control_process *cps)
{
	if (comm_data.bemf_crossing_detected) {
		comm_data.bemf_crossing_detected = false;
		cps->bemf_crossing_counter++;
		cps->bemf_lost_crossing_counter = 0;
		ON(LED_RED);
	} else {
		cps->bemf_crossing_counter = 0;
		cps->bemf_lost_crossing_counter++;
		OFF(LED_RED);
	}

	if (cps->bemf_lost_crossing_counter > 10) {
		return cps_cb_exit_control;
	}
	return cps_cb_continue;
}

/**
 * Initialization of the spinning callback process.
 * Calls cp_spinning_reset and registers control_process_spinning_cb
 * as handler for control process state cps_spinning.
 */
void cp_spinning_init(void)
{
	cp_spinning_reset();
	control_process_register_cb(cps_spinning,
				    control_process_spinning_trigger,
				    control_process_spinning_cb, NULL, NULL);
}

/**
 * Reset function for the aligning callback process.
 * Currently empty.
 */
void cp_spinning_reset(void)
{
}

/**
 * Do we have enough information to start spinning closed loop yet?
 */
bool cp_spinning_ready(void)
{
	return comm_process_ready();
}
