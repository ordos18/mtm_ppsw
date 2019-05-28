#define NULL '\0'

/********************************************************/
/*						LANCUCHY						*/
/********************************************************/

enum CompResult {NOTEQUAL, EQUAL};
char cDestination[] = "123";
char cSource[] = "45678";


void CopyString ( char pcSource[], char pcDestination[] ) {
	
	unsigned char ucCharCounter;
	
	for( ucCharCounter = 0; NULL != pcSource[ucCharCounter]; ucCharCounter++ ) {
		pcDestination[ucCharCounter] = pcSource[ucCharCounter];
	}
	pcDestination[ucCharCounter] = NULL;
}

enum CompResult eCompareString ( char pcStr1[], char pcStr2[] ) {
	
	unsigned char ucCharCounter;
	
	for( ucCharCounter = 0; pcStr1[ucCharCounter] == pcStr2[ucCharCounter]; ucCharCounter++ ) {
		if( NULL == pcStr1[ucCharCounter] ) {
			return EQUAL;
		}
	}
	return NOTEQUAL;
}

void AppendString ( char pcSourceStr[], char pcDestinationStr[] ) {
	
	unsigned char ucDestinationEnd;
	
	for( ucDestinationEnd = 0; NULL != pcDestinationStr[ucDestinationEnd]; ucDestinationEnd++ ) {}
	CopyString( pcSourceStr, pcDestinationStr + ucDestinationEnd );
}

void ReplaceCharactersInString ( char pcString[], char cOldChar, char cNewChar ) {
	
	unsigned char ucCharCounter;
	
	for( ucCharCounter = 0; NULL != pcString[ucCharCounter]; ucCharCounter++ ) {
		if( pcString[ucCharCounter] == cOldChar ) {
			pcString[ucCharCounter] = cNewChar;
		}
	}
}

/********************************************************/
/*						KONWERSJE						*/
/********************************************************/

enum Result {OK, ERROR};
enum Result res;
unsigned int uiValue = 5;
char cStr[10] = "0x10";

void UIntToHexStr ( unsigned int uiValue, char pcStr[] ) {
	
	unsigned char ucNibbleCounter, ucNibble;
	
	pcStr[0] = '0';
	pcStr[1] = 'x';
	for( ucNibbleCounter = 0; ucNibbleCounter < 4; ucNibbleCounter++ ) {
		ucNibble = (uiValue >> (ucNibbleCounter*4)) & 0x0F;
		if( ucNibble < 10) {
			pcStr[5-ucNibbleCounter] = '0' + ucNibble;
		} else {
			pcStr[5-ucNibbleCounter] = 'A' - 10 + ucNibble;
		}
	}
	pcStr[6] = NULL;
}

enum Result eHexStringToUInt ( char pcStr[], unsigned int *puiValue) {
	
	unsigned char ucCharacterIterator;
	char cCharacter;
	
	if( (pcStr[0] != '0') || (pcStr[1] != 'x') || (NULL == pcStr[2]) ) {
		return ERROR;
	}
	*puiValue = 0;
	for( ucCharacterIterator = 2; pcStr[ucCharacterIterator] != NULL ; ucCharacterIterator++ ) {
		cCharacter = pcStr[ucCharacterIterator];
		if( 6 == ucCharacterIterator ) {return ERROR;}
		*puiValue = (*puiValue << 4) + cCharacter;
		if( cCharacter >= 'A') {
			*puiValue = *puiValue - 'A' + 10;
		} else {
			*puiValue = *puiValue - '0';
		}
	}
	return OK;
}
/*
void AppendUIntToString ( unsigned int uiValue, char pcDestinationStr[] ) {
	
	for( ; *pcDestinationStr != NULL; pcDestinationStr++ ) {}
	UIntToHexStr( uiValue, pcDestinationStr);
}
*/
void AppendUIntToString ( unsigned int uiValue, char pcDestinationStr[] ) {
	
	unsigned char ucDestinationEnd;
	
	for( ucDestinationEnd = 0; pcDestinationStr[ucDestinationEnd] != NULL; ucDestinationEnd++ ) {}
	UIntToHexStr( uiValue, pcDestinationStr+ucDestinationEnd);
}

/********************************************************/
/*						DEKODOWANIE						*/
/********************************************************/

char cTokens1[] = "   ";
char cTokens2[] = "token1 token2  token3";
char cTokens3[] = "  store 0x1234  token3 ";
char cToken1[] = "load";
char cToken2[] = "reset ";
char cToken3[] = "abc";

#define MAX_TOKEN_NR 3 // maksymalna dopuszczalna ilosc tokenow
#define MAX_KEYWORD_STRING_LTH 10 // maksymalna dlugosc komendy
#define MAX_KEYWORD_NR 3

// juz uzyte przy konwersjach
//enum Result {OK, ERROR};

typedef enum KeywordCode {LD, ST, RST} KeywordCode;

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

struct Token asToken[MAX_TOKEN_NR];
unsigned char ucTokenNr;

struct Keyword asKeywordList[MAX_KEYWORD_NR] =
{
	{RST, "reset"},
	{LD,  "load" },
	{ST,  "store"}
};

unsigned char ucFindTokensInString (char *pcString)
{
	enum TokenDelimiter {TOKEN, DELIMITER};
	enum TokenDelimiter eTokenDelimiter = DELIMITER;
	unsigned char ucCharacterIterator, ucTokensCounter;
	char cCharacter;
	ucTokensCounter = 0;
	
	for (ucCharacterIterator = 0; ; ucCharacterIterator++)
	{
		cCharacter = pcString[ucCharacterIterator];
		
		switch (eTokenDelimiter)
		{
			case TOKEN:
				if (NULL == cCharacter) {
					return ucTokensCounter;
				} else if (' ' == cCharacter) {
					eTokenDelimiter = DELIMITER;
				}
				break;
			case DELIMITER:
				if ( MAX_TOKEN_NR == ucTokensCounter) {
					return ucTokensCounter;
				} else if (NULL == cCharacter) {
					return ucTokensCounter;
				} else if (' ' != cCharacter) {
					eTokenDelimiter = TOKEN;
					asToken[ucTokensCounter].uValue.pcString = pcString + ucCharacterIterator;
					ucTokensCounter++;
				}
				break;
			default: {}
		}
	}
}

enum Result eStringToKeyword (char pcStr[], enum KeywordCode *peKeywordCode)
{	
	unsigned char ucKeywordIterator;
	
	for (ucKeywordIterator = 0; ucKeywordIterator < MAX_KEYWORD_NR; ucKeywordIterator++)
	{
		if (eCompareString(pcStr, asKeywordList[ucKeywordIterator].cString) == EQUAL)
		{
			*peKeywordCode = asKeywordList[ucKeywordIterator].eCode;
			return OK;
		}
	}
	return ERROR;
}

void DecodeTokens (void)
{
	unsigned char ucTokenIterator;
	Token *psToken;
	
	for (ucTokenIterator = 0; ucTokenIterator < ucTokenNr; ucTokenIterator++)
	{
		psToken = &asToken[ucTokenIterator];
		if (eStringToKeyword(psToken->uValue.pcString, &psToken->uValue.eKeyword) == OK) {
			psToken->eType = KEYWORD;
		} else if (eHexStringToUInt(psToken->uValue.pcString, &psToken->uValue.uiNumber) == OK) {
			psToken->eType = NUMBER;
		} else {
			psToken->eType = STRING;
		}
	}
}

void DecodeMsg (char *pcString)
{
	ucTokenNr = ucFindTokensInString(pcString);
	ReplaceCharactersInString(pcString, ' ', NULL);
	DecodeTokens();
	
}


int main (void) {
	
	//CopyString(cSource, cDestination);
	//UIntToHexStr(uiValue, cStr);
	//res = eHexStringToUInt(cStr, &uiValue);
	//AppendUIntToString(uiValue, cStr);
	
	/*
	// Tokeny 1&3
	unsigned char cnt1, cnt2, cnt3;
	
	cnt1 = ucFindTokensInString(cTokens1);
	DecodeTokens();
	cnt2 = ucFindTokensInString(cTokens2);
	DecodeTokens();
	cnt3 = ucFindTokensInString(cTokens3);
	DecodeTokens();
	*/
	
	/*
	// Tokeny 2
	enum Result Res;
	enum KeywordCode Code;
	Res = eStringToKeyword(cToken1, &Code);
	Res = eStringToKeyword(cToken2, &Code);
	Res = eStringToKeyword(cToken3, &Code);
	*/
	
	
	// Tokeny 4
	DecodeMsg(cTokens1);
	DecodeMsg(cTokens2);
	DecodeMsg(cTokens3);
}
