#include<iostream>
#include<fstream>
using namespace std;

struct tree{
	int tr[4];
};
int main()
{
	struct tree t[3];
	for (int i=0;i<3;i++)
	{
		for(int j=0;j<4;j++)
		{
			cin>>t[i].tr[j];
		}
	}

	cout<<"--------\n";
	ofstream f;
	f.open("1.txt");
	
	for (int i=0;i<3;i++)
	{
		for(int j=0; j<4;j++)
		{
			f<<t[i].tr[j];
		}
		f<<"\n";
	}
	f.close();
}