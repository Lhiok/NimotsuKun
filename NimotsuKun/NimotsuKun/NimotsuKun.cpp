// NimotsuKun.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <string>
#include <iostream>
using namespace std;

bool win();

void initGame();
void getInput();
void updateGame();
void draw();

void Move(int nexPosX, int nexPosY, int nexNexPosX, int nexNexPosY);
bool MoveToAir(int nexPosX, int nexPosY);
bool MoveToWall(int nexPosX, int nexPosY);
bool MoveToBox(int nexPosX, int nexPosY, int nexNexPosX, int nexNexPosY);
bool MoveToTarget(int nexPosX, int nexPosY);
bool MoveToBoxOnTarget(int nexPosX, int nexPosY, int nexNexPosX, int nexNexPosY);

bool BoxMove(int nexNexPosX, int nexNexPosY);
bool BoxMoveToAir(int nexPosX, int nexPosY);
bool BoxMoveToWall(int nexPosX, int nexPosY);
bool BoxMoveToBox(int nexPosX, int nexPosY);
bool BoxMoveToTarget(int nexPosX, int nexPosY);
bool BoxMoveToBoxOnTarget(int nexPosX, int nexPosY);

static const string Tip = "Left:a Right:d Up:w Down:s command?"; // ������ʾ
static const string Win_Tip = "You Win!!!"; // ʤ����ʾ

int main()
{
	initGame();
	while (true)
	{
		getInput();
		updateGame();
		draw();
		if (win())
		{
			cout << Win_Tip << endl;
			break;
		}
	}
	return 0;
}

static const char Air = ' ';				// ��
static const char Wall = '#';				// ǽ��
static const char Player = 'p';				// ���
static const char Box = 'o';				// ����
static const char Target = '.';				// Ŀ�ĵ�
static const char Player_On_Target = 'P';	// ��Ŀ�ĵ��ϵ����
static const char Box_On_Target = 'O';		// ��Ŀ�ĵ��ϵ�����

static const char Left = 'a';
static const char Right = 'd';
static const char Up = 'w';
static const char Down = 's';

char map[5][8];
int posX, posY;
char player_input;


bool win()
{
	for (int x = 0; x < 8; ++x)
	{
		for (int y = 0; y < 5; ++y)
		{
			if (map[y][x] == Target || map[y][x] == Box)
			{
				return false;
			}
		}
	}
	return true;
}

// ��ʼ��ͼ
//	########
//	# .. p #
//	# oo   #
//	#      #
//	########

void initGame()
{
	memset(map, Air, sizeof(map));

	// ���
	posX = 5;
	posY = 1;
	map[posY][posX] = Player;

	// ǽ��
	for (int x = 0; x < 8; ++x) map[0][x] = map[4][x] = Wall;
	for (int y = 0; y < 5; ++y) map[y][0] = map[y][7] = Wall;

	// ����
	map[2][2] = map[2][3] = Box;

	// Ŀ�ĵ�
	map[1][2] = map[1][3] = Target;
	draw();
}

void getInput()
{
	cout << Tip;
	cin >> player_input;
}

void updateGame()
{
	int xMove = 0, yMove = 0;
	int nexPosX, nexPosY, nexNexPosX, nexNexPosY;

	if (player_input == Left) --xMove;
	else if (player_input == Right) ++xMove;
	else if (player_input == Up) --yMove;
	else if (player_input == Down) ++yMove;
	else return;

	nexPosX = posX + xMove;
	nexPosY = posY + yMove;
	nexNexPosX = nexPosX + xMove;
	nexNexPosY = nexPosY + yMove;

	Move(nexPosX, nexPosY, nexNexPosX, nexNexPosY);
}

void draw()
{
	for (int y = 0; y < 5; ++y)
	{
		for (int x = 0; x < 8; ++x)
		{
			cout << map[y][x];
		}
		cout << endl;
	}
}


// ����ƶ�
void Move(int nexPosX, int nexPosY, int nexNexPosX, int nexNexPosY) {
	bool res;
	switch (map[nexPosY][nexPosX])
	{
	case Air:
		res = MoveToAir(nexPosX, nexPosY);
		break;
	case Wall:
		res = MoveToWall(nexPosX, nexPosY);
		break;
	case Box:
		res = MoveToBox(nexPosX, nexPosY, nexNexPosX, nexNexPosY);
		break;
	case Target:
		res = MoveToTarget(nexPosX, nexPosY);
		break;
	case Box_On_Target:
		res = MoveToBoxOnTarget(nexPosX, nexPosY, nexNexPosX, nexNexPosY);
		break;
	default:
		cout << "Unknown Type!!!" << endl;
		res = false;
		break;
	}

	if (!res)
	{
		return;
	}

	if (map[posY][posX] == Player) map[posY][posX] = Air;
	else if (map[posY][posX] == Player_On_Target) map[posY][posX] = Target;
	else cout << "Player's Position Error!!!" << endl;

	posX = nexPosX;
	posY = nexPosY;
}

// �¸�λ���ǿ���
bool MoveToAir(int nexPosX, int nexPosY)
{
	map[nexPosY][nexPosX] = Player;
	return true;
}

// �¸�λ����ǽ
bool MoveToWall(int nexPosX, int nexPosY)
{
	return false;
}

// �¸�λ��������
bool MoveToBox(int nexPosX, int nexPosY, int nexNexPosX, int nexNexPosY)
{
	if (!BoxMove(nexNexPosX, nexNexPosY))
	{
		return false;
	}

	map[nexPosY][nexPosX] = Player;
	return true;
}

// �¸�λ����Ŀ�ĵ�
bool MoveToTarget(int nexPosX, int nexPosY)
{
	map[nexPosY][nexPosX] = Player_On_Target;
	return true;
}

// �¸�λ���ǵ���Ŀ�ĵص�����
bool MoveToBoxOnTarget(int nexPosX, int nexPosY, int nexNexPosX, int nexNexPosY)
{
	if (!BoxMove(nexNexPosX, nexNexPosY))
	{
		return false;
	}

	map[nexPosY][nexPosX] = Player_On_Target;
	return true;
}

// �����ƶ�
bool BoxMove(int nexNexPosX, int nexNexPosY)
{
	bool res;
	switch (map[nexNexPosY][nexNexPosX])
	{
	case Air:
		res = BoxMoveToAir(nexNexPosX, nexNexPosY);
		break;
	case Wall:
		res = BoxMoveToWall(nexNexPosX, nexNexPosY);
		break;
	case Box:
		res = BoxMoveToBox(nexNexPosX, nexNexPosY);
		break;
	case Target:
		res = BoxMoveToTarget(nexNexPosX, nexNexPosY);
		break;
	case Box_On_Target:
		res = BoxMoveToBoxOnTarget(nexNexPosX, nexNexPosY);
		break;
	default:
		cout << "Unknown Type!!!" << endl;
		res = false;
		break;
	}

	return res;
}

// �¸�λ���ǿ���
bool BoxMoveToAir(int nexPosX, int nexPosY)
{
	map[nexPosY][nexPosX] = Box;
	return true;
}

// �¸�λ����ǽ
bool BoxMoveToWall(int nexPosX, int nexPosY)
{
	return false;
}

// �¸�λ��������
bool BoxMoveToBox(int nexPosX, int nexPosY)
{
	return false;
}

// �¸�λ����Ŀ�ĵ�
bool BoxMoveToTarget(int nexPosX, int nexPosY)
{
	map[nexPosY][nexPosX] = Box_On_Target;
	return true;
}

// �¸�λ���ǵ���Ŀ�ĵص�����
bool BoxMoveToBoxOnTarget(int nexPosX, int nexPosY)
{
	return false;
}