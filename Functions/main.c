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
    char cStringA1[] = "012345";
    char cStringA2[] = "012345";

    char cStringB1[] = "012345";
    char cStringB2[] = "012_4";

    char cStringC1[] = "";
    char cStringC2[] = "";

    printf("eCompareString\n\n");

    printf("Test 1 - ");
    // Test porównania takich samych stringów
    if (EQUAL == eCompareString(cStringA1, cStringA2)) printf("OK\n"); else printf("Error\n");

    printf("Test 2 - ");
    // Test porównania ró¿nych stringów
    if (NOTEQUAL == eCompareString(cStringB1, cStringB2)) printf("OK\n"); else printf("Error\n");

    printf("Test 3 - ");
    // Test porównania pustych stringów
    if (EQUAL == eCompareString(cStringC1, cStringC2)) printf("OK\n"); else printf("Error\n");

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
    char cSource1[] = "01234";
    char cSource2[] = "abc";
    char cSource3[] = "";
    char cDestination[] = "56789";

    printf("CopyString\n\n");

    printf("Test 1 - ");
    // Test kopiowania stringa do tablicy zawieraj¹cej innego stringa o tej samej d³ugoœci
    CopyString(cSource1, cDestination);
    if (EQUAL == eCompareString(cSource1, cDestination)) printf("OK\n"); else printf("Error\n");

    printf("Test 2 - ");
    // Test kopiowania stringa do tablicy zawieraj¹cej innego stringa o wiêkszej d³ugoœci
    CopyString(cSource2, cDestination);
    if (EQUAL == eCompareString(cSource2, cDestination)) printf("OK\n"); else printf("Error\n");

    printf("Test 3 - ");
    // Test kopiowania pustego stringa do tablicy zawieraj¹cej innego stringa
    CopyString(cSource3, cDestination);
    if (EQUAL == eCompareString(cSource3, cDestination)) printf("OK\n"); else printf("Error\n");

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
    char cSource1[] = "345";
    char cDestination1[10] = "012";
    char cExpected1[] = "012345";

    char cSource2[] = "";
    char cDestination2[10] = "345";
    char cExpected2[] = "345";

    char cSource3[] = "012";
    char cDestination3[10] = "";
    char cExpected3[] = "012";

    printf("AppendString\n\n");

    printf("Test 1 - ");
    // Test dopisania stringa do tablicy zawieraj¹cej innego stringa
    AppendString(cSource1, cDestination1);
    if (EQUAL == eCompareString(cExpected1, cDestination1)) printf("OK\n"); else printf("Error\n");

    printf("Test 2 - ");
    // Test dopisania pustego stringa do tablicy zawieraj¹cej innego stringa
    AppendString(cSource2, cDestination2);
    if (EQUAL == eCompareString(cExpected2, cDestination2)) printf("OK\n"); else printf("Error\n");

    printf("Test 3 - ");
    // Test dopisania stringa do tablicy zawieraj¹cej pustego stringa
    AppendString(cSource3, cDestination3);
    if (EQUAL == eCompareString(cExpected3, cDestination3)) printf("OK\n"); else printf("Error\n");

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
    char cString1[] = "33_33_33";
    char cOldChar1 = '3';
    char cNewChar1 = 'x';
    char cExpected1[] = "xx_xx_xx";

    char cString2[] = "53_33_33";
    char cOldChar2 = '3';
    char cNewChar2 = NULL;
    char cExpected2[] = "5";

    char cString3[] = "012";
    char cOldChar3 = '3';
    char cNewChar3 = 'x';
    char cExpected3[] = "012";

    char cString4[] = "";
    char cOldChar4 = '3';
    char cNewChar4 = 'x';
    char cExpected4[] = "";

    printf("ReplaceCharactersInString\n\n");

    printf("Test 1 - ");
    // Test zamiany wielu znaków stringa na inny znak
    ReplaceCharactersInString(cString1, cOldChar1, cNewChar1);
    if (EQUAL == eCompareString(cExpected1, cString1)) printf("OK\n"); else printf("Error\n");

    printf("Test 2 - ");
    // Test zamiany znaku stringa na NULL
    ReplaceCharactersInString(cString2, cOldChar2, cNewChar2);
    if (EQUAL == eCompareString(cExpected2, cString2)) printf("OK\n"); else printf("Error\n");

    printf("Test 3 - ");
    // Test zamiany znaku w stringu niezawieraj¹cym tego znaku
    ReplaceCharactersInString(cString3, cOldChar3, cNewChar3);
    if (EQUAL == eCompareString(cExpected3, cString3)) printf("OK\n"); else printf("Error\n");

    printf("Test 4 - ");
    // Test zamiany znaku w pustym stringu
    ReplaceCharactersInString(cString4, cOldChar4, cNewChar4);
    if (EQUAL == eCompareString(cExpected4, cString4)) printf("OK\n"); else printf("Error\n");

    printf("\n\n");
}

/********************************************************/
/*						KONWERSJE						*/
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
    unsigned int uiValue1 = 0;
    char cDestination1[7];
    char cResult1[] = "0x0000";

    unsigned int uiValue2 = 10;
    char cDestination2[7];
    char cResult2[] = "0x000A";

    unsigned int uiValue3 = 65535;
    char cDestination3[7];
    char cResult3[] = "0xFFFF";

    printf("UIntToHexStr\n\n");

    printf("Test 1 - ");
    // Test zamiany najmniejszej wartoœci - 0 - 0x0000
    UIntToHexStr(uiValue1, cDestination1);
    if (EQUAL == eCompareString(cResult1, cDestination1)) printf("OK\n"); else printf("Error\n");

    printf("Test 2 - ");
    // Test zamiany wartoœci niebrzegowej - 10 - 0x000A
    UIntToHexStr(uiValue2, cDestination2);
    if (EQUAL == eCompareString(cResult2, cDestination2)) printf("OK\n"); else printf("Error\n");

    printf("Test 3 - ");
    // Test zamiany najwiêkszej wartoœci - 65535 - 0xFFFF
    UIntToHexStr(uiValue3, cDestination3);
    if (EQUAL == eCompareString(cResult3, cDestination3)) printf("OK\n"); else printf("Error\n");

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
    char uiValue1[] = "0x0000";
    unsigned int cDestination1;
    unsigned int cExpected1 = 0;
    enum Result Result1;

    char uiValue2[] = "0x000A";
    unsigned int cDestination2;
    unsigned int cExpected2 = 10;
    enum Result Result2;

    char uiValue3[] = "0xFFFF";
    unsigned int cDestination3;
    unsigned int cExpected3 = 65535;
    enum Result Result3;

    printf("eHexStringToUInt\n\n");

    printf("Test 1 - ");
    // Test zamiany najmniejszej wartoœci - 0x0000 - 0
    Result1 = eHexStringToUInt(uiValue1, &cDestination1);
    if( (OK == Result1) && (cExpected1 == cDestination1) ) printf("OK\n"); else printf("Error\n");

    printf("Test 2 - ");
    // Test zamiany wartoœci niebrzegowej - 0x000A - 10
    Result2 = eHexStringToUInt(uiValue2, &cDestination2);
    if( (OK == Result2) && (cExpected2 == cDestination2) ) printf("OK\n"); else printf("Error\n");

    printf("Test 3 - ");
    // Test zamiany najwiêkszej wartoœci - 0xFFFF - 65535
    Result3 = eHexStringToUInt(uiValue3, &cDestination3);
    if( (OK == Result3) && (cExpected3 == cDestination3) ) printf("OK\n"); else printf("Error\n");

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
    unsigned int uiValue1 = 10;
    char cDestination1[10] = "";
    char cResult1[] = "0x000A";

    unsigned int uiValue2 = 10;
    char cDestination2[10] = "MO";
    char cResult2[] = "MO0x000A";

    printf("AppendUIntToString\n\n");

    printf("Test 1 - ");
    // Test dopisania do pustego stringa
    AppendUIntToString(uiValue1, cDestination1);
    if (EQUAL == eCompareString(cResult1, cDestination1)) printf("OK\n"); else printf("Error\n");

    printf("Test 2 - ");
    // Test dopisania do niepustego stringa
    AppendUIntToString(uiValue2, cDestination2);
    if (EQUAL == eCompareString(cResult2, cDestination2)) printf("OK\n"); else printf("Error\n");

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
    char cTokens1[] = "   ";
    unsigned char ucCount1;
    unsigned char ucExpectedCount1 = 0;

    char cTokens2[] = "token1  token2";
    unsigned char ucCount2;
    unsigned char ucExpectedCount2 = 2;
    char cToken2_1[] = "token1  token2";
    char cToken2_2[] = "token2";

    char cTokens3[] = "  store  0x1234  token3 token 4  ";
    unsigned char ucCount3;
    unsigned char ucExpectedCount3 = 3;
    char cToken3_1[] = "store  0x1234  token3 token 4  ";
    char cToken3_2[] = "0x1234  token3 token 4  ";
    char cToken3_3[] = "token3 token 4  ";

    printf("ucFindTokensInString\n\n");

    printf("Test 1 - ");
    // Test pustego stringa
    ucCount1 = ucFindTokensInString(cTokens1);
    if (ucCount1 == ucExpectedCount1) printf("OK\n"); else printf("Error\n");

    printf("Test 2 - ");
    // Test dwóch tokenów z kilkoma delimiterami pomiêdzy nimi
    ucCount2 = ucFindTokensInString(cTokens2);
    if( (ucCount2 == ucExpectedCount2) &&
        (EQUAL == eCompareString(asToken[0].uValue.pcString, cToken2_1)) &&
        (EQUAL == eCompareString(asToken[1].uValue.pcString, cToken2_2)) ) printf("OK\n"); else printf("Error\n");

    printf("Test 3 - ");
    // Test czterech tokenów z kilkoma delimiterami na pocz¹tku, koñcu i pomiêdzy nimi
    ucCount3 = ucFindTokensInString(cTokens3);
    if( (ucCount3 == ucExpectedCount3) &&
        (EQUAL == eCompareString(asToken[0].uValue.pcString, cToken3_1)) &&
        (EQUAL == eCompareString(asToken[1].uValue.pcString, cToken3_2)) &&
        (EQUAL == eCompareString(asToken[2].uValue.pcString, cToken3_3)) ) printf("OK\n"); else printf("Error\n");

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
    char cString1[] = "load";
    enum Result eResult1;
    enum KeywordCode eDestination1;
    enum KeywordCode eExpected1 = LD;

    char cString2[] = "abc";
    enum Result eResult2;
    enum KeywordCode eDestination2;

    printf("eStringToKeyword\n\n");

    printf("Test 1 - ");
    // Test s³owa kluczowego
    eResult1 = eStringToKeyword(cString1, &eDestination1);
    if( (eDestination1 == eExpected1) &&
        (OK == eResult1) ) printf("OK\n"); else printf("Error\n");

    printf("Test 2 - ");
    // Test stringa spoza s³ów kluczowych
    eResult2 = eStringToKeyword(cString2, &eDestination2);
    if (ERROR == eResult2) printf("OK\n"); else printf("Error\n");

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
    char cTokens1[] = "store  0x1234  token3";
    char cTokens2[] = "reset";

    printf("DecodeTokens\n\n");

    printf("Test 1 - ");
    // Test s³owa kluczowego, liczby i innego stringa
    ucTokenNr = ucFindTokensInString(cTokens1);
	ReplaceCharactersInString(cTokens1, ' ', NULL);
	DecodeTokens();
    if( (KEYWORD == asToken[0].eType) && (ST == asToken[0].uValue.eKeyword) &&
        (NUMBER == asToken[1].eType) && (0x1234 == asToken[1].uValue.uiNumber) &&
        (STRING == asToken[2].eType) && (EQUAL == eCompareString("token3", asToken[2].uValue.pcString)) ) printf("OK\n"); else printf("Error\n");

    printf("Test 2 - ");
    // Test jednego s³owa kluczowego, sprawdzenie czy pozosta³e pola nie zosta³y zmienione po poprzednim wywo³aniu
    ucTokenNr = ucFindTokensInString(cTokens2);
	ReplaceCharactersInString(cTokens2, ' ', NULL);
	DecodeTokens();
    if( (KEYWORD == asToken[0].eType) && (RST == asToken[0].uValue.eKeyword) &&
        (NUMBER == asToken[1].eType) && (0x1234 == asToken[1].uValue.uiNumber) &&
        (STRING == asToken[2].eType) && (EQUAL == eCompareString("token3", asToken[2].uValue.pcString)) ) printf("OK\n"); else printf("Error\n");

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
    char cTokens1[] = "store  0x1234  token3";
    char cTokens2[] = "reset";

    printf("DecodeMsg\n\n");

    printf("Test 1 - ");
    // Test s³owa kluczowego, liczby i innego stringa
    DecodeMsg(cTokens1);
    if( (KEYWORD == asToken[0].eType) && (ST == asToken[0].uValue.eKeyword) &&
        (NUMBER == asToken[1].eType) && (0x1234 == asToken[1].uValue.uiNumber) &&
        (STRING == asToken[2].eType) && (EQUAL == eCompareString("token3", asToken[2].uValue.pcString)) ) printf("OK\n"); else printf("Error\n");

    printf("Test 2 - ");
    // Test jednego s³owa kluczowego, sprawdzenie czy pozosta³e pola nie zosta³y zmienione po poprzednim wywo³aniu
    DecodeMsg(cTokens2);
    if( (KEYWORD == asToken[0].eType) && (RST == asToken[0].uValue.eKeyword) &&
        (NUMBER == asToken[1].eType) && (0x1234 == asToken[1].uValue.uiNumber) &&
        (STRING == asToken[2].eType) && (EQUAL == eCompareString("token3", asToken[2].uValue.pcString)) ) printf("OK\n"); else printf("Error\n");

    printf("\n\n");
}


int main (void)
{
	printf("TESTY FUNKCJI DO OPERACJI NA STRINGACH \n\n\n");

	TestOf_eCompareString();
	TestOf_CopyString();
	TestOf_AppendString();
	TestOf_ReplaceCharactersInString();


	printf("TESTY FUNKCJI DO KONWERSJI STRINGÓW \n\n\n");

	TestOf_UIntToHexStr();
	TestOf_eHexStringToUInt();
	TestOf_AppendUIntToString();



	printf("TESTY FUNKCJI DO DEKODOWANIA KOMUNIKATÓW \n\n\n");

	TestOf_ucFindTokensInString();
	TestOf_eStringToKeyword();
	TestOf_DecodeTokens();
	TestOf_DecodeMsg();

}
