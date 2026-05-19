#pragma once

template<typename T>
struct control_block {
	int shared_counter; //how many shared pointers we had
	int weak_counter; //how many weak pointers we had
	T* ptr; // pointer on the object (raw)
	
	control_block(T* p) : shared_counter(1), weak_counter(0), ptr(p) {};
	
	void destroy() {
		delete ptr;
		ptr = nullptr;
	}

	~control_block() = default;
};