#include <iostream>

int main() {
	for (auto& length : {4, 8, 16, 32, 64, 128}) {
		std::cout << length << "\n";
	}
}
