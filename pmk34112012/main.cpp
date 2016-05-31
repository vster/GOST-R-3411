#include <iostream>
#include <fstream>
#include <iomanip> 
#include <string>
#include "main.h"

using namespace std;

typedef unsigned char byte; // Объявляем тип byte = unsigned char

const int MAXFN = 20;
const int MAXT = 1000;

void self_testing()
{
	unsigned char h512[64], h256[32];
	int i=0, j;

	for(i=0;i<TEST_VECTORS;i++)
	{
		hash_512(Message[i],MessageLength[i],h512);

		cout << "Text for hashing = ";
		for(j=0; j<MessageLength[i]/8; j++) 
			cout << hex << setw(2) << setfill('0') 
				 << (unsigned int) Message[i][j] << " "; 
		cout << "\n\n";
		
		cout << "Hash 512 = ";
		for(j=0; j<64; j++)
			cout << hex << setw(2) << setfill('0') 
				 <<(unsigned int) h512[j] << " "; 
		cout << "\n\n";
				

		if(memcmp(h512,Hash_512[i],sizeof(unsigned char)*64))
		{
			cout << "  SelfTesting: Fail\n";
			cout << "    Version: 512\n";
			cout << "    Message: " << i << endl;
			cout << "    Length: " << MessageLength[i] << endl;
			return;
		}

		hash_256(Message[i],MessageLength[i],h256);

		cout << "Hash 256 = ";
		for(j=0; j<32; j++)
			cout << hex << setw(2) << setfill('0')
				<< (unsigned int) h256[j] << " "; 
		cout << "\n\n";

		if(memcmp(h256,Hash_256[i],sizeof(unsigned char)*32))
		{
			cout << "  SelfTesting: Fail\n";
			cout << "    Version: 256\n";
			cout << "    Message: " << i << endl;
			return;
		}
	}
	cout << "  SelfTesting: Pass\n\n";
}

// It is not a cryptographic secure pseudorandom number generator
// 1 < bits < 64
unsigned long long get_random_number(unsigned long long bits)
{
	srand ((unsigned int)time(NULL));
	//  srand(__rdtsc());

    return ( (unsigned long long) ( (((unsigned long long)1<<bits)-(bits == 64 ? 2:1))  * ((long double)rand()/(long double)RAND_MAX) ) ) & ( ((unsigned long long)1<<bits) - (bits == 64 ? 2:1));
}
// Form filename from number
void FnameFromNum (char filename[], int num)		
{
	char fext[4];
	char fname[MAXFN] = "test."; 
	sprintf_s(fext,"%03x",num);
	strcat_s(fname,fext);
	strcpy_s(filename, 20, fname);
}
// Make Big-Endian number in buffer from int
void BEFromInt(byte buf[], int num)
{
	for (int i = 3; i>=0; i--)
	{
		buf[i] = num&0xFF;
		num >>= 8;
	}
}
// Перевернуть строку и скоприровать в другую
void ReverseStr( byte str_rev[MAXT], byte str[MAXT], int len )
{
	for ( int i = 0; i < len; i++ )
		str_rev[i] = str[len-1-i];
}

void make_one_test ( char fname[MAXFN], int i, int mode )
{
	byte h512[64], h256[32];
	byte h512_le[64], h256_le[32];
	byte TextToHash_le[MAXT];
	byte buf4b[4];
	byte s0[1] = {0x0};
	int len;

	ofstream testfile;
	testfile.open( fname, ios::binary );
	// testfile.write( reinterpret_cast<char*>(s0), 1);	
	
	// Output to file: hash-code
	if ( mode == 256 )
	{
		hash_256(TextToHashArray[i],MessageLength2[i],h256);		
		ReverseStr( h256_le, h256, 32 );
		testfile.write ( reinterpret_cast<char*>(h256_le), 32 );
	}
	else if ( mode == 512 )
	{
		hash_512(TextToHashArray[i],MessageLength2[i],h512);		
		ReverseStr( h512_le, h512, 64 );
		testfile.write ( reinterpret_cast<char*>(h512_le), 64 );
	}

	len = (int)MessageLength2[i]/8;
	BEFromInt( buf4b, len);
	testfile.write ( reinterpret_cast<char*>(buf4b), sizeof(int) );

	// Output to file: hashed data
	ReverseStr( TextToHash_le, TextToHashArray[i], len );
	testfile.write ( reinterpret_cast<char*>(TextToHash_le), len);
	testfile.close();
}

int main(int argc, char* argv[])
{
	int testnum;
	int mode;
	char fname[MAXFN];
	int i;

	if (argc == 1)
	{
	    std::cout << "Usage: " << argv[0] << " mode\n"
						"mode:    256, 512\n";                
		return 1;
	}

	mode = atoi(argv[1]);
	if ( (mode != 256) && (mode != 512))
		exit(1);
	
	cout << "Generation of datafit files for GOST R 34.11-2012 mode "
		 << mode << " bit..." << endl;

	ofstream logfile;

	logfile.open("pmkinfo.txt");

	logfile << "Контрольные тесты ГОСТ Р 34.11-2012, режим " << mode << " бит.\n";
	logfile << "Сформированы с помощью программной модели криптоалгоритма.\n\n";
	logfile << "Формат:\n";
	if ( mode == 256 )
		logfile << "  32 байта - значение хеш-функции;\n";
	else if ( mode = 512 )
		logfile << "  64 байта - значение хеш-функции;\n";
	logfile << "  4 байта - размер исходных данных;\n";
	logfile << "  data_length - исходные данные.\n\n";

	logfile << "Список тестов:\n";
	for( i = 0, testnum = 1; i < TEST_NUM ;i++ )
	{
		FnameFromNum ( fname, testnum );		

		logfile << fname << " - " << testnum
				<< ", " << (int)MessageLength2[i]/8
				<< endl;

		make_one_test ( fname, i, mode);
		testnum++;
	}
	logfile.close();
	cout << "OK." << endl;
	return 0;
}