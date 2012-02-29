/*
 * yamlgen - YAML to everything generator framework
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

#ifndef INTERPRETER_H__
#define INTERPRETER_H__

#include <yaml.h>
#include <vector>

#include <yamlgen/config_node.hpp>
#include <yamlgen/exception/interpreter_exception.hpp>
#include <yamlgen/exception/parser_exception.hpp>
#include <yamlgen/exception/builder_exception.hpp>
#include <yamlgen/exception/config_exception.hpp>

#define register_handler(mode, fun) \
	m_mode_handlers[mode] = &Interpreter::fun

namespace YAMLGen { 

class Interpreter
{

private: 

	/* 
		Right before entering a mapping mode, 
		the current config node is pushed to this 
		list for the recipient (parent mapping mode) 
		to use, after leaving the mapping mode. 
	*/
	::std::vector<ConfigNode> m_node_stack; 
	
	/* 
		When receiving the key (first scalar) in a 
		mapping mode, the current config key is pushed 
		to this list for the recipient (parent mapping 
		mode) to use, after leaving the mapping mode. 
	*/
	::std::vector< ::std::string> m_key_stack; 

	/* 
	  Config node that is currently operated on. 
		Is pushed to m_node_stack if another config 
		node (mapping start) is initialized. 
		After parsing, m_cur_node contains the 
		interpreter's result (root node). 
	*/
  ConfigNode m_cur_node; 
	

public:

  typedef enum {
    INIT=0,
	
		MAPPING_START, 
		SEQUENCE_START, 
		KEY, 
		VALUE, 
		INCLUDE, 
		COMPLETING, 
		
    ERROR,
    CONTINUE,
    DONE
  } interpreter_mode_t;

  typedef void (Interpreter::*mode_handler_fun)(yaml_event_t * event);
  mode_handler_fun m_mode_handlers[DONE+1];

private:

  interpreter_mode_t m_mode;

private: 

	::std::string m_cur_file; 

public:

  Interpreter()
    : m_mode(INIT)
  {
    register_handler(INIT,           init_mode);
		register_handler(MAPPING_START,  mapping_start_mode); 
		register_handler(SEQUENCE_START, sequence_start_mode); 
		register_handler(KEY,				     key_mode); 
		register_handler(VALUE,			     value_mode); 
		register_handler(COMPLETING,  	 completing_mode); 
  }

  ~Interpreter() {
  }

public:

  inline void log(void) const {
    ::std::vector< ConfigNode>::const_iterator it;
    ::std::vector< ConfigNode>::const_iterator end = m_node_stack.end();

		m_cur_node.log(); 
		::std::cerr << "--- Stack content (empty after successful run):" << ::std::endl;
    for(it = m_node_stack.begin(); it != end; ++it) {
      (*it).log();
    }
  }

public:

  interpreter_mode_t next_event(yaml_event_t * event);

	ConfigNode const & config() const { return m_cur_node;	}

public: 

	void read(const char * filename);

private:

  void init_mode(yaml_event_t * event);
  void mapping_start_mode(yaml_event_t * event);
  void sequence_start_mode(yaml_event_t * event);
  void key_mode(yaml_event_t * event);
  void value_mode(yaml_event_t * event);
  void completing_mode(yaml_event_t * event);

};

} /* namespace YAMLGen */

#endif /* INTERPRETER_H__ */
