#pragma once
#include "ControlBlock.h"
#include <algorithm>

template<typename T>
class shared_ptr;

template<typename T>
class weak_ptr { //template class
	ControlBlock<T>* wp; 

	void swap(weak_ptr& other) noexcept{ //swap function for weak_ptr type object
		std::swap(wp, other.wp);
	}
public:
	weak_ptr() : wp(nullptr) {} //default constructor

	weak_ptr(const shared_ptr<T>& shared) : wp(shared.sp) { //constructor by shared_ptr type object
		if (wp) {
			wp->weak_counter++;
		}

	}
	~weak_ptr() noexcept { //destructor (noexcept for clarity)
		if (wp) {
			wp->weak_counter--;
			if (wp->shared_counter	== 0
			&&	wp->weak_counter	== 0) { 
				delete wp;
			}
		}
	}
	weak_ptr(const weak_ptr& ptr) : wp(ptr.wp) {  //constructor by weak_ptr type object
		if (wp) {
			wp->weak_counter++;
		}
	}

	weak_ptr(weak_ptr&& other) noexcept : wp(other.wp) { //done... constructor move-semantics
		other.wp = nullptr;
	}

	weak_ptr& operator=(const shared_ptr<T>& shared) { //overloaded operator assignments by shared_ptr type object
		weak_ptr temp(shared);
		swap(temp); //done
		return *this;
	}

	weak_ptr& operator=(const weak_ptr& weak) { //overloaded operator assingments by weak_ptr type object
		if (this != &weak) {
			weak_ptr temp(weak);
			swap(temp);//done
		}
		return *this;
	}

	weak_ptr& operator=(weak_ptr&& other) noexcept { //done... overloaded operator assingments for move-semantics
		if (this != &other) {
			if (wp) {
				wp->weak_counter--;
				if (wp->shared_counter == 0 && wp->weak_counter == 0) {
					delete wp;
				}
			}
			wp = other.wp;
			other.wp = nullptr;
		}
		return *this;
		
	}

	bool expired() const noexcept { //expired or not function
		return wp == nullptr || wp->shared_counter == 0;
	}

	shared_ptr<T> lock() const noexcept { //lock function
		if (expired()) {
			return shared_ptr<T>();
		}
		shared_ptr<T> result;
		result.sp = wp;
		if (result.sp->shared_counter) {
			result.sp->shared_counter++;
		}
		return result;
	}

	int use_count() const noexcept{ //done 
		return wp ? wp->shared_counter : 0;
	}

	T* get() const noexcept { //done
		return expired() ? nullptr : wp->ptr;
	}

	void reset() noexcept { //done
		if (wp) {
			wp->weak_counter--;
			if (wp->shared_counter == 0 && wp->weak_counter == 0) {
				delete wp;
			}
		}
		wp = nullptr;
	}



	friend class shared_ptr<T>; //see private fields
};