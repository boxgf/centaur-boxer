#include<iostream>
using namespace std;
template <class TYPE>
class af
{
	public:
	
	TYPE g;
void	virtual fn()
	{
		
}
	af()
	{g=0;}
};
template <class TYPE>
class dc:public af<TYPE>
{
public:
void fn()
{
	cout<< "\nin virtual\n";
	
	}
	
	};
main()
{
	af<int>  k,*p;
	cout<<k.g;
	dc<int> der;
	p=&der;
	p->fn();
	int a;
	cin>>a;
}