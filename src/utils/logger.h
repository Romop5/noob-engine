#ifndef _LOGGER_H
#define _LOGGER_H
#include <cstdarg>
#include <cstdio>

#define STRIZE(x) #x
#define STR(x) STRIZE(x)

#ifndef NDEBUG
#define LOG_DEBUG(...) Logger::GetLog().Log("Debug", __VA_ARGS__)
#else
#define LOG_DEBUG(...) ;
#endif

#define LOG_ERROR(...) Logger::GetLog().Log("Error", __VA_ARGS__)
#define LOG_INFO(...) Logger::GetLog().Log("Info", __VA_ARGS__)

class Logger {
  private:
    FILE *output;

  public:
    Logger() : output(stdout) {}
    static Logger GetLog() {
        static Logger log;
        return log;
    }
    void Log(const char *place, const char *message, ...) {
        va_list args;
        va_start(args, message);
        fprintf(output, "[%s] ", place);
        vfprintf(output, message, args);
        va_end(args);
    }
};
#endif
