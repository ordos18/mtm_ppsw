
#define RECEIVER_SIZE 100

enum CompResult {NOTEQUAL, EQUAL};
enum eReceiverStatus {EMPTY, READY, OVERFLOW};
struct ReceiverBuffer {
	char cData[RECEIVER_SIZE];
	unsigned char ucCharCtr;
	enum eReceiverStatus eStatus;
};

void UART_InitWithInt(unsigned int uiBaudRate);
void Receiver_PutCharacterToBuffer (char cCharacter);
enum eReceiverStatus eReceiver_GetStatus (void);
void Receiver_GetStringCopy (char * ucDestination);
void CopyString ( char pcSource[], char pcDestination[] );
enum CompResult eCompareString ( char pcStr1[], char pcStr2[] );

