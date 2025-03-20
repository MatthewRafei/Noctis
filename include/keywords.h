#ifndef KEYWORDS_H
#define KEYWORDS_H

#define KEYWORD_IF "if"
#define KEYWORD_THEN "then"
#define KEYWORD_ELSE "else"
#define KEYWORD_TRUE "true"
#define KEYWORD_FALSE "false"
#define KEYWORD_AND "and"
#define KEYWORD_OR "or"
#define KEYWORD_WHILE "while"
#define KEYWORD_DO "do"
#define KEYWORD_FOR "for"
#define KEYWORD_FUNC "func"
#define KEYWORD_NULL "NULL"
#define KEYWORD_END "end"
#define KEYWORD_PRINT "print"
#define KEYWORD_PRINTLN "println"
#define KEYWORD_RET "return"

// Keyword as compound literal 
// We can make an array of keywords
#define KEYWORD_ASCPL { \
    KEYWORD_IF, \
    KEYWORD_THEN, \
    KEYWORD_ELSE, \
    KEYWORD_TRUE, \
    KEYWORD_FALSE, \
    KEYWORD_AND, \
    KEYWORD_OR, \
    KEYWORD_WHILE, \
    KEYWORD_DO, \
    KEYWORD_FOR, \
    KEYWORD_FUNC, \
    KEYWORD_NULL, \
    KEYWORD_END, \
    KEYWORD_PRINT, \
    KEYWORD_PRINTLN, \
    KEYWORD_RET \
}

int is_keyword(const char *s);

#endif // KEYWORDS_H
