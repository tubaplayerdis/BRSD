#include "function_api.hpp"
#include "function_def.hpp"
#include <vector>
#include <fstream>
#include <thread>
#include <shared_mutex>
#include <algorithm>
#include "json.hpp";
#include <libloaderapi.h>
#include <processthreadsapi.h>
#include <Psapi.h>
#include <iostream>

std::vector<function> internal_cache;
static std::shared_mutex cache_mutex;
std::unique_ptr<function_cache> cashe_class_ptr;

std::string convert_to_hex(const std::string& hex) {
	std::ostringstream oss;
	oss << '"';
	for (size_t i = 0; i < hex.size(); i += 2) {
		oss << "\\x" << hex.substr(i, 2);
	}
	oss << '"';
	return oss.str();
}


unsigned long long find_pattern_safe(const char* pattern, const char* mask, unsigned long long base, unsigned __int64 size)
{
	unsigned __int64 patternLen = strlen(mask);

	for (unsigned __int64 i = 0; i < size - patternLen; i++) {
		bool found = true;

		for (unsigned __int64 j = 0; j < patternLen; j++) {
			if (mask[j] != '?' && pattern[j] != *(char*)(base + i + j)) {
				found = false;
				break;
			}
		}

		if (found)
			return base + i;
	}

	return 0;
}

unsigned long long find_pattern_fast(const char* pattern, const char* mask, unsigned long long base, unsigned __int64 size)
{
	const unsigned __int64 patternLen = strlen(mask);
	if (patternLen == 0) {
		return 0;
	}

	// 1. Create the bad-character skip table
	std::vector<unsigned __int64> skipTable(256, patternLen);
	for (unsigned __int64 i = 0; i < patternLen - 1; ++i) {
		if (mask[i] != '?') {
			skipTable[static_cast<unsigned char>(pattern[i])] = patternLen - 1 - i;
		}
	}

	const unsigned long long searchEnd = base + size - patternLen;
	unsigned long long currentPos = base;

	while (currentPos <= searchEnd) {
		// 2. Compare from the end of the pattern backwards
		bool match = true;
		for (int j = patternLen - 1; j >= 0; --j) {
			if (mask[j] != '?' && pattern[j] != *(char*)(currentPos + j)) {
				// 3. On mismatch, use the skip table to jump forward
				// The character from the memory text determines the jump distance.
				const unsigned char mismatched_char = *(unsigned char*)(currentPos + patternLen - 1);
				currentPos += skipTable[mismatched_char];
				match = false;
				break;
			}
		}

		if (match) {
			return currentPos; // Found it
		}
	}

	return 0; // Not found
}

//Find the raw address of a function
unsigned long long resolve_function(function& func)
{
	//48897C241841564883EC40488BFA4C8BF1E80000000084C00F8400000000
	//"\x48\x89\x7C\x24\x18\x41\x56\x48\x83\xEC\x40\x48\x8B\xFA\x4C\x8B\xF1\xE8\x00\x00\x00\x00\x84\xC0\x0F\x84\x00\x00\x00\x00"
	const char* pattern = func.sig;
	const char* mask = func.mask;
	const char* hex_string = convert_to_hex(pattern).c_str();

	unsigned long long base = (unsigned long long)GetModuleHandle(NULL);
	MODULEINFO info = {};
	GetModuleInformation(GetCurrentProcess(), GetModuleHandle(NULL), &info, sizeof(info));
	unsigned long long size = (unsigned long long)info.SizeOfImage;

	unsigned long long address = find_pattern_fast(hex_string, mask, base, size);
	if (address != 0) return address;
	address = find_pattern_safe(hex_string, mask, base, size);

	if (address == 0) std::cout << "Failed to resolve: " << func.name << "\n";

	return address;
}

bool verify_function(function* func)
{
	if (!func) return false;

	//Compare opcodes
	if (func->offset != 0 && strcmp(func->sig, "*") != 0 && strcmp(func->mask, "*") != 0)
	{
		int op_len = strlen(func->sig)/2;
		int mask_len = strlen(func->mask);

		std::uintptr_t addr = (std::uintptr_t)GetModuleHandle(NULL);
	}


	//Populate offset

	if (func->offset == 0 && strcmp(func->sig, "*") != 0 && strcmp(func->mask, "*") != 0)
	{
		func->offset = resolve_function(*func);
	}

	if (func->offset == 0) return false;
}

void load_cache()
{
	using json = nlohmann::json;

	std::ifstream function_file("functions.json");

	if (!function_file.is_open())
	{
		std::ofstream new_function_file = std::ofstream("functions.json");
		new_function_file.close();
		return;
	}

	json json_data = json::parse(function_file);

	if (json_data.is_null() || !json_data.contains("functions") || !json_data["functions"].is_array()) return;

	std::unique_lock lock(cache_mutex);
	for (const auto& func : json_data["functions"]) {
		std::string name = func["name"].get<std::string>();
		std::string sig = func["sig"].get<std::string>();
		std::string mask = func["mask"].get<std::string>();
		std::uintptr_t offs = func["off"].get<std::uintptr_t>();

		function input;
		input.name = name.c_str();
		input.sig = sig.c_str();
		input.mask = sig.c_str();
		input.offset = offs;

		internal_cache.push_back(input);
	}

	loading_thread = std::thread([]() -> void
	{
		std::unique_lock copy_lock(cache_mutex);
		std::vector<function> local_cache = std::vector<function>(internal_cache);
		copy_lock.release();
		for (function func : local_cache)
		{
			func.offset = resolve_function(func);
			 
			std::unique_lock lock(cache_mutex);
			local_cache.swap(internal_cache);
		}
	});

}

function_cache::function_cache()
{
	cashe_class_ptr = std::unique_ptr<function_cache>(new function_cache());
}

function_cache* function_cache::get()
{
	if(!cashe_class_ptr) cashe_class_ptr = std::unique_ptr<function_cache>(new function_cache());
	return cashe_class_ptr.get();
}

unsigned long long function_cache::get_function_pointer(const char* name)
{
	std::shared_lock lock(cache_mutex);
	for (function &func : internal_cache)
	{
		if (strcmp(func.name, name) == 0) 
		{
			if (func.offset == 0) resolve_function(func);
		}
	}
	lock.release();



}