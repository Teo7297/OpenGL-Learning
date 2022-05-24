#include <iostream>

class Logger
{
private:
    static std::string charsToAdd(size_t, char);

public:
    Logger();
    ~Logger();
    static void log(std::string);
    static void warning(std::string);
    static void error(std::string);
    static void logProjectInfo();
};
