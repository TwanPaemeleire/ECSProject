#include "pch.h"
#include "TestClass.h"
#include <iostream>

void TestClass::TestFunction()
{
	std::cout << "TestClass function called: " << m_TestValue << std::endl;
	++m_TestValue;
}