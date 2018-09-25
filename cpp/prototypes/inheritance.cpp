#include <iostream>

class Human{
public:
	Human() {
		std::cout << "Human constructor" << "\n";
	};
	~Human() {
		std::cout << "Human destrucor" << "\n";
	};

	int virtual f() {
		return 1;
	}

};

class Fysiker : public Human{
public:
	Fysiker() {
		std::cout << "Fysiker constructor" << "\n";
	};
	~Fysiker() {
		std::cout << "Fysiker destrucor" << "\n";
	};

	int f() override {
		return 2;
	}

};

int main() {
	Fysiker f;
	Human* h = &f;
	std::cout << h->f() << "\n";
}
