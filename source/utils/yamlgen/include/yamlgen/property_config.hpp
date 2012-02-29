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

#ifndef PROPERTY_CONFIG_HPP__
#define PROPERTY_CONFIG_HPP__

#include <yamlgen/logging.hpp>
#include <yamlgen/config.hpp>
#include <yamlgen/exception/config_exception.hpp>


namespace YAMLGen {

class PropertyConfig : public Config
{ 

protected: 

	::std::string m_name; 

public: 

	typedef ::std::map< ::std::string, ::std::string> property_map;
	typedef ::std::pair< ::std::string, ::std::string> property_entry; 

	property_map m_properties; 

public: 

	PropertyConfig() 
	: m_name("") { } 

	PropertyConfig(::std::string const & name) 
	: m_name(name) { } 

	virtual ~PropertyConfig() { } 

public: 

	inline bool has_property(::std::string const & key) const { 
		return (m_properties.find(key) != m_properties.end());
	}

	inline void set_properties(const property_map & props) { 
		m_properties = props;
	}
	inline property_map const & properties(void) const { 
		return m_properties;
	}

	inline void set_property(const ::std::string & name, 
													 const ::std::string & value) 
	{
		m_properties.insert(property_entry(name, value));
	}

	inline ::std::string property(::std::string const & key) const { 
		property_map::const_iterator prop_it  = m_properties.find(key); 
		property_map::const_iterator prop_end = m_properties.end(); 
		if (prop_it == prop_end) { 
			::std::stringstream what_ss; 
			what_ss << "Could not find property " << key; 
			throw ConfigException(what_ss.str().c_str());
		}
		::std::string value = (*prop_it).second; 
		return value; 
	}

	::std::string operator[](::std::string const & key) const { 
		return property(key);
	}

public: 

	inline void name(::std::string const & name) { 
		m_name = name; 
	}
	inline ::std::string const & name(void) const { 
		return m_name; 
	}

public: 

	virtual void log(void) const { 
		property_map::const_iterator it; 
		property_map::const_iterator end = m_properties.end(); 

		LOG_INFO_PRINT("Properties in %s", m_name.c_str());
		for(it = m_properties.begin(); it != end; ++it) { 
			LOG_INFO_PRINT("|- %s = %s", 
					(*it).first.c_str(), (*it).second.c_str());
		}
	}

};

} /* namespace YAMLGen */

#endif /* PROPERTY_CONFIG_HPP__ */
