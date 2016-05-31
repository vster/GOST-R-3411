#include <iostream>
#include <iomanip> 
#include <string>
#include "main.h"

using namespace std;

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

int main()
{
	unsigned char h512[64], h256[32];
	int i, j, len;

	// self_testing();

	for( i = 0; i < TEST_NUM ;i++ )
	{
		hash_512(TextToHashArray[i],MessageLength2[i],h512);

		cout << "TEST " << dec << i+1 << "\n\n";
		
		cout << "Text for hashing (big-endian) = \n";
		for(j = 0; j < MessageLength2[i]/8; j++) 
			cout << hex << setw(2) << setfill('0') 
			<< (unsigned int)TextToHashArray [i][j] << " "; 
		cout << "\n\n";
		
		cout << "Hash 512 (big-endian) = \n";
		for(j=0; j<64; j++) 
			cout << hex << setw(2) << setfill('0')
			<< (unsigned int) h512[j] << " ";
		cout << "\n\n";		

		hash_256(TextToHashArray[i],MessageLength2[i],h256);

		cout << "Hash 256 (big-endian) = \n";
		for(j=0; j<32; j++)
			cout << hex << setw(2) << setfill('0') 
			<< (unsigned int) h256[j] << " "; 
		cout << "\n\n\n";		
		
	}
	return 0;
}