#pragma once
#include <initializer_list>
#include <string_view>
#include <filesystem>
#include <algorithm>
#include <optional>
#include <iostream>
#include <fstream>
#include <cassert>
#include <cstddef>
#include <memory>
#include <vector>
#include <string>
#include <chrono>
#include <map>
#include <set>

#define PROTECTED_API
#define PRIVATE_API
#define PUBLIC_API

#include "Utilities/ConsoleColorOutput/ConsoleColorOutput.hpp"

///resource for static_block: https://stackoverflow.com/questions/19227664/whats-the-c-idiom-equivalent-to-the-java-static-block
#define CONCATE_(X,Y) X##Y
#define CONCATE(X,Y) CONCATE_(X,Y)
#define UNIQUE(NAME) CONCATE(NAME, __LINE__)
// `UNIQUE` macro required if we expect multiple `static` blocks in function
#define static_block static Static_ UNIQUE(block) = [&]() -> void

struct Static_
{
	template<typename T>
	Static_(T only_once) { only_once(); }

	~Static_() {}  // to counter "warning: unused variable"
};

template<typename T>
using ref = T&;

template<typename T>
using const_ref = const T&;

template<typename T>
using rvalue_ref = T&&;

template<typename T>
using ptr = T*;

template<typename T>
using const_ptr = const T*;

inline std::string toUpper(std::string str)
{
	std::transform(std::cbegin(str), std::cend(str), std::begin(str), ::toupper);
	return str;
}

template<typename T>
inline void serializePOD(std::fstream& out, const_ptr<T> value, size_t itemsNumber = 1)
{
	out.write(reinterpret_cast<const char*>(value), sizeof(T) * itemsNumber);
}

template<typename T>
inline void deserializePOD(std::fstream& in, ptr<T> value, size_t itemsNumber = 1)
{
	in.read(reinterpret_cast<char*>(value), sizeof(T) * itemsNumber);
}