#include "hash_table.h"

int main()
{
	HashTable ht;

	ht.reg((char*)"user1", (char*)"passUser1", PASSLENGTH);
	ht.reg((char*)"user2", (char*)"passUser2", PASSLENGTH);
	ht.reg((char*)"user3", (char*)"passUser3", PASSLENGTH);
	ht.reg((char*)"user4", (char*)"passUser4", PASSLENGTH);
	ht.reg((char*)"user5", (char*)"passUser5", PASSLENGTH);
	ht.reg((char*)"user6", (char*)"passUser6", PASSLENGTH);
	ht.reg((char*)"user7", (char*)"passUser7", PASSLENGTH);
	ht.reg((char*)"user8", (char*)"passUser8", PASSLENGTH);
	ht.reg((char*)"user9", (char*)"passUser9", PASSLENGTH);

	ht.print();
	ht.find((char*)"user7");
	ht.del((char*)"user9");
	ht.del((char*)"user8");

	ht.print();

	return 0;
}