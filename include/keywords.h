#ifndef KEYWORDS_H
#define KEYWORDS_H

#define KEYWORD_IF      "if"
#define KEYWORD_THEN    "then"
#define KEYWORD_ELSE    "else"
#define KEYWORD_TRUE    "true"
#define KEYWORD_FALSE   "false"
#define KEYWORD_OR      "or"
#define KEYWORD_WHILE   "while"
#define KEYWORD_DO      "do"
#define KEYWORD_FOR     "for"
#define KEYWORD_FUNC    "func"
#define KEYWORD_NULL    "NULL"
#define KEYWORD_RETURN  "return"
#define KEYWORD_IMPORT  "import"
#define KEYWORD_EXPORT  "export"
#define KEYWORD_FLOAT   "float"
#define KEYWORD_INT     "int"
#define KEYWORD_CHAR    "char"
#define KEYWORD_STRING  "string"
#define KEYWORD_UNIT    "unit"
#define KEYWORD_LET     "let"
#define KEYWORD_MUT     "mut"

// Keyword as compound literal 
// We can make an array of keywords
// Dont forget a comma. It will fail the assert
#define KEYWORD_ASCPL {				\
    KEYWORD_IF,					\
    KEYWORD_THEN,				\
    KEYWORD_ELSE,				\
    KEYWORD_TRUE,				\
    KEYWORD_FALSE,				\
    KEYWORD_OR,					\
    KEYWORD_WHILE,				\
    KEYWORD_DO,					\
    KEYWORD_FOR,				\
    KEYWORD_FUNC,				\
    KEYWORD_NULL,				\
    KEYWORD_RETURN,				\
    KEYWORD_IMPORT,				\
    KEYWORD_EXPORT,				\
    KEYWORD_FLOAT,				\
    KEYWORD_INT,				\
    KEYWORD_CHAR,				\
    KEYWORD_STRING,				\
    KEYWORD_UNIT,				\
    KEYWORD_LET,				\
    KEYWORD_MUT, 				\
}

#endif // KEYWORDS_H
