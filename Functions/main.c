#define NULL '\0'
#include <stdio.h>

/********************************************************/
/*						LANCUCHY						*/
/********************************************************/

enum CompResult {NOTEQUAL, EQUAL};

enum CompResult eCompareString ( char pcStr1[], char pcStr2[] )
{
	unsigned char ucCharCounter;

	for( ucCharCounter = 0; pcStr1[ucCharCounter] == pcStr2[ucCharCounter]; ucCharCounter++ ) {
		if( NULL == pcStr1[ucCharCounter] ) {
			return EQUAL;
		}
	}
	return NOTEQUAL;
}

void TestOf_eCompareString (void)
{
    printf("eCompareString\n\n");

    printf("Test 1 - ");
    // Takie same stringi
    if (EQUAL == eCompareString("012345", "012345")) printf("OK\n"); else printf("Error\n");

    printf("Test 2 - ");
    // Rozne stringi
    if (NOTEQUAL == eCompareString("012345", "012_4")) printf("OK\n"); else printf("Error\n");

    printf("Test 3 - ");
    // Puste stringi
    if (EQUAL == eCompareString("", "")) printf("OK\n"); else printf("Error\n");

    printf("\n\n");
}

void CopyString ( char pcSource[], char pcDestination[] )
{
	unsigned char ucCharCounter;

	for( ucCharCounter = 0; NULL != pcSource[ucCharCounter]; ucCharCounter++ ) {
		pcDestination[ucCharCounter] = pcSource[ucCharCounter];
	}
	pcDestination[ucCharCounter] = NULL;
}

void TestOf_CopyString (void)
{
    char cDestination[10];

    printf("CopyString\n\n");

    printf("Test 1 - ");
    // Niepusty string
    CopyString("01234", cDestination);
    if (EQUAL == eCompareString("01234", cDestination)) printf("OK\n"); else printf("Error\n");

    printf("Test 2 - ");
    // Pusty string
    CopyString("", cDestination);
    if (EQUAL == eCompareString("", cDestination)) printf("OK\n"); else printf("Error\n");

    printf("\n\n");
}

void AppendString ( char pcSourceStr[], char pcDestinationStr[] )
{
	unsigned char ucDestinationEnd;

	for( ucDestinationEnd = 0; NULL != pcDestinationStr[ucDestinationEnd]; ucDestinationEnd++ ) {}
	CopyString( pcSourceStr, pcDestinationStr + ucDestinationEnd );
}

void TestOf_AppendString (void)
{
    char cDestination[10];

    printf("AppendString\n\n");

    printf("Test 1 - ");
    // Niepusty string do niepustego stringa
	CopyString("012", cDestination);
    AppendString("345", cDestination);
    if (EQUAL == eCompareString("012345", cDestination)) printf("OK\n"); else printf("Error\n");

    printf("Test 2 - ");
    // Pusty string do niepustego stringa
	CopyString("345", cDestination);
    AppendString("", cDestination);
    if (EQUAL == eCompareString("345", cDestination)) printf("OK\n"); else printf("Error\n");

    printf("Test 3 - ");
    // Niepusty string do pustego stringa
	CopyString("", cDestination);
    AppendString("012", cDestination);
    if (EQUAL == eCompareString("012", cDestination)) printf("OK\n"); else printf("Error\n");

    printf("\n\n");
}

void ReplaceCharactersInString ( char pcString[], char cOldChar, char cNewChar )
{
	unsigned char ucCharCounter;

	for( ucCharCounter = 0; NULL != pcString[ucCharCounter]; ucCharCounter++ ) {
		if( pcString[ucCharCounter] == cOldChar ) {
			pcString[ucCharCounter] = cNewChar;
		}
	}
}

void TestOf_ReplaceCharactersInString (void)
{
    char cString[10];

    printf("ReplaceCharactersInString\n\n");

    printf("Test 1 - ");
    // Wiecej niz jeden znak
	CopyString("33_33_33", cString);
    ReplaceCharactersInString(cString, '3', 'x');
    if (EQUAL == eCompareString("xx_xx_xx", cString)) printf("OK\n"); else printf("Error\n");

    printf("Test 2 - ");
    // Zamiana na NULL
	CopyString("53_33_33", cString);
    ReplaceCharactersInString(cString, '3', NULL);
    if (EQUAL == eCompareString("5", cString)) printf("OK\n"); else printf("Error\n");

    printf("Test 3 - ");
    // String niezawierajacy szukanego znaku
	CopyString("012", cString);
    ReplaceCharactersInString(cString, '3', 'x');
    if (EQUAL == eCompareString("012", cString)) printf("OK\n"); else printf("Error\n");

    printf("Test 4 - ");
    // Pusty string
	CopyString("", cString);
    ReplaceCharactersInString(cString, '3', 'x');
    if (EQUAL == eCompareString("", cString)) printf("OK\n"); else printf("Error\n");

    printf("\n\n");
}

/********************************************************/
/*				    KONWERSJE					        */
/********************************************************/

enum Result {OK, ERROR};

void UIntToHexStr ( unsigned int uiValue, char pcStr[] )
{
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

void TestOf_UIntToHexStr (void)
{
    char cDestination[7];

    printf("UIntToHexStr\n\n");

    printf("Test 1 - ");
    // 0 - 0x0000
    UIntToHexStr(0, cDestination);
    if (EQUAL == eCompareString("0x0000", cDestination)) printf("OK\n"); else printf("Error\n");

    printf("Test 2 - ");
    // 2479 - 0x09AF
    UIntToHexStr(2479, cDestination);
    if (EQUAL == eCompareString("0x09AF", cDestination)) printf("OK\n"); else printf("Error\n");

    printf("Test 3 - ");
    // 65535 - 0xFFFF
    UIntToHexStr(65535, cDestination);
    if (EQUAL == eCompareString("0xFFFF", cDestination)) printf("OK\n"); else printf("Error\n");

    printf("\n\n");
}

enum Result eHexStringToUInt ( char pcStr[], unsigned int *puiValue)
{
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

void TestOf_eHexStringToUInt (void)
{
    unsigned int cDestination;
    enum Result Result;

    printf("eHexStringToUInt\n\n");

    printf("Test 1 - ");
    // 0x0 - 0
    Result = eHexStringToUInt("0x0", &cDestination);
    if( (OK == Result) && (0 == cDestination) ) printf("OK\n"); else printf("Error\n");

    printf("Test 2 - ");
    // 0x09AF - 2479
    Result = eHexStringToUInt("0x09AF", &cDestination);
    if( (OK == Result) && (2479 == cDestination) ) printf("OK\n"); else printf("Error\n");

    printf("Test 3 - ");
    // 0xFFFF - 65535
    Result = eHexStringToUInt("0xFFFF", &cDestination);
    if( (OK == Result) && (65535 == cDestination) ) printf("OK\n"); else printf("Error\n");

    printf("Test 4 - ");
    // 1x0000 - ERROR
    Result = eHexStringToUInt("1x0000", &cDestination);
    if( (ERROR == Result) ) printf("OK\n"); else printf("Error\n");

    printf("Test 5 - ");
    // 0-0000 - ERROR
    Result = eHexStringToUInt("0-0000", &cDestination);
    if( (ERROR == Result) ) printf("OK\n"); else printf("Error\n");

    printf("Test 6 - ");
    // 0xFFFFF - ERROR
    Result = eHexStringToUInt("0xFFFFF", &cDestination);
    if( (ERROR == Result) ) printf("OK\n"); else printf("Error\n");

    printf("\n\n");
}

void AppendUIntToString ( unsigned int uiValue, char pcDestinationStr[] )
{
	unsigned char ucDestinationEnd;

	for( ucDestinationEnd = 0; pcDestinationStr[ucDestinationEnd] != NULL; ucDestinationEnd++ ) {}
	UIntToHexStr( uiValue, pcDestinationStr+ucDestinationEnd);
}

void TestOf_AppendUIntToString (void)
{
    char cDestination[10];

    printf("AppendUIntToString\n\n");

    printf("Test 1 - ");
    // Pusty string + 0x000A
    CopyString("", cDestination);
    AppendUIntToString(10, cDestination);
    if (EQUAL == eCompareString("0x000A", cDestination)) printf("OK\n"); else printf("Error\n");

    printf("Test 2 - ");
    // Niepusty string + 0x000A
    CopyString("MO ", cDestination);
    AppendUIntToString(10, cDestination);
    if (EQUAL == eCompareString("MO 0x000A", cDestination)) printf("OK\n"); else printf("Error\n");

    printf("\n\n");
}

/********************************************************/
/*						DEKODOWANIE						*/
/********************************************************/

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

void TestOf_ucFindTokensInString (void)
{
    unsigned char ucTokensCount;
    char cTokens[30];

    printf("ucFindTokensInString\n\n");

    printf("Test 1 - ");
    // Pusty string
    CopyString("   ", cTokens);
    ucTokensCount = ucFindTokensInString(cTokens);
    if (0 == ucTokensCount) printf("OK\n"); else printf("Error\n");

    printf("Test 2 - ");
    // Dwa tokeny, kilka delimiterow
    CopyString("token1   token2", cTokens);
    ucTokensCount = ucFindTokensInString(cTokens);
    if( (2 == ucTokensCount) &&
        (cTokens == asToken[0].uValue.pcString) &&
        (cTokens+9 == asToken[1].uValue.pcString) ) printf("OK\n"); else printf("Error\n");

    printf("Test 3 - ");
    // Ilosc tokenow ponad limit - slowo kluczowe, liczba hex, string, wielokrotne delimitery na poczatku, pomiedzy i na koncu
    CopyString("  store  0x1234  token3 token4  ", cTokens);
    ucTokensCount= ucFindTokensInString(cTokens);
    if( (3 == ucTokensCount) &&
        (cTokens+2 == asToken[0].uValue.pcString) &&
        (cTokens+9 == asToken[1].uValue.pcString) &&
        (cTokens+17 == asToken[2].uValue.pcString) ) printf("OK\n"); else printf("Error\n");

    printf("\n\n");
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

void TestOf_eStringToKeyword (void)
{
    enum Result eResult;
    enum KeywordCode eKeywordCode;

    printf("eStringToKeyword\n\n");

    printf("Test 1 - ");
    // Slowo kluczowe
    eResult = eStringToKeyword("load", &eKeywordCode);
    if( (eKeywordCode == LD) &&
        (OK == eResult) ) printf("OK\n"); else printf("Error\n");

    printf("Test 2 - ");
    // String spoza slow kluczowych
    eResult = eStringToKeyword("abc", &eKeywordCode);
    if (ERROR == eResult) printf("OK\n"); else printf("Error\n");

    printf("\n\n");
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

void TestOf_DecodeTokens (void)
{
    char cTokens[30];

    printf("DecodeTokens\n\n");

    printf("Test 1 - ");
    // S³owo kluczowe, liczba i inny string
    CopyString("store  0x1234  token3", cTokens);
    ucTokenNr = ucFindTokensInString(cTokens);
	ReplaceCharactersInString(cTokens, ' ', NULL);
	DecodeTokens();
    if( (KEYWORD == asToken[0].eType) && (ST == asToken[0].uValue.eKeyword) &&
        (NUMBER == asToken[1].eType) && (0x1234 == asToken[1].uValue.uiNumber) &&
        (STRING == asToken[2].eType) && (cTokens+15 == asToken[2].uValue.pcString)) printf("OK\n"); else printf("Error\n");

    printf("Test 2 - ");
    // Jedno slowo kluczowe, sprawdzenie pozostalych pol
    CopyString("reset", cTokens);
    ucTokenNr = ucFindTokensInString(cTokens);
	ReplaceCharactersInString(cTokens, ' ', NULL);
	DecodeTokens();
    if( (KEYWORD == asToken[0].eType) && (RST == asToken[0].uValue.eKeyword) &&
        (NUMBER == asToken[1].eType) && (0x1234 == asToken[1].uValue.uiNumber) &&
        (STRING == asToken[2].eType) && (cTokens+15 == asToken[2].uValue.pcString)
        ) printf("OK\n"); else printf("Error\n");

    printf("\n\n");
}

void DecodeMsg (char *pcString)
{
	ucTokenNr = ucFindTokensInString(pcString);
	ReplaceCharactersInString(pcString, ' ', NULL);
	DecodeTokens();

}

void TestOf_DecodeMsg (void)
{
    char cTokens[30];

    printf("DecodeMsg\n\n");

    printf("Test 1 - ");
    // S³owo kluczowe, liczba i inny string
    CopyString("store  0x1234  token3", cTokens);
    DecodeMsg(cTokens);
    if( (KEYWORD == asToken[0].eType) && (ST == asToken[0].uValue.eKeyword) &&
        (NUMBER == asToken[1].eType) && (0x1234 == asToken[1].uValue.uiNumber) &&
        (STRING == asToken[2].eType) && (cTokens+15 == asToken[2].uValue.pcString) ) printf("OK\n"); else printf("Error\n");

    printf("Test 2 - ");
    // Jedno slowo kluczowe, sprawdzenie pozostalych pol
    CopyString("reset", cTokens);
    DecodeMsg(cTokens);
    if( (KEYWORD == asToken[0].eType) && (RST == asToken[0].uValue.eKeyword) &&
        (NUMBER == asToken[1].eType) && (0x1234 == asToken[1].uValue.uiNumber) &&
        (STRING == asToken[2].eType) && (cTokens+15 == asToken[2].uValue.pcString) ) printf("OK\n"); else printf("Error\n");

    printf("\n\n");
}


int main (void)
{
	printf("TESTY FUNKCJI DO OPERACJI NA STRINGACH \n\n\n");

	TestOf_eCompareString();
	TestOf_CopyString();
	TestOf_AppendString();
	TestOf_ReplaceCharactersInString();


	printf("TESTY FUNKCJI DO KONWERSJI STRINGOW \n\n\n");

	TestOf_UIntToHexStr();
	TestOf_eHexStringToUInt();
	TestOf_AppendUIntToString();



	printf("TESTY FUNKCJI DO DEKODOWANIA KOMUNIKATOW \n\n\n");

	TestOf_ucFindTokensInString();
	TestOf_eStringToKeyword();
	TestOf_DecodeTokens();
	TestOf_DecodeMsg();

}
