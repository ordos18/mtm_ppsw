#define MAX_TOKEN_NR 3 // maksymalna dopuszczalna ilosc tokenow
#define MAX_KEYWORD_STRING_LTH 10 // maksymalna dlugosc komendy
#define MAX_KEYWORD_NR 2

typedef enum KeywordCode {CAL, GOTO} KeywordCode;

typedef union TokenValue {
	enum KeywordCode	eKeyword; // jezeli KEYWORD
	unsigned int		uiNumber; // jezeli NUMBER
	char * 				pcString; // jezeli STRING
} TokenValue;

typedef enum TokenType {KEYWORD, NUMBER, STRING} TokenType;

typedef struct Token {
	enum	TokenType	eType;	// KEYWORD, NUMBER, STRING
	union	TokenValue	uValue; // enum, unsigned int, char*
} Token;

typedef struct Keyword {
	enum KeywordCode eCode;
	char cString[MAX_KEYWORD_STRING_LTH + 1];
} Keyword;

void DecodeMsg (char *pcString);
