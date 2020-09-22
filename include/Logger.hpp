#include <fstream>

enum LoggingMode {
    LOG_NONE = 1 << 0,
    LOG_FILE = 1 << 1,
    LOG_STDOUT = 1 << 2,
};

class Logger {
    private:
        static std::ofstream m_fileStream;
        static int m_mode;

    public:
        static void setLoggingMode(int mode);
        static void logString(const std::string& message);
};
