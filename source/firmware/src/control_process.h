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

#ifndef __CONTROL_PROCESS_H
#define __CONTROL_PROCESS_H

/**
 * Default motor alignment time.
 *
 * @todo move to central configuration header
 */
#define CONTROL_PROCESS_ALIGN_TIME 200

#include <cmsis/stm32.h>

/**
 * Control process state machine states
 */
enum control_process_state {
	cps_error = 0,
	cps_idle,
	cps_aligning,
	cps_spinup,
	cps_spinning,
	cps_num_states,
};

/**
 * Control process callback return states.
 * By evaluating these states in the calling
 * control process, callbacks are enabled to
 * influence the overall control process.
 *
 * Return states are:
 * - cps_cb_error: A general, fatal error.
 * - cps_cb_continue: No changes on process flow.
 * - cps_cb_exit_control: Control process is told
 *   to leave closed loop control and allow to
 *   resume it later.
 * - cps_cb_resume_control: Control process is
 *   tols to enter closed loop control.
 *
 */
enum control_process_cb_state {
	cps_cb_error = 0,
	cps_cb_continue,
	cps_cb_exit_control,
	cps_cb_resume_control,
	cps_cb_num_states
};

/**
 * Internal state structure
 */
struct control_process {
	enum control_process_state state;    /**< State machine state */
	bool ignite;		             /**< Ignite mode trigger */
	bool kill;		             /**< Kill motor trigger */
	uint32_t bemf_crossing_counter;	     /**< Valid BEMF crossing detection counter */
	uint32_t bemf_lost_crossing_counter; /**< Invalid BEMF crossing detection counter */
};

void control_process_init(void);
/*@unused@*/ void control_process_reset(void);
void control_process_ignite(void);
void control_process_kill(void);
void run_control_process(void);
void control_process_register_cb(enum control_process_state cp_state,
				 bool * trigger,
				 enum
				 control_process_cb_state (*callback_fun)
				 (struct control_process * cps),
				 /*@null@*/
				 enum
				 control_process_cb_state
				 (*state_in_callback_fun) (struct
							   control_process *
							   cps),
				 /*@null@*/
				 enum
				 control_process_cb_state
				 (*state_out_callback_fun) (struct
							    control_process *
							    cps));

#endif /* __CONTROL_PROCESS_H */
