#pragma once

template<typename T>
struct ControlBlock {
	int shared_counter; //how many shared pointers we had
	int weak_counter; //how many weak pointers we had
	T* ptr; // pointer on the object (raw)
	ControlBlock(T* p) : shared_counter(1), weak_counter(0), ptr(p) {};
	
	~ControlBlock() { //done...
		delete ptr;
	}
};