#pragma once
#include "Counter.h"

template<typename T>
class weak_ptr;

template <typename T> // шаблонный класс
class shared_ptr {
	T* shared_obj;
	Counter* counter;

	void deleting() { //метод удаления объектов через подсчёт ссылок
		if (counter) { //если есть значение счётчика отличное от nullptr, то удаляем
			counter->shared_counter--;
			if (counter->shared_counter == 0) { //если равен нулю счётчик, то удаляем. Причём, если предыдущие указатели, которые стучались в этот метод отлетали от этого условия, то последний его пройдёт и удалит всё из кучи
				delete shared_obj;
				shared_obj = nullptr;
				if (counter->weak_counter == 0) {
					delete counter;
					counter = nullptr;
				}
			}
		}

	}
public:
	shared_ptr() : shared_obj(nullptr), counter(nullptr) {}; // базовый конструктор

	explicit shared_ptr(T* ptr) : shared_obj(nullptr), counter(nullptr) {
	
		if (ptr) {
			try {
				counter = new Counter();
				shared_obj = ptr;
				counter->shared_counter++;
			}
			catch (...) {
				delete ptr;
				throw;
			}
		}

	}

	shared_ptr(const shared_ptr& other) { //конструктор копирования
		shared_obj = other.shared_obj;
		counter = other.counter;
		if (counter) {
			counter->shared_counter++;
		}
	}
	shared_ptr& operator=(const shared_ptr& other) { // конструктор присваивания
		if (this != &other) { // чтобы не было one = one
			deleting(); //отвязываем, чтобы утечек не было
			shared_obj = other.shared_obj;
			counter = other.counter;
			if (counter) {
				counter->shared_counter++;
			}
		}
		return *this;
	}

	shared_ptr(shared_ptr&& other) noexcept { //конструктор от move
		shared_obj = other.shared_obj;
		counter = other.counter;
		other.shared_obj = nullptr;
		other.counter = nullptr;
	}

	shared_ptr& operator=(shared_ptr&& other) noexcept { // присваивание от перемещения
		if (this != &other) {
			deleting();
			shared_obj = other.shared_obj;
			counter = other.counter;
			other.shared_obj = nullptr;
			other.counter = nullptr;
		}
		return *this;
	}

	T& operator*() const { //перегрузка оператора разыменовывания
		return *shared_obj;
	}

	~shared_ptr() noexcept { //деструктор (как прочитал из ДЗ можно указывать noexcept, но по умолчанию он и так noexcept)
		deleting();
	}

	friend class weak_ptr<T>; //видим приватные поля
};