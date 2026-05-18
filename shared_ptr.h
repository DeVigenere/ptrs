#pragma once
#include "control_block.h"
#include <stdexcept>

template<typename T>
class weak_ptr;

template <typename T> 
class shared_ptr { //template class
	control_block<T>* sp;

	void deleting() { //for correct deletion objects. Used in destructor
		if (sp) { 
			sp->shared_counter--;
			if (sp->shared_counter == 0) { 
				sp->destroy();
				if (sp->weak_counter == 0) {
					delete sp;
					sp = nullptr;
				}
			}
			
		}

	}

	void swap(shared_ptr& other) noexcept { //swap function for shared_ptr type object
		std::swap(sp, other.sp);
	}
public:
	shared_ptr() : sp(nullptr) {} //default contructor

	explicit shared_ptr(T* ptr) : sp(nullptr) { //contructor by raw pointer
		if (ptr) {
			sp = new control_block<T>(ptr);
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

	~shared_ptr() noexcept {
		deleting();
	}

	shared_ptr& operator=(const shared_ptr& other) {  //overloaded operator assignments by shared_ptr object
		if (this != &other) { 
			shared_ptr(other).swap(*this);
		}
		return *this;
	}

	shared_ptr& operator=(shared_ptr&& other) noexcept { //overloaded operator assignments by move-semantics
		if (this != &other) {
			shared_ptr(std::move(other)).swap(*this);
		}
		return *this;
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
				sp = new control_block<T>(ptr);
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