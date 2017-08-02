#include "Snake.h"
#include <MyTools/Character.h>
#include <MyTools/CLPublic.h>
#include <MyTools/CLFile.h>
#include <conio.h>
#include "SnakeAI.h"

CSnake::CSnake()
{
	ZeroMemory(&_Food, sizeof(_Food));
}

VOID CSnake::CreateSnake()
{
	POINT ShakeHead = {static_cast<LONG>(_Wall.GetWidth() / 2), static_cast<LONG>(_Wall.GetHeight() / 2)};

	_DeqSnake.push_back(ShakeHead);
	CWall::PrintSnakeByPoint(ShakeHead, CWall::em_PrintType::em_PrintType_SnakeHead_Dir_Top);
}

VOID CSnake::Run(_In_ DWORD dwWidth, _In_ DWORD dwHeight, _In_ em_Snake_Difficulty emDifficulty)
{
	CSnakeAI SnakeAI(dwWidth, dwHeight, _Wall);
	Ready();
	Initialize(dwWidth, dwHeight);

	CONST DWORD dwSleepTime = GetSleepTime(emDifficulty);
	em_Snake_Direction emLastDirection = em_Snake_Direction::em_Snake_Direction_None;


	for(;;)
	{
		/*for (;;)
		{
			if (_kbhit() && _getch() == VK_SPACE)
				break;

			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}*/
		/*for (int i = 0; i < 10 || true ; ++i)
		{
			if (_kbhit())
			{
				switch (_getch())
				{
				case 'W': case 'w':
					emSnakeCurrentDirection = emLastDirection != em_Snake_Direction::em_Snake_Direction_Bottom ? em_Snake_Direction::em_Snake_Direction_Top : em_Snake_Direction::em_Snake_Direction_None;
					break;
				case 'S': case 's':
					emSnakeCurrentDirection = emLastDirection != em_Snake_Direction::em_Snake_Direction_Top ? em_Snake_Direction::em_Snake_Direction_Bottom : em_Snake_Direction::em_Snake_Direction_None;
					break;
				case 'A': case 'a':
					emSnakeCurrentDirection = emLastDirection != em_Snake_Direction::em_Snake_Direction_Right ? em_Snake_Direction::em_Snake_Direction_Left : em_Snake_Direction::em_Snake_Direction_None;
					break;
				case 'D': case 'd':
					emSnakeCurrentDirection = emLastDirection != em_Snake_Direction::em_Snake_Direction_Left ? em_Snake_Direction::em_Snake_Direction_Right : em_Snake_Direction::em_Snake_Direction_None;
					break;
				default:
					emSnakeCurrentDirection = em_Snake_Direction::em_Snake_Direction_None;
					break;
				}
				break;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}*/
		static int SleepTime = 20;
		if (_kbhit())
		{
			switch (_getch())
			{
			case 'W':case 'w':
				SleepTime -= 10;
				break;
			case 'S': case 's':
				SleepTime += 10;
				break;
			default:
				break;
			}
		}


		em_Snake_Direction NextDir = em_Snake_Direction::em_Snake_Direction_None;
		if (!SnakeAI.GetNextDirection(_DeqSnake, _Food, NextDir))
		{
			::MessageBoxW(NULL, L"Invalid Path!", L"", NULL);

			int nBackStep = 1;
			BackToRecord(nBackStep, dwWidth, dwHeight);
			continue;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(SleepTime));
		NextDir = NextDir == em_Snake_Direction::em_Snake_Direction_None ? emLastDirection : NextDir;
		if (!TurnToDirection(NextDir))
		{
			GameOver();
			Initialize(dwWidth, dwHeight);
			Ready();
			continue;
		}
		emLastDirection = NextDir;
	}
}

VOID CSnake::Initialize(_In_ DWORD dwWidth, _In_ DWORD dwHeight)
{
	//
	_Wall.Clear();

	//
	_Wall.CreateWall(dwHeight, dwWidth);

	//
	CreateSnake();

	//
	CreateFood();
}

BOOL CSnake::CreateFood()
{
	if (!CreateEnablePoint(_Food))
		return FALSE;
	 
	_Wall.PrintSnakeByPoint(_Food, CWall::em_PrintType::em_PrintType_Food);
	return TRUE;
}

BOOL CSnake::CreateEnablePoint(_Out_ POINT& UsefulPoint) CONST
{
	auto Vec = _Wall.GetVecUsefulPoint(_DeqSnake, _Food);
	if (Vec.size() == 0)
		return FALSE;

	UsefulPoint = std::move(Vec.at(MyTools::CCharacter::GetRand(Vec.size() - 1, 0)));
	return TRUE;
}

DWORD CSnake::GetSleepTime(_In_ em_Snake_Difficulty emDifficulty) CONST
{
	struct DifficultyContent
	{
		em_Snake_Difficulty emSnakeDifficulty;
		DWORD				dwSleepTime;
	};

	CONST static std::vector<DifficultyContent> Vec = 
	{
		{ em_Snake_Difficulty::em_Snake_Difficulty_Easy, 400 },
		{ em_Snake_Difficulty::em_Snake_Difficulty_Normal, 200 },
		{ em_Snake_Difficulty::em_Snake_Difficulty_Hard, 100 },
	};

	auto pElement = MyTools::CLPublic::Vec_find_if_Const(Vec, [emDifficulty](CONST DifficultyContent& Content) { return Content.emSnakeDifficulty == emDifficulty; });
	return pElement == nullptr ? 0 : pElement->dwSleepTime;
}

VOID CSnake::GameOver() CONST
{
	::MessageBoxW(NULL, _bWin ?  L"Congratulation~ Victory!" : L"Sorry~ You Lose~", L"", NULL);
}

BOOL CSnake::TurnToDirection(_In_ em_Snake_Direction emDir)
{
	auto Head = *_DeqSnake.begin();
	switch (emDir)
	{
	case CSnake::em_Snake_Direction::em_Snake_Direction_None:
		return TRUE;
	case CSnake::em_Snake_Direction::em_Snake_Direction_Top:
		Head.y -= 1;
		break;
	case CSnake::em_Snake_Direction::em_Snake_Direction_Left:
		Head.x -= 1;
		break;
	case CSnake::em_Snake_Direction::em_Snake_Direction_Right:
		Head.x += 1;
		break;
	case CSnake::em_Snake_Direction::em_Snake_Direction_Bottom:
		Head.y += 1;
		break;
	default:
		break;
	}

	return SetSnakeNextStep(emDir, Head);
}

BOOL CSnake::SetSnakeNextStep(_In_ em_Snake_Direction emDir, _In_ CONST POINT& NextPoint)
{
	if (_Wall.IsKnockWall(NextPoint)) // Knock Wall
	{
		_bWin = FALSE;
		return FALSE;
	}
	else if (NextPoint.x == _Food.x && NextPoint.y == _Food.y)// Knock Food
	{
		if (!CreateFood())
		{
			_bWin = TRUE;
			return FALSE;
		}
	}
	else if (IsKnockSnakeBody(NextPoint)) // Knock Snake Body
	{
		_bWin = FALSE;
		return FALSE;
	}
	else
	{
		// Delete Tail
		auto Tail = _DeqSnake.back();
		CWall::PrintSnakeByPoint(Tail, CWall::em_PrintType::em_PrintType_Empty);

		_DeqSnake.pop_back();
	}
	

	// Add New Head
	_DeqSnake.push_front(NextPoint);

	switch (emDir)
	{
	case CSnake::em_Snake_Direction::em_Snake_Direction_Top:
		CWall::PrintSnakeByPoint(NextPoint, CWall::em_PrintType::em_PrintType_SnakeHead_Dir_Top);
		break;
	case CSnake::em_Snake_Direction::em_Snake_Direction_Left:
		CWall::PrintSnakeByPoint(NextPoint, CWall::em_PrintType::em_PrintType_SnakeHead_Dir_Left);
		break;
	case CSnake::em_Snake_Direction::em_Snake_Direction_Right:
		CWall::PrintSnakeByPoint(NextPoint, CWall::em_PrintType::em_PrintType_SnakeHead_Dir_Right);
		break;
	case CSnake::em_Snake_Direction::em_Snake_Direction_Bottom:
		CWall::PrintSnakeByPoint(NextPoint, CWall::em_PrintType::em_PrintType_SnakeHead_Dir_Bottom);
		break;
	default:
		break;
	}
	

	if (_DeqSnake.size() > 1)
	{
		// Update Old Head to Body
		CWall::PrintSnakeByPoint(_DeqSnake.at(1), CWall::em_PrintType::em_PrintType_SnakeBody);
	}
	AddToRecord();
	return TRUE;
}

BOOL CSnake::IsKnockSnakeBody(_In_ CONST POINT& Tar) CONST
{
	return MyTools::CLPublic::Deque_find_if(_DeqSnake, static_cast<POINT*>(nullptr), [Tar](_In_ CONST POINT& itm) { return itm.x == Tar.x && itm.y == Tar.y; });
}

VOID CSnake::Ready()
{
	BOOL bReady = FALSE;
	while (!_kbhit() && !bReady)
	{
		switch (_getch())
		{
		case VK_SPACE:
			bReady = TRUE;
			break;
		default:
			break;
		}

		
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

}

VOID CSnake::AddToRecord()
{
	SnakeRecordContent Content;
	Content.Food = _Food;
	Content.Snake = _DeqSnake;
	_VecRecord.push_back(Content);
}

VOID CSnake::BackToRecord(int nCount, _In_ DWORD dwWidth, _In_ DWORD dwHeight)
{
	for (int i = 0;i < nCount; ++i)
	{
		_VecRecord.erase(_VecRecord.end() - 1);
	}

	_Wall.Clear();
	_Wall.CreateWall(dwHeight, dwWidth);

	auto Content = _VecRecord.back();
	_VecRecord.erase(_VecRecord.end() - 1);

	_Food = Content.Food;
	_Wall.PrintSnakeByPoint(_Food, CWall::em_PrintType::em_PrintType_Food);

	_DeqSnake = Content.Snake;
	for(CONST auto& itm : _DeqSnake)
	{
		CWall::PrintSnakeByPoint(itm, CWall::em_PrintType::em_PrintType_SnakeBody);
	}
}
