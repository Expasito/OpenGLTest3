

#include <iostream>


struct cheese {
	int a, b, c;
	cheese() = default;
	cheese(int d, int e, int f) : a(d), b(e), c(f) {};
};

struct apple {
	float x, y;
	apple() = default;
	apple(float z, float a) : x(z), y(a) {};
};

void addInts(int x, int y, int z) {
	std::cout << "int total: " << x + y + z << "\n";
}

void multFloats(int x, int y) {
	std::cout << "float total: " << x * y << "\n";
}
template<typename T>
void addInt(std::initializer_list<T> x) {

	int val[3];
	std::copy(x.begin(), x.end(), std::begin(val));

	addInts(val[0], val[1], val[2]);
}
template<typename T>
void multFloat(std::initializer_list<T> x) {
	//std::cout << "floats works" << "\n";
	int val[2];
	std::copy(x.begin(), x.end(), std::begin(val));
	multFloats(val [0] , val[1]);

}

template<typename T, typename... Args>
static void foo2(Args... args) {
	auto values = { args... };
	/*for (auto v : values) {
		std::cout << v << "\n";
	}*/
	const size_t value = sizeof...(args);
	//std::cout << "size is" << value << "\n";
	//std::cout << typeid(values).name() << "\n";
	auto arr[value];
	//std::copy(args.begin(), args.end(), std::begin(arr));
	if (typeid(T) == typeid(float)) {
		multFloat(values);
	}
	if (typeid(T) == typeid(int)) {
		addInt(values);
	}
	if (typeid(T) == typeid(cheese)) {

	}
	if (typeid(T) == typeid(apple)) {

	}

	//int dummy[] = { 0, ((void)bar(std::forward<Args>(args)), 0) ... };

}

int main() {
	//foo2<int>(1, 2, 3);
	//std::cout << "next example\n";
	//foo2<float>(1.0f, 2.0f);
	//cheese b(1, 2, 3);
	//cheese c();
	foo2<cheese>(1, 2, 3);

	//foo2<float>(1.0f, 2.9f);
	return 0;
}
