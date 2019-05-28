
#define RECEIVER_SIZE 4

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
