#include <iostream>
#include <MyTools/Character.h>
#include "Snake.h"
#include <fcntl.h>
#include <io.h>

int main()
{
	_setmode(_fileno(stdout), _O_U8TEXT);

	CSnake Snake;
	Snake.Run(15, 15, CSnake::em_Snake_Difficulty::em_Snake_Difficulty_Easy);

	::Sleep(INFINITE);
	return 0;
}