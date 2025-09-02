enum ErrorLevel {
    INFO,
    WARNING,
    ERROR,
    FATAL
};

struct DiagnosticMessage {
    const char *file;
    size_t line;
    size_t col;
    enum ErrorLevel level;
    const char *fmt;
};

void report_error(const char *file, const size_t line, const size_t col, const enum ErrorLevel level, const char *fmt);