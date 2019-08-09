#include <iostream>

using namespace std;

enum season
{
	spring,
	summer,
	fall,
	winter
};
enum class days	//scoped enumeration
{
	monday,
	tuesday,
	wednesday,
	thirsday,
	friday,
	saturday,
	sunday
};


const char* s;			//	s is a pointer to a const char
char c;						
char *const t = &c;		//	t is a constant pointer to address of c

const char b;
const char* const f = &b;	//	you can use this because you are pointing to a "const char" value with a "const" pointer.
// char* const f = &b;		//	you cant use this because your char is "const char" . you need to have a pointer to "const char" not "char".
// const char* a = &b;		//	you can use this because your pointer is a pointer to "const char"


class A
{
public:
	int t;

	A()
	{
		t = 1;
	}

	~A()
	{
		std::cout << "sildim amk" << std::endl;
	}

	// return value is constant. also this function is defined as constant at the end.
	// this means that it cannot change member variables in class.
	int func() const	
	{						
		return t;
	}

	int func2() 
	{
		return t;
	}
};

int main()
{
	// *s = 'a';			//	s is a constant char. you cant change.
	s++;					//	s is not a constant pointer so you can increase it.
	*t = 'a';				//	value in t is not constant so you can change.
	// t++;					//	you cant do this because t is a constant pointer
	//b = 'a';				//	you cant use this because b is constant
	//*f = 'a';				//	you cant use this because the value in constant pointer f is consant.
	//f++;					//	you cant use this because f is a constant pointer.

	const A testClass;		//  if a class is declared as const when making, you cant change its values and can ony call constant functions.
	//testClass.t = 5;
//	testClass.t	= testClass.func();
	
	// testClass.func();	works. func is const. "int func() const"
	// testClass.func2();	wont work. func2 is not const. "int func2()"
	A* testclass2 = new A();
	testclass2->func();

	int* ptr = new int;
	int* ptr1 = new int(4);
	int* ptr2 = new int	{ 4 };

	int length = 5 ;
	char* arr = new char[length];

	season s = spring
	days d = days::sunday

	delete ptr;
	delete testclass2;

}

