/* Program:	Hello World
   Author:	Anthony Lucchese
   Date:	September 3, 2021
   File name:	hello_world.c
   Compile:	cc -o helloworld hello_world.c
   Run:		./helloworld

   This C program acceppts user's first name and a positive
   integer N; it then prints a personal greeting N times.
*/

#include <stdio.h>

int main() {
	char name[20];		// string of chars, <= 19 chars
	int times; 
	int i = 0;

	printf("Please enter your firstname and a number: \n");
	scanf("%s%d", name, &times);

	// print greetings 
	for (i = 0; i < times; i++) {
		printf("Hello %s, your first program works!\n", name);
	}
} 
