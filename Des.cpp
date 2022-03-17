#include<iostream>
#include<bitset>
#include<string>
#include<string.h>
#include "Des.h"

using namespace std;

bool DESENCRY=1;
bool DESDECRY=0;

//PC-1选位表
int pc1Table[56] = {
	57,49,41,33,25,17,9,1,
	58,50,42,34,26,18,10,2,
	59,51,43,35,27,19,11,3,
	60,52,44,36,63,55,47,39,
	31,23,15,7,62,54,46,38,
	30,22,14,6,61,53,45,37,
	29,21,13,5,28,20,12,4
};
//PC-2选位表
int pc2Table[48] =
{
	14,17,11,24,1,5,
	3,28,15,6,21,10,
	23,19,12,4,26,8,
	16,7,27,20,13,2,
	41,52,31,37,47,55,
	30,40,51,45,33,48,
	44,49,39,56,34,53,
	46,42,50,36,29,32
};
//左移位数表
int loopTable[16] =
{
	1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1
};
//IP表（初始置换表）
int ipTable[64] =
{
	58,50,42,34,26,18,10,2,
	60,52,44,36,28,20,12,4,
	62,54,46,38,30,22,14,6,
	64,56,48,40,32,24,16,8,
	57,49,41,33,25,17,9,1,
	59,51,43,35,27,19,11,3,
	61,53,45,37,29,21,13,5,
	63,55,47,39,31,23,15,7
};
//逆置换IP^-1表(解密时IP初始置换的逆)
int ipReverseTable[64] = {
	40,8,48,16,56,24,64,32,
	39,7,47,15,55,23,63,31,
	38,6,46,14,54,22,62,30,
	37,5,45,13,53,21,61,29,
	36,4,44,12,52,20,60,28,
	35,3,43,11,51,19,59,27,
	34,2,42,10,50,18,58,26,
	33,1,41,9,49,17,57,25
};
//扩展置换表(E盒)
int extendTable[48] =
{
	32,1,2,3,4,5,
	4,5,6,7,8,9,
	8,9,10,11,12,13,
	12,13,14,15,16,17,
	16,17,18,19,20,21,
	20,21,22,23,24,25,
	24,25,26,27,28,29,
	28,29,30,31,32,1
};

//八个S盒，三维数组存储
int sBox[8][4][16] =
{
	//S1
	14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7,
	0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8,
	4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0,
	15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13,
	//S2
	15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10,
	3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5,
	0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15,
	13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9,
	//S3
	10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8,
	13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1,
	13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7,
	1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12,
	//S4
	7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15,
	13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9,
	10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4,
	3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14,
	//S5
	2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9,
	14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6,
	4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14,
	11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3,
	//S6
	12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11,
	10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8,
	9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6,
	4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13,
	//S7
	4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1,
	13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6,
	1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2,
	6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12,
	//S8
	13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7,
	1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2,
	7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8,
	2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11
};

//置换表(P盒)
int pTable[32] =
{
	16,7,20,21,
	29,12,28,17,
	1,15,23,26,
	5,18,31,10,
	2,8,24,14,
	32,27,3,9,
	19,13,30,6,
	22,11,4,25
};


bitset<64> ptr_to_bits64(char* ptr){
    //len of ptr is 8bytes
    string s_result;
    for(int i=0;i<8;i++){
        bitset<8> tem(ptr[i]);
        s_result+=tem.to_string();
    }
    bitset<64> result(s_result);
    return result;
}


void bits64_to_ptr(bitset<64> bits, char* result){
    for(int i=0;i<8;i++){
        bitset<8> tem(bits.to_string().substr(8*i,8));
        result[i]=tem.to_ulong();
    }
}

void init_map(bitset<64>& txt) {
	bitset<64> tem = txt;
	for (int i = 0; i < 64; i++) {
		tem[64-1-i] = txt[64-1-(ipTable[i] - 1)];
	}
	txt = tem;
}

bitset<32> Sbox(bitset <48> k) {
	bitset<32> S_R;
	string k_s = k.to_string();
	//将k拆分为8个6位的小段
	bitset<6> ss[8];
	for (int i = 0; i < 8; i++) {
		string ss_s = k_s.substr(i * 6, 6);
		bitset<6> tem(ss_s);
		ss[i] = tem;
	}

	//最终生成8个4位小段
	bitset<4> SS[8];
	string S_Rs="";
	for (int i = 0; i < 8; i++) {
		int row = ss[i][6-1-0] * 2 + ss[i][6-1-5]; //前后两位拼起来，决定行数
		int col = ss[i][6-1-1] * 8 + ss[i][6-1-2] * 4 + ss[i][6-1-3] * 2 + ss[i][6-1-4] * 1; //中间四位拼起来，决定列数

		bitset<4> tem(sBox[i][row][col]);
		SS[i] = tem;
		//8个4位小段拼成32位结果
		S_Rs += SS[i].to_string();
	}
	bitset<32> tem(S_Rs);
	S_R = tem;
	return S_R;
}


bitset<64> rounds(bitset<64> txt, bitset<48>* keys) {
	bitset<64> out;
	//分为左右32位:R , L
	string L_s = txt.to_string().substr(0, 32); //提取高32位，转为字符串
	bitset<32> L(L_s); //以字符串来初始化左半部分
	//右半部分同理
	string R_s = txt.to_string().substr(32, 32); //提取低28位，转为字符串
	bitset<32> R(R_s); //以字符串来初始化左半部分

	//16轮
	for (int i = 0; i < 16; i++) {
		bitset<32> temp = R; //将来赋给L
		
		//对于R
		//扩展置换
		bitset<48> E_R; //进行扩展置换后的R
		for (int i = 0; i < 48; i++) {
			E_R[48-1-i] = R[32-1-(extendTable[i] - 1)];
		}
		//和本轮的key[i]进行逐比特异或
		E_R = E_R ^ keys[i];

		//S盒
		bitset<32> S_R=Sbox(E_R); //进行S盒代换后的R

		//置换P
		bitset<32> P_R;
		for (int i = 0; i < 32; i++) {
			P_R[32-1-i] = S_R[32-1-(pTable[i] - 1)];
		}
		R = L ^ P_R;
		L = temp;
	}

	//左右交换
	bitset<32> tem = L;
	L = R;
	R = tem;

	//左右拼接
	string outs = L.to_string() + R.to_string();
	bitset<64> t(outs);
	out = t;

	return out;
}

void reverse_key(bitset<48>* keys) { //反转密钥
	bitset<48> key1;
	for (int i = 0; i < 8; i++) {
		key1 = keys[i];
		keys[i] = keys[15 - i];
		keys[15 - i] = key1;
	}
}


void shift_left(bitset<28>& k,int round) {
	string k_s = k.to_string();
	//将最左侧的loopTable[round]（要移动的位数）位放到最后，然后取最右侧28位即可
	k_s = (k_s + k_s.substr(0, loopTable[round])).substr(loopTable[round], 28);
	bitset<28> tem(k_s);
	k = tem;
}


void CDesOperate::MakeKey(){
	//置换选择1
	bitset<56> tem;
	for (int i = 0; i < 56; i++) {
		tem[56-1-i] = key[64-1-(pc1Table[i]-1)];
	}
	
	bitset<56> round_input = tem;
	for (int i = 0; i < 16; i++) {//16轮
		//拆分为左右两部分:R , L
		string L_s = round_input.to_string().substr(0, 28); //提取高28位，转为字符串
		bitset<28> L(L_s); //以字符串来初始化左半部分
		//右半部分同理
		string R_s = round_input.to_string().substr(28, 28); //提取低28位，转为字符串
		bitset<28> R(R_s); //以字符串来初始化右半部分
		
		//左循环移位
		shift_left(L,i);
		shift_left(R,i);

		//将结果作为下一轮的输入
		string round_input_s = L.to_string() + R.to_string();
		bitset<56> temp(round_input_s);
		round_input = temp;
		//此时，L、R拼接产生的结果存在round_input中

		//置换选择2
		for (int j = 0; j < 48; j++) {
			keys[i][48-1-j] = round_input[56-1-(pc2Table[j] - 1)];
		}

	}
}


void CDesOperate::HandleData(ULONG32* left , bool choice){
    if (choice==DESENCRY) {//加密

        //接受64bit明文
        bitset<64> txt=ptr_to_bits64((char*)left);

        MakeKey();

        //初始置换
        init_map(txt);

        //轮结构与左右交换
        bitset<64> out = rounds(txt, keys);

        //逆初始置换
        bitset<64> tem;
        for (int i = 0; i < 64; i++) {
            tem[64-1-i] = out[64-1-(ipReverseTable[i] - 1)];
        }
        out = tem;
        
        bits64_to_ptr(out,(char*)left);
	}
    else if(choice==DESDECRY) {//解密

        //接受64bit密文
        bitset<64> txt=ptr_to_bits64((char*)left);

        MakeKey();

        //反转密钥顺序
        reverse_key(keys);

        //初始置换
        init_map(txt);

        //轮结构与左右交换
        bitset<64> out = rounds(txt, keys);

        //逆初始置换
        bitset<64> tem;
        for (int i = 0; i < 64; i++) {
            tem[64-1-i] = out[64-1-(ipReverseTable[i] - 1)];
        }
        out = tem;
        bits64_to_ptr(out,(char*)left);
    }
}





void CDesOperate::Encry(char* pPlaintext,  int nPlaintextLength,  char *pCipherBuffer, 
    int &nCipherBufferLength,  char *pKey,int nKeyLength){
    if(nKeyLength != 8)
	{
		return ;
	}

	key=ptr_to_bits64(pKey);

	int nLenthofLong = ((nPlaintextLength+7)/8)*2;  //number of ULONG32 (group by 64bits)
	if(nCipherBufferLength<nLenthofLong*4)  // number of bytes we need
	{//output buffer is not enough
		nCipherBufferLength=nLenthofLong*4;
		return ;
	}
    
	memset(pCipherBuffer,0,nCipherBufferLength);
    
	ULONG32 *pOutPutSpace = (ULONG32 *)pCipherBuffer;
	ULONG32 * pSource;
	if(nPlaintextLength != sizeof(ULONG32)*nLenthofLong)
	{
		pSource= new ULONG32[nLenthofLong];
		memset(pSource,0,sizeof(ULONG32)*nLenthofLong);
		memcpy(pSource,pPlaintext,nPlaintextLength);
	}
	else{
		pSource= (ULONG32 *)pPlaintext;
	}

	//encry
	ULONG32 gp_msg[2] = {0,0};
	for (int i=0;i<(nLenthofLong/2);i++)
	{
		gp_msg[0] = pSource [2*i];
		gp_msg[1] = pSource [2*i+1];
		HandleData(gp_msg,DESENCRY);
		pOutPutSpace[2*i] = gp_msg[0];
		pOutPutSpace[2*i+1] = gp_msg[1];
	}
	if(pPlaintext!=(char *) pSource)
	{
		delete []pSource;
	}

	return ;    
}


void CDesOperate::Decry(char* pCipher,  int nCipherBufferLength,  char *pPlaintextBuffer, 
    int &nPlaintextBufferLength,  char *pKey,int nKeyLength){
    if(nKeyLength != 8)
	{
		return ;
	}
	key=ptr_to_bits64(pKey);

	if(nPlaintextBufferLength<nCipherBufferLength)  // number of bytes we need
	{//output buffer is not enough
		nPlaintextBufferLength = nCipherBufferLength;
		return ;
	}
	memset(pPlaintextBuffer,0,nPlaintextBufferLength);
	ULONG32 *pOutPutSpace = (ULONG32 *)pPlaintextBuffer;
	ULONG32 * pSource=(ULONG32*)pCipher;

	//decry
	ULONG32 gp_msg[2] = {0,0};
	for (int i=0;i<(nCipherBufferLength/8);i++)
	{
		gp_msg[0] = pSource [2*i];
		gp_msg[1] = pSource [2*i+1];
		HandleData(gp_msg,DESDECRY);
		pOutPutSpace[2*i] = gp_msg[0];
		pOutPutSpace[2*i+1] = gp_msg[1];
	}
	
	return ; 
}


/*
int main(){
    char s[8]="abc";

    bitset<64> bits;

    bits=ptr_to_bits64(s);

    cout<<bits.to_string()<<endl;

    bits64_to_ptr(bits,s);

    cout<<s<<endl;
}
*/