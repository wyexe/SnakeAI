#ifndef __SNAKE_SNAKE_H__
#define __SNAKE_SNAKE_H__

#include <queue>
#include <MyTools/ClassInstance.h>
#include "Wall.h"

class CSnakeAI;
class CSnake : public MyTools::CClassInstance<CSnake>
{
public:
	enum class em_Snake_Difficulty
	{
		em_Snake_Difficulty_Easy,
		em_Snake_Difficulty_Normal,
		em_Snake_Difficulty_Hard
	};
	enum class em_Snake_Direction
	{
		em_Snake_Direction_None,
		em_Snake_Direction_Top,
		em_Snake_Direction_Left,
		em_Snake_Direction_Right,
		em_Snake_Direction_Bottom
	};

	struct SnakeRecordContent
	{
		POINT Food;
		std::deque<POINT> Snake;
	};
public:
	CSnake();
	~CSnake() = default;

private:
	CSnake(CONST CSnake&) = delete;
	CSnake& operator = (CONST CSnake&) = delete;

public:
	VOID CreateSnake();

	VOID Run(_In_ DWORD dwWidth, _In_ DWORD dwHeight, _In_ em_Snake_Difficulty emDifficulty);

	VOID Initialize(_In_ DWORD dwWidth, _In_ DWORD dwHeight);
private:
	BOOL CreateFood();

	BOOL CreateEnablePoint(_Out_ POINT& UsefulPoint) CONST;

	DWORD GetSleepTime(_In_ em_Snake_Difficulty emDifficulty) CONST;

	VOID GameOver() CONST;

	BOOL TurnToDirection(_In_ em_Snake_Direction emDir);

	BOOL SetSnakeNextStep(_In_ em_Snake_Direction emDir, _In_ CONST POINT& NextPoint);

	BOOL IsKnockSnakeBody(_In_ CONST POINT& Tar) CONST;

	VOID Ready();

	VOID AddToRecord();

	VOID BackToRecord(int nCount, _In_ DWORD dwWidth, _In_ DWORD dwHeight);
private:
	CWall _Wall;
	POINT _Food;
	BOOL _bWin;
	std::deque<POINT> _DeqSnake;
	std::deque<SnakeRecordContent> _VecRecord;
};



#endif // !__SNAKE_SNAKE_H__
