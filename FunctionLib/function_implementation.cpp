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
std::unique_ptr<function_cache> cashe_class_ptr;

HMODULE get_target_module()
{
	// Replace "YourGame.exe" with the actual game executable name
	HMODULE game_module = GetModuleHandleW(L"BrickRigs-Win64-Shipping.exe");
	if (game_module) {
		return game_module;
	}

	// Fallback
	return GetModuleHandle(NULL);
}

std::vector<uint8_t> hex_to_bytes(const std::string& hex)
{
    std::vector<uint8_t> bytes;
    std::string clean_hex;

    for (char c : hex) {
        if (isxdigit(static_cast<unsigned char>(c))) {
            clean_hex += c;
        }
    }

    if (clean_hex.length() % 2 != 0) {
        throw std::invalid_argument("Hex string must have even length");
    }

    bytes.reserve(clean_hex.length() / 2);

    for (size_t i = 0; i < clean_hex.length(); i += 2) {
        std::string byteString = clean_hex.substr(i, 2);
        uint8_t byte = static_cast<uint8_t>(strtol(byteString.c_str(), nullptr, 16));
        bytes.push_back(byte);
    }

    return bytes;
}

// Your exact algorithm
unsigned long long find_pattern(const char* pattern, const char* mask,
    unsigned long long base, unsigned long long size)
{
    unsigned long long patternLen = strlen(mask);

    for (unsigned long long i = 0; i < size - patternLen; i++) {
        bool found = true;

        for (unsigned long long j = 0; j < patternLen; j++) {
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

unsigned long long resolve_function(function& func)
{
    std::cout << "Resolving function: " << func.name << std::endl;

    try {
        // Get the CORRECT target module
        HMODULE target_module = get_target_module();
        if (!target_module) {
            std::cerr << "Failed to get target module" << std::endl;
            return 0;
        }

        // Get module information for the correct module
        MODULEINFO info{};
        if (!GetModuleInformation(GetCurrentProcess(), target_module, &info, sizeof(info))) {
            std::cerr << "Failed to get module information" << std::endl;
            return 0;
        }

        unsigned long long base = reinterpret_cast<unsigned long long>(target_module);
        unsigned long long size = info.SizeOfImage;

        std::cout << "Target module base: 0x" << std::hex << base
            << ", Size: 0x" << size << std::dec << std::endl;

        // Convert hex pattern
        std::vector<uint8_t> bytes = hex_to_bytes(func.sig);
        const char* mask = func.mask.c_str();

        // Auto-correct mask length if needed
        if (bytes.size() != strlen(mask)) {
            std::string corrected_mask = func.mask;
            corrected_mask.resize(bytes.size(), '?');
            mask = corrected_mask.c_str();
            std::cout << "Corrected mask length to: " << corrected_mask.length() << std::endl;
        }

        const char* pattern = reinterpret_cast<const char*>(bytes.data());

        // Search in the correct module
        unsigned long long address = find_pattern(pattern, mask, base, size);

        if (address) {
            std::cout << "Found " << func.name << " at: 0x" << std::hex << address
                << " (offset: 0x" << (address - base) << ")" << std::dec << std::endl;
        }
        else {
            std::cerr << "Pattern not found for: " << func.name << std::endl;

            // Debug: dump what we're searching for
            std::cout << "Pattern (" << bytes.size() << " bytes): ";
            for (size_t i = 0; i < bytes.size(); ++i) {
                printf("%02X ", bytes[i]);
            }
            std::cout << std::endl;
            std::cout << "Mask: " << mask << std::endl;
        }

        return address;

    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 0;
    }
}

bool compare_bytes(const uint8_t* data, const uint8_t* sig, const char* mask, size_t len)
{
    for (size_t i = 0; i < len; i++) {
        if (mask[i] == 'x' && data[i] != sig[i])
            return false;
    }
    return true;
}

bool verify_function(function* func)
{
    if (!func) return false;

    // Get correct module base
    HMODULE target_module = get_target_module();
    if (!target_module) return false;

    uintptr_t module_base = reinterpret_cast<uintptr_t>(target_module);

    if (func->offset != 0) {
        // Verify against correct module
        std::vector<uint8_t> sig_bytes = hex_to_bytes(func->sig);
        uint8_t* data = reinterpret_cast<uint8_t*>(module_base + func->offset);

        if (!compare_bytes(data, sig_bytes.data(), func->mask.c_str(), sig_bytes.size())) {
            std::cout << "Signature mismatch, rescanning..." << std::endl;
            func->offset = 0;
        }
    }

    if (func->offset == 0) {
        unsigned long long absolute_addr = resolve_function(*func);
        if (absolute_addr) {
            func->offset = absolute_addr - module_base;
        }
        else {
            return false;
        }
    }

    return true;
}

// Test function to diagnose the issue
void diagnose_scanning_issue()
{
    std::cout << "=== Diagnosis ===" << std::endl;

    // Test with your working pattern
    function test_func;
    test_func.name = "AddChatMessage";
    test_func.sig = "48897C241841564883EC40488BFA4C8BF1E80000000084C00F8400000000";
    test_func.mask = "xxxxxxxxxxxxxxxxxx????xxxx????";

    // Get target module info
    HMODULE target_module = get_target_module();
    if (target_module) {
        MODULEINFO info{};
        if (GetModuleInformation(GetCurrentProcess(), target_module, &info, sizeof(info))) {
            std::cout << "Target module range: 0x" << std::hex
                << reinterpret_cast<uintptr_t>(target_module) << " - 0x"
                << (reinterpret_cast<uintptr_t>(target_module) + info.SizeOfImage)
                << std::dec << std::endl;
        }
    }

    resolve_function(test_func);
    std::cout << "=== End Diagnosis ===" << std::endl;
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
	std::cout << "Loading cache from: " << path << std::endl;

	std::ifstream function_file(path, std::ios::binary);
	if (!function_file.is_open()) {
		std::cout << "Cache file not found, creating empty one" << std::endl;
		std::ofstream new_function_file(path, std::ios::trunc);
		return;
	}

	// Check if file is empty
	function_file.seekg(0, std::ios::end);
	if (function_file.tellg() == 0) {
		std::cout << "Cache file is empty" << std::endl;
		return;
	}
	function_file.seekg(0);

	json json_data;
	try {
		function_file >> json_data;
		std::cout << "Successfully parsed JSON" << std::endl;
	}
	catch (const std::exception& e) {
		std::cerr << "JSON parse error: " << e.what() << "\n";
		return;
	}

	if (json_data.is_null() || !json_data.contains("functions") || !json_data["functions"].is_array()) {
		std::cerr << "Invalid JSON structure" << std::endl;
		return;
	}

	for (const auto& funcObj : json_data["functions"]) {
		if (!does_json_obj_contain_func(funcObj)) {
			std::cerr << "Invalid function object in JSON" << std::endl;
			continue;
		}

		function input;
		input.name = funcObj["name"].get<std::string>();
		input.sig = funcObj["sig"].get<std::string>();
		input.mask = funcObj["mask"].get<std::string>();
		input.offset = funcObj["off"].get<std::uint64_t>();

		std::cout << "Processing function: " << input.name << std::endl;

		if (verify_function(&input)) {
			internal_cache.push_back(std::move(input));
			std::cout << "Successfully added function to cache" << std::endl;
		}
		else {
			std::cerr << "Failed to verify function: " << input.name << std::endl;
		}
	}

	std::cout << "Cache loading complete. Total functions: " << internal_cache.size() << std::endl;
}

// Add this debug function to help troubleshoot
void debug_pattern_search()
{
	function test_func;
	test_func.name = "AddChatMessage";
	test_func.sig = "48897C241841564883EC40488BFA4C8BF1E80000000084C00F8400000000";
	test_func.mask = "xxxxxxxxxxxxxxxxxx????xxxx????";
	test_func.offset = 0;

	// Fix the mask length to match the signature
	std::vector<uint8_t> bytes = hex_to_bytes(test_func.sig);
	if (test_func.mask.length() != bytes.size()) {
		std::cout << "Correcting mask length from " << test_func.mask.length()
			<< " to " << bytes.size() << std::endl;
		// Pad or truncate mask to match pattern length
		test_func.mask.resize(bytes.size(), '?');
	}

	verify_function(&test_func);
}

void save_cache()
{
	std::string path = get_function_json_path();

	using json = nlohmann::json;
	json obj;
	obj["functions"] = json::array();

	{   // lock while we read internal_cache
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
		std::string out = obj.dump(4);
		std::ofstream stream(path);
		stream << out;
	}
}

function_cache::function_cache()
{
	char path[MAX_PATH];
	GetModuleFileNameA(NULL, path, MAX_PATH);
	std::cout << "Current module path: " << path << std::endl;
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