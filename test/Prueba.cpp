#include <iostream>
#include <string>
using namespace std;

int main()
{
  string str1, str2;
  
  getline(cin,str1);
  getline(cin,str2);

  int a = stoi(str1);
  int b = stoi(str2);

  cout << "La suma de a + b es " << a + b << endl;

  return 0;
}
