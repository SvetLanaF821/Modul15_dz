#pragma once
#include "sha1.h"
#include <string>

#define LOGINLENGTH 10				// Длина имени
#define PASSLENGTH 15				// Длина пароля

typedef char Login[LOGINLENGTH];	// тип логина

class HashTable {					
public:

	HashTable();
	~HashTable();

	void reg(Login _login, char _pass[], int pass_length);
	bool login(Login _login, char _pass[], int pass_length);
	void add(Login _login, uint* _pass_sha1_hash);
	void del(Login _login);
	int find(Login _login);
	void print();
	
private:

	enum enAuthDataStatus {
		free,
		engaged,
		deleted
	};

	struct AuthData {				

		AuthData() : login(""), pass_sha1_hash(0), status(enAuthDataStatus::free) {}

		~AuthData() = default;

		AuthData(Login fr_name, uint* _pass_sha1_hash) :
			pass_sha1_hash(_pass_sha1_hash),
			status(enAuthDataStatus::engaged) {
			strcpy_s(login, fr_name);
		}

		AuthData& operator = (const AuthData& other) {
			pass_sha1_hash = other.pass_sha1_hash;
			strcpy_s(login, other.login);
			status = other.status;
			return *this;
		}

		bool operator == (const AuthData& other) {
			return  status == other.status &&
				(status != enAuthDataStatus::engaged || (pass_sha1_hash == other.pass_sha1_hash && !strcmp(login, other.login)));
		}

		Login login;
		uint* pass_sha1_hash;
		enAuthDataStatus status;
	};

	void resize();
	int hash_func(Login _login, int offset);

	AuthData* array;
	int mem_size;
	int count;
	int countDel;
};
