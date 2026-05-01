#include <iostream>
#include <chrono>

#include "Timer.h"
#include "LinearAllocator.h"

class TestClass
{
private:
	int value =0;
	bool shouldPrint = false;
public:
	TestClass(int n, bool shouldPrint)
	{
		value = n;
		this->shouldPrint = shouldPrint;
	}

	void CalculateFactorial() const
	{
		long long factorial = 1;

		for (int i = 1; i <= value; i++)
		{
			factorial *= i;
		}

		if (shouldPrint)
		{
			std::cout << "Factorial of " << value << " is: " << factorial << std::endl;
		}
	}
};

static void DefaultAllocatorExample()
{
	LinearAllocator allocator(1024 * 1024 * 100); // 100 MB

	{
		Timer timer;

		for (int i = 0; i < 100000; i++)
		{
			TestClass* test = allocator.make<TestClass>(10, false);
			test->CalculateFactorial();
		}

		allocator.reset();
	}
	
	{
		Timer timer;
		for (int i = 0; i < 100000; i++)
		{
			TestClass* test = new TestClass(10, false);
			test->CalculateFactorial();
			delete test;
		}
	}
}

int main()
{
	DefaultAllocatorExample();
}