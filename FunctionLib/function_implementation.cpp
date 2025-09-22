#include "function_api.hpp"
#include <windows.h>
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

struct function
{
	std::string name;
	std::uint64_t offset;
	std::string sig;
	std::string mask;
};

std::vector<function> internal_cache;
static std::shared_mutex cache_mutex;
std::unique_ptr<function_cache> cashe_class_ptr;

std::string convert_to_hex(const std::string& hex) {
	std::ostringstream oss;
	for (size_t i = 0; i < hex.size(); i += 2) {
		oss << "\\x" << hex.substr(i, 2);
	}
	return oss.str();
}

// Convert hex string ("4889...") into bytes
std::vector<uint8_t> convert_to_bytes(const std::string& hex)
{
	std::vector<uint8_t> bytes;
	bytes.reserve(hex.size() / 2);
	for (size_t i = 0; i < hex.size(); i += 2) {
		std::string byteString = hex.substr(i, 2);
		uint8_t byte = (uint8_t)strtol(byteString.c_str(), nullptr, 16);
		bytes.push_back(byte);
	}
	return bytes;
}


// Convert hex string ("48897C...") into bytes {0x48, 0x89, 0x7C, ...}
std::vector<uint8_t> hex_to_bytes(const std::string& hex)
{
	std::vector<uint8_t> bytes;
	bytes.reserve(hex.size() / 2);
	for (size_t i = 0; i < hex.size(); i += 2) {
		std::string byteString = hex.substr(i, 2);
		uint8_t byte = static_cast<uint8_t>(strtol(byteString.c_str(), nullptr, 16));
		bytes.push_back(byte);
	}
	return bytes;
}

// Safe pattern scan
unsigned long long find_pattern_safe(const uint8_t* pattern, const char* mask,
	unsigned long long base, unsigned __int64 size)
{
	const size_t patternLen = strlen(mask);

	for (unsigned __int64 i = 0; i <= size - patternLen; i++) {
		bool found = true;

		for (size_t j = 0; j < patternLen; j++) {
			if (mask[j] != '?' && pattern[j] != *(uint8_t*)(base + i + j)) {
				found = false;
				break;
			}
		}

		if (found)
			return base + i;
	}

	return 0;
}

// Fast Boyer-Moore-Horspool pattern scan
unsigned long long find_pattern_fast(const uint8_t* pattern, const char* mask,
	unsigned long long base, unsigned __int64 size)
{
	const size_t patternLen = strlen(mask);
	if (patternLen == 0) return 0;

	// bad-character skip table
	std::vector<size_t> skipTable(256, patternLen);
	for (size_t i = 0; i < patternLen - 1; ++i) {
		if (mask[i] != '?') {
			skipTable[pattern[i]] = patternLen - 1 - i;
		}
	}

	const unsigned long long searchEnd = base + size - patternLen;
	unsigned long long currentPos = base;

	while (currentPos <= searchEnd) {
		bool match = true;

		for (int j = static_cast<int>(patternLen) - 1; j >= 0; --j) {
			if (mask[j] != '?' && pattern[j] != *(uint8_t*)(currentPos + j)) {
				const uint8_t mismatched = *(uint8_t*)(currentPos + patternLen - 1);
				currentPos += skipTable[mismatched];
				match = false;
				break;
			}
		}

		if (match) return currentPos;
	}

	return 0;
}

// Resolve a function by pattern
unsigned long long resolve_function(function& func)
{
	// Convert hex string into real bytes
	std::vector<uint8_t> bytes = hex_to_bytes(func.sig);
	const char* mask = func.mask.c_str();

	unsigned long long base = (unsigned long long)GetModuleHandle(NULL);
	MODULEINFO info{};
	GetModuleInformation(GetCurrentProcess(), (HMODULE)base, &info, sizeof(info));
	unsigned long long size = (unsigned long long)info.SizeOfImage;

	unsigned long long address = find_pattern_fast(bytes.data(), mask, base, size);
	if (address != 0) return address;
	return find_pattern_safe(bytes.data(), mask, base, size);
}

bool compare_bytes(const uint8_t* data, const uint8_t* sig, const char* mask, size_t len)
{
	for (size_t i = 0; i < len; i++) {
		if (mask[i] == 'x' && data[i] != sig[i])
			return false;
		if (mask[i] == '?') continue;
	}
	return true;
}

bool verify_function(function* func)
{
	if (!func) return false;

	//Compare opcodes
	if (func->offset != 0)
	{

		std::vector<uint8_t> sig_bytes = convert_to_bytes(func->sig);
		size_t sig_len = sig_bytes.size();

		std::uintptr_t addr = (std::uintptr_t)GetModuleHandle(NULL) + func->offset;
		uint8_t* data = reinterpret_cast<uint8_t*>(addr);

		//byte comparison failed. rescan and set the new offset
		if (!compare_bytes(data, sig_bytes.data(), func->mask.c_str(), sig_len))
		{
			func->offset = resolve_function(*func) - (std::uintptr_t)GetModuleHandle(NULL);
		}
	}

	//Populate offset
	if (func->offset == 0)
	{
		func->offset = resolve_function(*func) - (std::uintptr_t)GetModuleHandle(NULL);
	}

	if (func->offset == 0) return false;

	return true;
}

bool does_json_obj_contain_func(const nlohmann::json obj)
{
	if (!obj.contains("name")) return false;
	if (!obj.contains("sig")) return false;
	if (!obj.contains("mask")) return false;
	if (!obj.contains("off")) return false;
	return true;
}

std::string get_function_json_path()
{
	// Obtain HMODULE for THIS function (i.e. the DLL's module)
	HMODULE hMod = nullptr;
	if (!GetModuleHandleExW(
		GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
		reinterpret_cast<LPCWSTR>(&get_function_json_path),
		&hMod))
	{
		return {}; // failed
	}

	// Get module filename (wchar) with dynamic buffer growth
	std::vector<wchar_t> buf(MAX_PATH);
	DWORD len = 0;
	for (;;) {
		len = GetModuleFileNameW(hMod, buf.data(), static_cast<DWORD>(buf.size()));
		if (len == 0) return {};             // error
		if (len < buf.size()) break;         // success
		buf.resize(buf.size() * 2);          // increase and retry
	}

	// Build path to functions.json using the parent directory of the DLL
	std::filesystem::path p(buf.data());
	std::filesystem::path jsonPath = p.parent_path() / L"functions.json";
	std::wstring wpath = jsonPath.wstring();

	// Convert wide string to UTF-8 std::string
	int needed = WideCharToMultiByte(CP_UTF8, 0, wpath.c_str(), -1, nullptr, 0, nullptr, nullptr);
	if (needed == 0) return {};
	std::string out(needed - 1, '\0'); // exclude terminating null
	WideCharToMultiByte(CP_UTF8, 0, wpath.c_str(), -1, &out[0], needed, nullptr, nullptr);

	return out;
}

void load_cache()
{
	using json = nlohmann::json;

	std::string path = get_function_json_path();
	std::cout << path << std::endl;

	std::ifstream function_file(path, std::ios::binary);
	if (!function_file.is_open()) {
		// create empty file (optional)
		std::ofstream new_function_file(path, std::ios::trunc);
		return;
	}

	// check if file empty
	function_file.seekg(0, std::ios::end);
	if (function_file.tellg() == 0) {
		// nothing to parse
		return;
	}
	function_file.seekg(0);


	json json_data;
	try {
		function_file >> json_data;
	}
	catch (const std::exception& e) {
		std::cerr << "JSON parse error: " << e.what() << "\n";
		return;
	}

	if (json_data.is_null() || !json_data.contains("functions") || !json_data["functions"].is_array()) return;

	std::unique_lock lock(cache_mutex);
	for (const auto& funcObj : json_data["functions"]) {
		if (!does_json_obj_contain_func(funcObj)) continue;

		function input;
		input.name = funcObj["name"].get<std::string>();
		input.sig = funcObj["sig"].get<std::string>();
		input.mask = funcObj["mask"].get<std::string>();
		input.offset = funcObj["off"].get<std::uint64_t>();

		verify_function(&input);
		internal_cache.push_back(std::move(input));
		
	}
}

void save_cache()
{
	std::string path = get_function_json_path();
	std::string tmp = path + ".tmp";

	using json = nlohmann::json;
	json obj;
	obj["functions"] = json::array();

	{   // lock while we read internal_cache
		std::shared_lock lock(cache_mutex);
		for (const auto& funct : internal_cache) {
			obj["functions"].push_back({
				{"name", funct.name},
				{"off",  funct.offset},
				{"sig",  funct.sig},
				{"mask", funct.mask}
				});
		}
	}

	// write to temp file
	{
		std::ofstream ofs(tmp, std::ios::binary | std::ios::trunc);
		if (!ofs.is_open()) {
			std::cerr << "Failed to open temp file for writing: " << tmp << "\n";
			return;
		}
		std::string out = obj.dump(4);
		ofs << out;
		if (!ofs) {
			std::cerr << "Write failed to " << tmp << "\n";
			return;
		}
	}

	// rename (std::filesystem::rename replaces on most platforms)
	try {
		std::filesystem::rename(tmp, path);
	}
	catch (const std::exception& e) {
		std::cerr << "Failed to replace original file: " << e.what() << "\n";
	}
}

function_cache::function_cache()
{
	load_cache();
}

function_cache::~function_cache()
{
	save_cache();
}

function_cache* function_cache::get()
{
	if(!cashe_class_ptr) cashe_class_ptr = std::unique_ptr<function_cache>(new function_cache());
	return cashe_class_ptr.get();
}

unsigned long long function_cache::get_function_pointer(const char* name)
{
	for (function func : internal_cache)
	{
		if (func.name == name)
		{
			return func.offset + (std::uintptr_t)GetModuleHandle(NULL);
		}
	}
	return 0;
}