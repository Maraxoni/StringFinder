#include <iostream>
#include <optional>
#include <span>
#include <string_view>
#include <variant>
#include <vector>

#ifdef _WIN32
#define WINDOWS
#endif

#ifndef WINDOWS
using HANDLE = void*;
#include <fstream>
#else
// clang-format off
#include <windef.h>
#include <fileapi.h>
#include <winbase.h>
// clang-format on
#endif

static_assert(
    std::is_same<HANDLE, void*>::value,
    "Code assumes that Windows handle is of type void*"
);

using generic_buf = std::vector<char>;
using win_buf     = std::pair<std::span<const char>, HANDLE>;

using read_buffer_result = std::variant<std::monostate, generic_buf, win_buf>;

static read_buffer_result read_buffer(const char* filename)
{
#ifdef WINDOWS

    auto file = CreateFileA(
        filename,
        GENERIC_READ,
        FILE_SHARE_READ,
        nullptr,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        nullptr
    );

    if (file == INVALID_HANDLE_VALUE) {
        return std::monostate {};
    }

    LARGE_INTEGER size;
    auto res = GetFileSizeEx(file, &size);
    if (not res) {
        CloseHandle(file);
        return std::monostate {};
    }

    auto mapping
        = CreateFileMapping(file, nullptr, PAGE_READONLY, 0, 0, nullptr);
    CloseHandle(file);
    if (not mapping) {
        return std::monostate {};
    }

    auto buffer = MapViewOfFile(mapping, FILE_MAP_READ, 0, 0, 0);
    if (not buffer) {
        CloseHandle(mapping);
        return std::monostate {};
    }

    return {
        std::pair {
            std::span {
                static_cast<const char*>(buffer),
                (size_t)size.QuadPart,
            },
            mapping,
        },
    };

#else
    auto fstream
        = std::fstream(filename, std::ios_base::in | std::ios_base::binary);

    if (not fstream) {
        return std::monostate {};
    }

    fstream.seekg(0, std::ios_base::end);
    const auto size = fstream.tellg();
    fstream.seekg(0, std::ios_base::beg);

    auto buffer = std::vector<char>(size);
    fstream.read(buffer.data(), size);

    return buffer;
#endif
}

std::optional<size_t> search_string_impl(
    std::span<const char> buf, std::string_view pattern
)
{
    if (pattern.empty()) {
        return std::nullopt;
    }

    size_t buf_size     = buf.size();
    size_t pattern_size = pattern.size();

    if (pattern_size > buf_size) {
        return std::nullopt;
    }

    for (size_t i = 0; i <= buf_size - pattern_size; ++i) {
        size_t j = 0;
        while (j < pattern_size && buf[i + j] == pattern[j]) {
            ++j;
        }
        if (j == pattern_size) {
            return i;
        }
    }

    return std::nullopt;
}

std::optional<size_t> search_string_core(
    const char* filename, std::string_view pattern
)
{
    auto result = read_buffer(filename);

    if (std::holds_alternative<std::monostate>(result)) {
        return std::nullopt;
    }

    auto [buf_span, mapping] = [&] {
        if (std::holds_alternative<generic_buf>(result)) {
            return win_buf {
                std::get<generic_buf>(result),
                nullptr,
            };
        } else {
            return std::get<win_buf>(result);
        }
    }();
    

    auto res = search_string_impl(buf_span, pattern);

#ifdef WINDOWS
    if (mapping) {
        UnmapViewOfFile(buf_span.data());
        CloseHandle(mapping);
    }
#endif

    return res;
}