#include <iostream>

using namespace std;

int main() {
	for (;;)
	{	
		motor(0, -50); // hinten links - langsam
		motor(1, 100); // vorne links - geht ned
		motor(2, 20); // hinten rechts
		motor(3, 20); // vorne rechts
	}
}
