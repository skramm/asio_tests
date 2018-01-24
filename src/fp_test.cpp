/**
fp_test.cpp
function pointer test
*/

typedef void (*callback_func)( int, float, char );

struct A
{
//	callback_func _cb;
	void (*_cb)( int, float, char );

/*	void assignCallback( callback_func f )
	{
		_cb = f;
	}*/
};

struct B : public A
{
// this is the true callback function
	void tcb( int a, float b, char c )
	{
		// do lots of things with a, b, c
	}

	B()
	{
//		assignCallback( &B::cb );
//		_cb = &B::tcb;
	}
};

int main()
{
	B b;
}
