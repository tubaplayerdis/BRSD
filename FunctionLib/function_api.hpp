#pragma once
#include <exception>
#include <memory>

class function_exception : public std::exception
{
	const char* message;

	public:
	function_exception(const char* msg) : message(msg) {}

	const char* what() const noexcept {
		return message;
	}
};

class function_cache
{
	friend struct std::default_delete<function_cache>;

	function_cache();
	~function_cache();
public:

	unsigned long long get_function_pointer(const char* name);

	static function_cache* get();
};