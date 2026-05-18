#pragma once
#include "Counter.h"

template<typename T>
class shared_ptr;

template<typename T>
class weak_ptr {
private:
	T* weak_obj;
	Counter* weak_counter;
public:
	weak_ptr() : weak_obj(nullptr), weak_counter(nullptr) {}; // дефолтный конструктор

	weak_ptr(const shared_ptr<T>& shared) {
		weak_obj = shared.shared_obj;
		weak_counter = shared.counter;
		if (weak_counter) { //увеличиваем при создании через shared
			weak_counter->weak_counter++;
		}

	}
	~weak_ptr() noexcept {
		if (weak_counter) {
			weak_counter->weak_counter--;
			if (weak_counter->shared_counter == 0 && weak_counter->weak_counter == 0) { // если оба счётчика по нулям, то только тогда убираем weak
				delete weak_counter;
			}
		}
	}
	weak_ptr(const weak_ptr& ptr) {
		weak_obj = ptr.weak_obj;
		weak_counter = ptr.weak_counter;
		if (weak_counter) { //опять же увеличиваем, если создаем еще
			weak_counter->weak_counter++;
		}
	}

	weak_ptr& operator=(const shared_ptr<T>& shared) {
		if (weak_counter) {
			weak_counter->weak_counter--;
			if (weak_counter->shared_counter == 0 && weak_counter->weak_counter == 0) { //убираем, чтобы утечки не было
				delete weak_counter;
			}
		}
		weak_obj = shared.shared_obj;
		weak_counter = shared.counter;
		if (weak_counter) {
			weak_counter->weak_counter++;
		}
		return *this;
	}

	weak_ptr& operator=(const weak_ptr& weak) {
		if (this != &weak) {
			if (weak_counter) {
				weak_counter->weak_counter--;
				if (weak_counter->shared_counter == 0 && weak_counter->weak_counter == 0) {
					delete weak_counter;
				}
			}
			weak_obj = weak.weak_obj;
			weak_counter = weak.weak_counter;
			if (weak_counter) {
				weak_counter->weak_counter++;
			}
		}
		return *this;
	}

	bool expired() const noexcept {
		return weak_counter == nullptr || weak_counter->shared_counter == 0;
	}

	shared_ptr<T> lock() const noexcept {
		if (expired()) {
			return shared_ptr<T>();
		}
		shared_ptr<T> result;
		result.shared_obj = weak_obj;
		result.counter = weak_counter;
		if (result.counter) {
			result.counter->shared_counter++;
		}
		return result;
	}

	friend class shared_ptr<T>; //видим приватные поля
};