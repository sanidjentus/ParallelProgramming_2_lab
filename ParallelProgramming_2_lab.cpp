#include <iostream>
#include <Windows.h>
#include <thread>
#include <ctime>

const size_t ARRAY_SIZE = 100; //размер массива
const size_t THREADS = 4; //кол-во потоков
const int MAX_VALUE = 101; //максимальное значение из элементов массива, т.к. rand() задаем в пределах 100

using namespace std;

//заполняем массив случайными числами 
void initRandomArray(int* arr) {
	for (size_t i = 0; i < ARRAY_SIZE; i++)
		arr[i] = rand() % 100;
	arr[ARRAY_SIZE - 1] = -100; //мин четное число
}

//ф-я поиска минимального из четных элементов

void findMinEven(int* arr, size_t left, size_t right, int& result) {
	result = 1;
	for (size_t i = left; i < right; i++) {
		if (arr[i] % 2 == 0 && (result == 1 || arr[i] < result)) {
			result = arr[i];
		}
	}
}

//печать массива
/*
void printArray(int* arr) {
	for (int i = 0; i < 100; i++)
		cout << i << " - " << arr[i] << endl;
}
*/

//ф-я разделения на потоки 
int findMinEvenParallel(int* arr) {
	thread t[THREADS]; // создаем 4 потока
	int minEvenThread[THREADS]; // минимальный четный элемент из каждого потока 
	size_t n = ARRAY_SIZE / THREADS; // размер потоков
	//cout << n << endl;
	for (size_t i = 0; i < THREADS; i++) {
		if (i == THREADS - 1)
			t[i] = thread(findMinEven, arr, n * i, ARRAY_SIZE, ref(minEvenThread[i])); 
		else
			t[i] = thread(findMinEven, arr, n * i, n * (i + 1), ref(minEvenThread[i]));
	}

	for (size_t i = 0; i < THREADS; i++)
		t[i].join();

	int globalMinEven = 0; //четное минимальное число из всех потоков
	findMinEven(minEvenThread, 0, THREADS, globalMinEven);
	return globalMinEven;
}

int main() {
	int arr[ARRAY_SIZE];
	srand(GetTickCount64());
	initRandomArray(arr);
	//printArray(arr);

	int minEvenNotParallel = 0; //результат выполнения ф-ии findMinEven
	findMinEven(arr, 0, ARRAY_SIZE, minEvenNotParallel);
	
	cout << "Min even number without parallel: " << minEvenNotParallel <<  endl;
	cout << "Min even number with parallel: " << findMinEvenParallel(arr) << endl;
	return 0;
}