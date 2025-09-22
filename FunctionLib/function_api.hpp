#pragma once

class function_cache
{
	function_cache();
public:

	unsigned long long get_function_pointer(const char* name);

	static function_cache* get();
};