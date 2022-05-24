#include "logger.h"
#include <ProjectConfig.h>

using namespace std;

Logger::Logger()
{
}

Logger::~Logger()
{
}

void Logger::log(std::string msg)
{
    std::cout << "\033[;37m"
              << "[log]: " << msg << "\033[0m\n";
}

void Logger::warning(std::string msg)
{
    std::cout << "\033[;33m"
              << "[warning]: " << msg << "\033[0m\n";
}

void Logger::error(std::string msg)
{
    std::cout << "\033[;31m"
              << "[error]: " << msg << "\033[0m\n";
}

void Logger::logProjectInfo()
{
    string p_name = charsToAdd(10, ' ') + PROJECT_NAME + " Project";
    string p_version = charsToAdd(10, ' ') + "Version " + PROJECT_VERSION;
    string cm_version = charsToAdd(10, ' ') + "Built using CMake version: " + CMAKE_VERSION;
    string owner = charsToAdd(10, ' ') + "Owned by " + PROJECT_OWNER;

    cout
        << charsToAdd(52, '#') << "\n"
        << "#" << charsToAdd(50, ' ') << "#"
        << "\n"
        << "#" << p_name << charsToAdd(50 - p_name.length(), ' ') << "#\n"
        << "#" << p_version << charsToAdd(50 - p_version.length(), ' ') << "#\n"
        << "#" << cm_version << charsToAdd(50 - cm_version.length(), ' ') << "#\n"
        << "#" << owner << charsToAdd(50 - owner.length(), ' ') << "#\n"
        << "#" << charsToAdd(50, ' ') << "#\n"
        << charsToAdd(52, '#') << "\n"
        << std::endl;
}

string Logger::charsToAdd(size_t count, char c)
{
    string result = "";
    for (size_t i = 0; i < count; i++)
    {
        result += c;
    }
    return result;
}