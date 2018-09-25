#include <iostream>

class test{
public:

	int* variable;
	test() {
		std::cout << "Called constructor!" << "\n";
		variable = new int(4);
	};

	~test() {
		delete variable;
		std::cout << "Called destructor!" << "\n";
	};
};

int main() {
	test* t = new test();
	test t2 = test();

	delete t;
}
