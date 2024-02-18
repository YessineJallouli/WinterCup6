#include <iostream>
#include <vector>

//Use this function for a asking the LCM of P[i] and P[j]
long long query(int i, int j)
{
	long long response;
	std::cout << '?' << i << ' ' << j << std::endl;
	std::cin >> response;
	return response;
}

//Use this function to print the guessed permutation
void answer(const long long sum)
{
	std::cout << "! " << sum << std::flush;
}


int main()
{
	int n;
	std::cin >> n;
	long long sum=0;
	/*
		Your code here. Feel free to modify any part of this file, or even change it completely.
	*/
}
