// Тесты алгоритма хеширования ГОСТ Р 34.11-94
// Используются узлы замен из процедуры проверок
// самого ГОСТ Р 34.11-94

#include <iostream>
#include <iomanip>
#include "hash.h"

using namespace std;

const int HSB = 32;


/*
// Контрольные примеры для узлов замен из ГОСТ Р 34.11-94
// OID 1.2.643.2.2.30.0
GOST ("This is message, length=32 bytes") = B1C466D37519B82E8319819FF32595E047A28CB6F83EFF1C6916A815A637FFFA
GOST ("Suppose the original message has length = 50 bytes") = 471ABA57A60A770D3A76130635C1FBEA4EF14DE51F78B4AE57DD893B62F55208
GOST ("") = CE85B99CC46752FFFEE35CAB9A7B0278ABB4C2D2055CFF685AF4912C49490F8D
GOST ("a") = D42C539E367C66E9C88A801F6649349C21871B4344C6A573F849FDCE62F314DD
GOST ("abc") = F3134348C44FB1B2A277729E2285EBB5CB5E0F29C975BC753B70497C06A4D51D
GOST ("message digest") = AD4434ECB18F2C99B60CBE59EC3D2469582B65273F48DE72DB2FDE16A4889A4D
GOST ("The quick brown fox jumps over the lazy dog") = 77B7FA410C9AC58A25F49BCA7D0468C9296529315EACA76BD1A10F376D1F4294
GOST (128 символов "U") = 53A3A3ED25180CEF0C1D85A074273E551C25660A87062A52D926A9E8FE5733A4
GOST (1000000 символов "a") = 5C00CCC2734CDD3332D3D4749576E3C1A7DBAF0E7EA74E9FA602413C90A129FA
*/
/*
// Контрольные примеры для  таблицы замен RFC 4357
// OID 1.2.643.2.2.30.1
GOST("This is message, length=32 bytes") = 2CEFC2F7B7BDC514E18EA57FA74FF357E7FA17D652C75F69CB1BE7893EDE48EB
GOST("Suppose the original message has length = 50 bytes") = C3730C5CBCCACF915AC292676F21E8BD4EF75331D9405E5F1A61DC3130A65011
GOST("") = 981E5F3CA30C841487830F84FB433E13AC1101569B9C13584AC483234CD656C0
GOST("a") = E74C52DD282183BF37AF0079C9F78055715A103F17E3133CEFF1AACF2F403011
GOST("abc") = B285056DBF18D7392D7677369524DD14747459ED8143997E163B2986F92FD42C
GOST("message digest") = BC6041DD2AA401EBFA6E9886734174FEBDB4729AA972D60F549AC39B29721BA0
GOST("The quick brown fox jumps over the lazy dog") =  9004294A361A508C586FE53D1F1B02746765E71B765472786E4770D565830A76
GOST(128 символов "U") = 1C4AC7614691BBF427FA2316216BE8F10D92EDFD37CD1027514C1008F649C4E8
GOST(1000000 символов "a") = 8693287AA62F9478F7CB312EC0866B6C4E4A0F11160441E8F4FFCD2715DD554F
*/


static char *TextToHashArray[9] = {
		"012345678901234567890123456789012345678901234567890123456789012",
		"Се ветри, Стрибожи внуци, веютъ с моря стрелами на храбрыя плъкы Игоревы",
		"This is message, length=32 bytes",
		"Suppose the original message has length = 50 bytes",
		"", 
		"a", 
		"abc", 
		"message digest", 
		"The quick brown fox jumps over the lazy dog"
		};

void line()
{
		cout << "\n";
		for (int i=0; i<79; i++) 
			cout << "-";
		cout << "\n";	
}



// Output of text in low-endian
void out_text_le ( char *TextToOut )
{
	cout << "Text to hash (low-endian)= \n" << "\"" 
		 << TextToOut << "\"" << endl << endl;    
}

// Output of text in big-endian
void out_text_be ( char *TextToOut, int len )
{
	int i;
	cout << "\nText to hash (big-endian) = " << endl;
	
	for (i = len-1; i >= 0; i--)
		cout << hex << setw(2) << setfill('0')
			 << (((unsigned int)TextToOut[i]) & 0x00FF) << " ";
	cout << endl << endl;
}

// Output of hash-code in low-endian 
void out_hash_le( byte hashed[HSB] )
{
	int i;
	cout << "Hash-code (low-endian) = \n";
	for( i=0; i<HSB; ++i )
		cout << hex << setfill('0') << setw(2)
			 << (int)hashed[i] << " ";
	cout << endl << endl;
}

// Output of hash-code in big-endian
void out_hash_be( byte hashed[HSB] )
{
	int i;
	cout << "Hash-code (big-endian) = \n";
	for( i=HSB-1; i>=0; i-- )
		cout << hex << setfill('0') << setw(2)
			 << (int)hashed[i] << " ";
	cout << endl << endl;
}

int main() 
{
	// byte *buf;
	int len;
	byte hashed[HSB];
	// int i;
		
	out_sbox();

	//--------------
	line();

	for (int j = 0; j < 9; j++)
	{
 		len = (int)strlen((char*)TextToHashArray[j]);

		hash((byte*)TextToHashArray[j], len, hashed);
    
		cout << "\nTEST " << j+1 << "\n\n";
		cout << "Text length  = " << dec << len << " bytes\n\n";
		
		out_text_le ( TextToHashArray[j] );		
		
		out_hash_le( hashed );

		out_text_be ( TextToHashArray[j], len );

		out_hash_be( hashed );

		//--------------
		line();
	}

	return 0;
}
