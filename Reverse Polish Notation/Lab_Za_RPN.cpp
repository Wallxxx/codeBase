#include "pch.h"
#include "reversePN.h" // Заголовочный файл с нашим классом. 

int main()
{
	ReversePolishNotation<double> testDouble;
	string str;
	getline(cin, str);
	cout << "Answer: " << testDouble.calc(str) << endl;
	testDouble.~ReversePolishNotation();

	/*
	ReversePolishNotation<double> testDouble;
	string str = "((3.5 + 55.76) * (4 - 3)) / (77 - 3.33)";
	cout << "Test 1 (double): " << testDouble.calc(str) << " (correct answer = 0,804398)" << endl;
	str = "5 * p - e ^ 2 * (4 - 2.35)";
	cout << "Test 2 (double): " << testDouble.calc(str) << " (correct answer = 3,51602)" << endl;
	str = "64367.34217773 / ((63.7731 + 3.00043) * (0.2455 - 0.00001))";
	cout << "Test 3 (double): " << testDouble.calc(str) << " (correct answer = 3926,6974)" << endl;
	str = "(352.664 + 236) * p";
	cout << "Test 4 (double): " << testDouble.calc(str) << " (correct answer = 1849,3424978)" << endl;
	str = "(143.3 * 5.25) / (64367.34217773 / ((63.7731 + 3.00043) * (0.2455 - 0.00001)))";
	cout << "Test 5 (double): " << testDouble.calc(str) << " (correct answer = 0,1915923034)" << endl << endl;
	testDouble.~ReversePolishNotation();

	ReversePolishNotation<float> testFloat;
	str = "(13.3 * 5.25) / (647.34217773 / ((63.7731 + 3.00043) * (0.245 - 0.034001)))";
	cout << "Test 6 (float): " << testFloat.calc(str) << " (correct answer = 1,519713679)" << endl;
	str = "64.63 / 0.0001 * 3 ^ 9";
	cout << "Test 7 (float+): " << testFloat.calc(str) << " (correct answer = 12721122900 or 1,27211229e10)" << endl;
	str = "p ^ e";
	cout << "Test 8 (float): " << testFloat.calc(str) << " (correct answer = 22,4591577183)" << endl;
	str = "(4 * (3 + 9 / (5 - 2.46)))";
	cout << "Test 9 (float): " << testFloat.calc(str) << " (correct answer = 26,1732283464566929133858)" << endl;
	str = "3.52 / 2.52 ^ (4.2134 - 4.1423)";
	cout << "Test 10 (float): " << testFloat.calc(str) << " (correct answer = 3,2961205358180620)" << endl << endl;
	testFloat.~ReversePolishNotation();

	ReversePolishNotation<int> testInt;
	str = "3 / 2 ^ (4 - 4)";
	cout << "Test 11 (int): " << testInt.calc(str) << " (correct answer = 1,519713679)" << endl;
	str = "4 * ((2 ^ 21) / 32) * (0 + 7) ^ (0 - 121 + 128)";
	cout << "Test 12 (int+): " << testInt.calc(str) << " (correct answer = 215886856192 :: not enough space)" << endl;
	str = "6 + (24 - 64 / (24 + 1))";
	cout << "Test 13 (int): " << testInt.calc(str) << " (correct answer = 27,44)" << endl;
	str = "0 - 42 * 5";
	cout << "Test 14 (int): " << testInt.calc(str) << " (correct answer = -210)" << endl;
	str = "2 ^ 2 ^ 2 ^ 2";
	cout << "Test 15 (int): " << testInt.calc(str) << " (correct answer = 256)" << endl << endl;

	ReversePolishNotation<long> testLong; 
	str = "4 * ((2 ^ 21) / 32) * (0 + 7) ^ (0 - 121 + 128)";
	cout << "Test 16 (long+): " << testLong.calc(str) << " (correct answer = 215886856192 :: not enough space)" << endl;
	str = "5 ^ 8 / 2 ^ 12";
	cout << "Test 17 (long): " << testLong.calc(str) << " (correct answer = 95,36743)" << endl;
	str = "p * (e + 4)";
	cout << "Test 18 (long): " << testLong.calc(str) << " (correct answer = 21,106104837032 :: pi = 3, exp = 2 (for int & long))" << endl;
	str = "325 - 6452 + 74 + (325 / 24)";
	cout << "Test 19 (long): " << testLong.calc(str) << " (correct answer = -6039,45833)" << endl;
	str = "32 - 63 + 2";
	cout << "Test 20 (long): " << testLong.calc(str) << " (correct answer = -29)" << endl << endl;

	cout << "(+) - tasks of increased complexity" << endl << endl; */
	return 0;
}
