#ifndef __SNAKE_WALL_H__
#define __SNAKE_WALL_H__

#include <deque>
#include <MyTools/ToolsPublic.h>

class CWall
{
public:
	enum class em_PrintType
	{
		em_PrintType_Empty,
		em_PrintType_LeftWall,
		em_PrintType_TopWall,
		em_PrintType_RightWall,
		em_PrintType_BottomWall,
		em_PrintType_SnakeHead_Dir_Top,
		em_PrintType_SnakeHead_Dir_Bottom,
		em_PrintType_SnakeHead_Dir_Left,
		em_PrintType_SnakeHead_Dir_Right,
		em_PrintType_SnakeBody,
		em_PrintType_Food,
		em_PrintType_SnakeTail
	};
public:
	CWall();
	~CWall() = default;

	// ´´½¨Ç½
	VOID CreateWall(_In_ DWORD dwHeight, _In_ DWORD dwWidth);

	//
	DWORD GetWidth() CONST _NOEXCEPT;

	//
	DWORD GetHeight() CONST _NOEXCEPT;

	//
	VOID SetPointAttribute(_In_ CONST POINT& Tar, BOOL bEnable);

	//
	std::vector<POINT> GetVecUsefulPoint(_In_ CONST std::deque<POINT>& VecSnake, _In_ CONST POINT& Food) CONST;

	// 
	BOOL IsKnockWall(_In_ CONST POINT& Tar) CONST;

	//
	VOID Clear();
public:
	static VOID PrintByPoint(_In_ POINT TarPoint, _In_ em_PrintType emPrintType);

	static VOID PrintSnakeByPoint(_In_ POINT TarPoint, _In_ em_PrintType emPrintType);

	static VOID HideCursor();
private:
	DWORD _dwHeight;
	DWORD _dwWidth;
	std::vector<std::vector<BOOL>> _VecChess;
};



#endif // !__SNAKE_WALL_H__
