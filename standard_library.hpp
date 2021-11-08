#pragma once
#define PROJECT_DATA_BASE "MangoDataBase"

// #define NDEBUG
#include <initializer_list>
#include <string_view>
#include <filesystem>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <cassert>
#include <cstddef>
#include <memory>
#include <vector>
#include <string>
#include <array>
#include <set>

#define MANGO_API
#define MANGO_PUBLIC_API
#define MANGO_QUERY_API

template<typename T>
using ref = T&;

template<typename T>
using const_ref = const T&;

template<typename T>
using ptr = T*;

template<typename T>
using const_ptr = const T*;