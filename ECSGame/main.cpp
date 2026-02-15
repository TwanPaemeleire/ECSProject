#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "TestClass.h"
#include <iostream>

int main(int, char* []) 
{
	std::unique_ptr<TestClass> testClass = std::make_unique<TestClass>();
	testClass->TestFunction();
	testClass->TestFunction();
	testClass->TestFunction();
	return 0;
}