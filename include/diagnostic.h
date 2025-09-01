enum ErrorLevel {
    INFO,
    WARNING,
    ERROR,
    FATAL
};

void report_error(const char *file, const int row, const int col, const enum ErrorLevel level, const char *fmt);