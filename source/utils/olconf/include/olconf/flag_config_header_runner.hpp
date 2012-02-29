/*
 * olconf - yamlgen based Open-BLDC configuration header generator
 * Copyright (C) 2010 by Tobias Fuchs <twh.fuchs@gmail.com>
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

#ifndef FLAG_CONFIG_HEADER_RUNNER_HPP__
#define FLAG_CONFIG_HEADER_RUNNER_HPP__

#include <olconf/flag_config.hpp>
#include <olconf/abstract_flag_config_runner.hpp>

#include <vector>


namespace YAMLGen { 
namespace OBLDC { 

class FlagConfigHeaderRunner: public AbstractFlagConfigRunner
{ 

public: 

	FlagConfigHeaderRunner() { } 
	
	FlagConfigHeaderRunner(::std::string const & module_name) 
	: AbstractFlagConfigRunner(module_name) { } 

	virtual ~FlagConfigHeaderRunner() { } 
	
	virtual void run(FlagConfigBuilder * const builder);

};

} /* namespace OBLDC */
} /* namespace YAMLGen */

#endif /* FLAG_CONFIG_HEADER_RUNNER_HPP__ */
