#include "hash_table.h"
#include <iostream>

HashTable::HashTable() {	
	array = new AuthData[mem_size];
	mem_size = 8;
	count = 0;
	countDel = 0;
}

HashTable::~HashTable() {
	delete[] array;
}

void HashTable::reg(Login _login, char _pass[], int pass_length) {	
	// Регистрация нового пользователя и хеширование его пароля
	uint* hashP = sha1(_pass, pass_length);
	add(_login, hashP);
}

bool HashTable::login(Login _login, char _pass[], int pass_length) {
	// Возвращаем true в случае успешного логина

	for (int i = 0; i < count; ++i)
	{
		if (strcmp(_login, array[i].login) == 0)
		{
			uint* hashPass = sha1(_pass, pass_length);
			if (memcmp(hashPass, array[i].pass_sha1_hash, SHA1HASHLENGTHBYTES) == 0)
			{
				return true;
			}
		}
	}
	return false;
}

void HashTable::add(Login _login, uint* _pass_sha1_hash) {
	
	int index = -1, i = 0;
	
	for (; i < mem_size; i++) {					// берем пробы по всем i от 0 до размера массива
		index = hash_func(_login, i);
		if (array[index].status == enAuthDataStatus::free) {			
			break;								// найдена пустая ячейка, занимаем ее
		}
	}

	if (i >= mem_size) {						// все перебрали, нет места
		resize();
		add(_login, _pass_sha1_hash);
		return;
	}
	else {										// кладем в свободную ячейку пару
		array[index] = AuthData(_login, _pass_sha1_hash);
		std::cout << "add: " << index << "  " << array[index].login << "  " << *array[index].pass_sha1_hash << std::endl;
		count++;
	}

}

void HashTable::del(Login _login) {
	int index = -1, i = 0;
	// берем пробы по всем i от 0 до размера массива
	for (; i < mem_size; i++) {
		index = hash_func(_login, i);
		if (array[index].status == enAuthDataStatus::engaged &&
			!strcmp(array[index].login, _login)) {
			std::cout << "del: " << array[index].login << "  " << *array[index].pass_sha1_hash << std::endl;
			
			array[index].status = enAuthDataStatus::deleted;
			count--;

			countDel++;
			if (countDel >= int(mem_size * 0.4))		// Если удаленных больше 40%
				resize();

			return;
		}
		else if (array[index].status == enAuthDataStatus::free) {
			return;
		}
	}
}

int HashTable::find(Login _login) {

	for (int i = 0; i < mem_size; i++) {
		int index = hash_func(_login, i);
		if (array[index].status == enAuthDataStatus::engaged &&
			!strcmp(array[index].login, _login)) {
			std::cout << "find: " << array[index].login << "  " << *array[index].pass_sha1_hash << std::endl;
			return *array[index].pass_sha1_hash;
		}
		else if (array[index].status == enAuthDataStatus::free) {
			return -1;
		}
	}
	return -1;
}

void HashTable::print() {						// Печать всего массива array
	std::cout << "Array (status 1 - engaged, satus 2 - deleted)" << std::endl;
	for (int i = 0; i < mem_size; i++) {
		if (array[i].status != enAuthDataStatus::free) {
			std::cout << "array[ " << i << "]:\tlogin - " << array[i].login << 
				"\t\thash pass - " << *array[i].pass_sha1_hash << 
				"\t\tstatus - " << array[i].status << std::endl;
		}
	}
	std::cout << std::endl;
}

void HashTable::resize() {

	std::cout << "resize: " << std::endl;
	AuthData* saveArr = array;					// запоминаем старый массив
	int oldSize = mem_size;

	mem_size *= 2;								// увеличиваем размер в два раза  
	count = 0;									// обнуляем количество элементов
	array = new AuthData[mem_size];				// выделяем новую память

	for (int i = 0; i < mem_size; i++) {
		memcpy(array[i].login, "", LOGINLENGTH);
		array[i].pass_sha1_hash = 0;
		array[i].status = enAuthDataStatus::free;
	}
	int ind = -1;
	for (int i = 0; i < oldSize; i++) {
		if (saveArr[i].status == enAuthDataStatus::engaged)		
			add(saveArr[i].login, saveArr[i].pass_sha1_hash);
	}
	
	delete[] saveArr;							// чистим за собой
	std::cout << "resize end." << std::endl;
}

int HashTable::hash_func(Login _login, int offset) {
	
	int sum = 0, i = 0;
	for (; i < strlen(_login); i++) {			// вычисляем индекс
		sum += _login[i];
	}
	const double A = 0.7;
	const int M = 11;
	// ХФ составлена  методом умножения + квадратичные пробы
	return int((A * sum - int(A * sum)) + offset * offset) % mem_size;
}