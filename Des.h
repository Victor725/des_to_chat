#include<bitset>
using namespace std;

typedef int INT32;
typedef unsigned int ULONG32;
typedef unsigned char ULONG8;


class CDesOperate
{
private:
	bitset<48> keys[16];
	bitset<64> key;
    void HandleData(ULONG32* left , bool choice);
    void MakeKey();
public:
    CDesOperate(){};
    ~CDesOperate(){};
    void Encry(char* pPlaintext,  int nPlaintextLength,  char *pCipherBuffer, 
    int &nCipherBufferLength,  char *pKey,int nKeyLength);
    void Decry(char* pCipher,  int nCipherBufferLength,  char *pPlaintextBuffer, 
    int &nPlaintextBufferLength,  char *pKey,int nKeyLength);
};