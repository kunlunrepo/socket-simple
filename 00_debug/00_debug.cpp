#include <iostream>

// 第一种：通过输出信息
void debug1()
{
	int a = 0;
	for (int i = 1; i <= 10; i++) {
		a = a + i;
		if (i == 5)
			printf("debug: i==5, a=%d\n", a); // 输出信息
	}
	printf("a=%d \n", a);
}

// 第二种：通过debug宏
void debug2()
{
	int a = 0;
	for (int i = 1; i <= 10; i++) {
		a = a + i;
#ifdef DEBUG // DEBUG宏 内部的代码在发布release版时不会含有
		if (i == 5)
			printf("debug: i==5, a=%d\n", a); // 输出信息
#endif
	}
	printf("a=%d \n", a);
}

// 第三种：通过断点
void debug3()
{
	char str[] = "Haha";
	int arr[] = { 1,3,5,7,9 };
	int* pArr = arr;
	int a = 0;
	int* pA = &a;
	float f = 1.25f;
	for (int i = 1; i <= 10; i++) { // 多个变量可添加监视同时观察
		a = a + i;
	}
	printf("a=%d \n", a);
}

void funA(int n, int* pArr, int len)
{
	for (int i = 0; i < len; i++)
	{
		pArr[i] *= n;
	}
}

int main() {
	//debug1();
	//debug2();
	//debug3();
	int arr[] = { 1,3,5,7,9 };
	funA(2, arr, 5);
}

