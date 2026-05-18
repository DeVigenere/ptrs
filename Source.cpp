#include <iostream>
#include "ControlBlock.h"
#include "shared_ptr.h"
#include "weak_ptr.h"

int main() {
	shared_ptr<int> one(new int (42));
	shared_ptr<int> two(one);
	shared_ptr<int> three = one;
	shared_ptr<int> four(std::move(one));
	shared_ptr<int> five = std::move(two);
	

	return 0;
}