/**
 * @brief Logger
 * @author Jacob Schloss <jacob@schloss.io>
 * @copyright Copyright (c) 2019 Jacob Schloss. All rights reserved.
 * @license Licensed under the 3-Clause BSD license. See LICENSE for details
*/

#pragma once

#include "freertos_cpp_util/object_pool/Object_pool.hpp"
#include "freertos_cpp_util/Queue_static_pod.hpp"

#include "freertos_cpp_util/logging/Logger_types.hpp"
#include "freertos_cpp_util/logging/Log_sink_base.hpp"

namespace freertos_util
{
namespace logging
{

class Logger
{
public:

	Logger()
	{
		m_sink = nullptr;
	}

	void set_sink(Log_sink_base* const sink)
	{
		m_sink = sink;
	}

	bool log(const LOG_SEVERITY level, const char* module_name, char* fmt, ...);

	void process_one();

protected:

	typedef Stack_string<8+2+1> Time_str;

	static bool get_time_str(const uint32_t tick_count, Time_str* const time_str);

	static const char* LOG_SEVERITY_to_str(const LOG_SEVERITY level);

	Object_pool<String_type      , NUM_RECORDS> m_log_pool;

	Queue_static_pod<String_type*, NUM_RECORDS> m_log_buffer;

	Log_sink_base* m_sink;
};

}
}