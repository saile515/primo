#pragma once

#include <cstdlib>
#include <format>
#include <print>

namespace primo
{

static void info(std::string message)
{
    std::print("\033[0;37mINFO: {}\033[0m\n", message);
}

template <class... Args> static void info(std::format_string<Args...> format, Args &&...args)
{
    info(std::format(format, args...));
}

static void warning(std::string message)
{
    std::print("\033[0;33mWARNING: {}\033[0m\n", message);
}

template <class... Args> static void warning(std::format_string<Args...> format, Args &&...args)
{
    warning(std::format(format, args...));
}

static void error(std::string message)
{
    std::print(stderr, "\033[0;31mERROR: {}\033[0m\n", message);
    exit(EXIT_FAILURE);
}

template <class... Args> static void error(std::format_string<Args...> format, Args &&...args)
{
    error(std::format(format, args...));
}

} // namespace primo
