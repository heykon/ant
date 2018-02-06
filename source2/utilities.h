#define _WIN32_WINNT 0x502

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <unistd.h>



#define ONE_INT sizeof(int)
#define B_SIZE_INT 50
#define B_SIZE B_SIZE_INT*ONE_INT

#define HEIGHT 600
#define WIDTH 800

#define FRAME_INTERVAL 2



int char_b_to_int(char* b, int pos)
{
	
	return ((unsigned char)b[pos*ONE_INT]) << 24 |
		((unsigned char)b[pos*ONE_INT + 1]) << 16 |
		((unsigned char)b[pos*ONE_INT + 2]) << 8 |
		(unsigned char)b[pos*ONE_INT + 3];
}


void int_to_char_b(char* b, int pos, int v)
{
	b[pos*ONE_INT] = (char)(v >> 24);
	b[pos*ONE_INT + 1] = (char)(v >> 16);
	b[pos*ONE_INT + 2] = (char)(v >> 8);
	b[pos*ONE_INT + 3] = (char)v;

}