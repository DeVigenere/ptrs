#pragma once
#include "ControlBlock.h"
#include <stdexcept>

template<typename T>
class weak_ptr;

template <typename T> 
class shared_ptr { //template class
	ControlBlock<T>* sp;

	void deleting() { //for correct deletion objects. Used in destructor
		if (sp) { 
			sp->shared_counter--;
			if (sp->shared_counter == 0) { 
				if (sp->weak_counter == 0) {
					delete sp;
				}
			}
		}

	}
public:
	shared_ptr() : sp(nullptr) {} //default contructor

	explicit shared_ptr(T* ptr)  { //contructor by raw pointer
		if (ptr) {
			sp = new ControlBlock<T>(ptr);
		}
	}

	shared_ptr(const shared_ptr& other) : sp(other.sp){ //constructor copy
		if (sp) {
			sp->shared_counter++;
		}
	}
	
	shared_ptr(shared_ptr&& other) noexcept : sp(other.sp) { //constructor copy by move-semantics
		other.sp = nullptr;
	}

	shared_ptr& operator=(const shared_ptr& other) {  //overloaded operator assignments by shared_ptr object
		if (this != &other) { 
			deleting(); 
			sp = other.sp;
			if (sp) {
				sp->shared_counter++;
			}
		}
		return *this;
	}

	shared_ptr& operator=(shared_ptr&& other) noexcept { //overloaded operator assignments by move-semantics
		if (this != &other) {
			deleting();
			sp = other.sp;
			other.sp = nullptr;
		}
		return *this;
	}

	~shared_ptr() noexcept { 
		deleting();
	}

	T& operator*() const {  //operator dereferences
		if (!sp) throw std::runtime_error("empty ptr"); 
		return *(sp->ptr);
	}

	explicit operator bool() const noexcept{ //done... operator bool
		return sp != nullptr && sp->shared_counter > 0;
	}

	T* operator->() const noexcept { //done... operator access
		return sp ? sp->ptr : nullptr;
	}

	void reset() noexcept { //function if we dont use another pointer
		deleting();
		sp = nullptr;
	}
	
	void reset(T* ptr) { //function if we using
		deleting();
		sp = nullptr;
		if (ptr) {
			try {
				sp = new ControlBlock<T>(ptr);
			}
			catch (...) {
				delete ptr;
				throw;
			}
		
		}
	}

	T* get() const noexcept { // done
		return sp ? sp->ptr : nullptr;
	}

	int use_count() const noexcept{ // done
		return sp ? sp->shared_counter : 0; 
	}


	friend class weak_ptr<T>; //see private fields
};