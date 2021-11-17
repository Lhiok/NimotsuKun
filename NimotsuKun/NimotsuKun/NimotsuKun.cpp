// NimotsuKun.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <string>
#include <iostream>
using namespace std;

enum class EnumUnitType
{
	Unknown,				// δ֪
	Air = ' ',				// ����
	Wall = '#',				// ǽ��
	Player = 'p',			// ���
	Box = 'o',				// ����
	Target = '.',			// Ŀ�ĵ�
	Player_On_Target = 'P', // ��Ŀ�ĵ��ϵ����
	Box_On_Target = 'O',	// ��Ŀ�ĵ��ϵ�����
};

enum class EnumInputType
{
	Other,			// ����
	Left = 'a',		// ����
	Right = 'd',	// ����
	Up = 'w',		// ����
	Down = 's',		// ����
};

bool hasWon(const EnumUnitType* mapMess, const int width, const int height);

void initGame(EnumUnitType*& mapMess, int& width, int& height, int& posX, int& posY);
void draw(const EnumUnitType* mapMess, int width, int height);
void getInput(EnumInputType& player_input);
void updateGame(EnumUnitType*& mapMess, const int width, const int height, int& posX, int& posY, const EnumInputType player_input);

void Move(EnumUnitType*& mapMess, const int width, const int height, int& posX, int& posY, const int dx, const int dy);
int getNexPos(const int width, const int height, const int posX, const int posY, const int dx, const int dy);
bool MoveToAir(EnumUnitType*& mapMess, const int pos);
bool MoveToWall(EnumUnitType*& mapMess, const int pos);
bool MoveToBox(EnumUnitType*& mapMess, const int pos, const int nexPos);
bool MoveToTarget(EnumUnitType*& mapMess, const int pos);
bool MoveToBoxOnTarget(EnumUnitType*& mapMess, const int pos, const int nexPos);


bool BoxMove(EnumUnitType*& mapMess, const int pos);
bool BoxMoveToAir(EnumUnitType*& mapMess, const int pos);
bool BoxMoveToWall(EnumUnitType*& mapMess, const int pos);
bool BoxMoveToBox(EnumUnitType*& mapMess, const int pos);
bool BoxMoveToTarget(EnumUnitType*& mapMess, const int pos);
bool BoxMoveToBoxOnTarget(EnumUnitType*& mapMess, const int pos);

int main()
{
	// ��ͼ��Ϣ
	EnumUnitType* mapMess = nullptr;
	// ��ͼ��С
	int width = 0, height = 0;
	// �������
	int posX = -1, posY = -1;
	// �������
	EnumInputType player_input = EnumInputType::Other;

	initGame(mapMess, width, height, posX, posY);
	while (true)
	{
		draw(mapMess, width, height);
		getInput(player_input);
		updateGame(mapMess, width, height, posX, posY, player_input);
		if (hasWon(mapMess, width, height)) break;
	}
	delete[] mapMess;
	mapMess = nullptr;
	return 0;
}

static const string Win_Tip = "You Win!!!"; // ʤ����ʾ

// �Ƿ��Ѿ���ʤ
bool hasWon(const EnumUnitType* mapMess, const int width, const int height)
{
	for (int i = height * width - 1; ~i; --i)
	{
		// ����δ����Ŀ�ĵص����Ӿͼ�����Ϸ
		if (mapMess[i] == EnumUnitType::Box)
		{
			return false;
		}
	}
	cout << Win_Tip << endl;
	return true;
}

// ��ʼ������
//	########
//	# .. p #
//	# oo   #
//	#      #
//	########
void initGame(EnumUnitType*& mapMess, int& width, int& height, int& posX, int& posY)
{
	width = 8;
	height = 5;
	mapMess = new EnumUnitType[width * height];

	for (int i = width * height - 1; ~i; --i) mapMess[i] = EnumUnitType::Air;

	// ���
	posX = 5;
	posY = 1;
	mapMess[posY * width + posX] = EnumUnitType::Player;

	// ǽ��
	for (int x = 0; x < 8; ++x) mapMess[0 * width + x] = mapMess[4 * width + x] = EnumUnitType::Wall;
	for (int y = 0; y < 5; ++y) mapMess[y * width + 0] = mapMess[y * width + 7] = EnumUnitType::Wall;

	// ����
	mapMess[2 * width + 2] = mapMess[2 * width + 3] = EnumUnitType::Box;

	// Ŀ�ĵ�
	mapMess[1 * width + 2] = mapMess[1 * width + 3] = EnumUnitType::Target;
}

// ����
void draw(const EnumUnitType* mapMess, int width, int height)
{
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			cout << (char)mapMess[y * width + x];
		}
		cout << endl;
	}
}

static const char* Tip = "Left:%c Right:%c Up:%c Down:%c command?"; // ������ʾ

// ��ȡ����
void getInput(EnumInputType& player_input)
{
	// �����ʾ
	printf(Tip, EnumInputType::Left, EnumInputType::Right, EnumInputType::Up, EnumInputType::Down);

	// ��ȡ����
	char input;
	cin >> input;

	// ת������
	if (input == (char)EnumInputType::Left) player_input = EnumInputType::Left;
	else if (input == (char)EnumInputType::Right) player_input = EnumInputType::Right;
	else if (input == (char)EnumInputType::Up) player_input = EnumInputType::Up;
	else if (input == (char)EnumInputType::Down) player_input = EnumInputType::Down;
	else player_input = EnumInputType::Other;
}

// ��Ϸ����
void updateGame(EnumUnitType*& mapMess, const int width, const int height, int& posX, int& posY, const EnumInputType player_input)
{
	// ���������ȡ�ƶ��仯��
	int dx = 0, dy = 0;
	if (player_input == EnumInputType::Left) --dx;
	else if (player_input == EnumInputType::Right) ++dx;
	else if (player_input == EnumInputType::Up) --dy;
	else if (player_input == EnumInputType::Down) ++dy;
	else return;

	Move(mapMess, width, height, posX, posY, dx, dy);
}

// ����ƶ�
void Move(EnumUnitType*& mapMess, const int width, const int height, int& posX, int& posY, const int dx, const int dy)
{
	// ��һλ��(һά)
	int nexPos = getNexPos(width, height, posX, posY, dx, dy);
	if (nexPos < 0)
	{
		return;
	}

	// ����λ��(һά)
	int nexNexPos = getNexPos(width, height, posX + dx, posY + dy, dx, dy);
	
	// �ƶ����
	bool res = false;
	// ��ǰλ��(һά)
	int pos = posY * width + posX;

	// ���λ�úϷ��Լ��
	if (mapMess[pos] != EnumUnitType::Player && mapMess[pos] != EnumUnitType::Player_On_Target)
	{
		cout << "Player's Position Error!!!" << endl;
		exit(-1);
	}

	switch (mapMess[nexPos])
	{
	case EnumUnitType::Air:
		res = MoveToAir(mapMess, nexPos);
		break;
	case EnumUnitType::Wall:
		res = MoveToWall(mapMess, nexPos);
		break;
	case EnumUnitType::Box:
		res = nexNexPos < 0 ? false : MoveToBox(mapMess, nexPos, nexNexPos);
		break;
	case EnumUnitType::Target:
		res = MoveToTarget(mapMess, nexPos);
		break;
	case EnumUnitType::Box_On_Target:
		res = nexNexPos < 0 ? false : MoveToBoxOnTarget(mapMess, nexPos, nexNexPos);
		break;
	default:
		cout << "Unexpected Unit!!!" << endl;
		exit(-1);
	}

	// �ƶ�ʧ�� ��������
	if (!res)
	{
		return;
	}

	// ����������ڵ�λ
	mapMess[pos] = (mapMess[pos] == EnumUnitType::Player)? EnumUnitType::Air: EnumUnitType::Target;

	// �������λ��
	posX = posX + dx;
	posY = posY + dy;
}

int getNexPos(const int width, const int height, const int posX, const int posY, const int dx, const int dy)
{
	int nexPosX = posX + dx, nexPosY = posY + dy;

	// Խ����
	if (nexPosX < 0 || nexPosX >= width) return -1;
	if (nexPosY < 0 || nexPosY >= height) return -1;

	return nexPosY * width + nexPosX;
}

// �¸�λ���ǿ���
bool MoveToAir(EnumUnitType*& mapMess, const int pos)
{
	mapMess[pos] = EnumUnitType::Player;
	return true;
}

// �¸�λ����ǽ
bool MoveToWall(EnumUnitType*& mapMess, const int pos)
{
	return false;
}

// �¸�λ��������
bool MoveToBox(EnumUnitType*& mapMess, const int pos, const int nexPos)
{
	if (!BoxMove(mapMess, nexPos))
	{
		return false;
	}

	mapMess[pos] = EnumUnitType::Player;
	return true;
}

// �¸�λ����Ŀ�ĵ�
bool MoveToTarget(EnumUnitType*& mapMess, const int pos)
{
	mapMess[pos] = EnumUnitType::Player_On_Target;
	return true;
}

// �¸�λ���ǵ���Ŀ�ĵص�����
bool MoveToBoxOnTarget(EnumUnitType*& mapMess, const int pos, const int nexPos)
{
	if (!BoxMove(mapMess, nexPos))
	{
		return false;
	}

	mapMess[pos] = EnumUnitType::Player_On_Target;
	return true;
}

// �����ƶ�
bool BoxMove(EnumUnitType*& mapMess, const int pos)
{
	bool res = false;

	switch (mapMess[pos])
	{
	case EnumUnitType::Air:
		res = BoxMoveToAir(mapMess, pos);
		break;
	case EnumUnitType::Wall:
		res = BoxMoveToWall(mapMess, pos);
		break;
	case EnumUnitType::Box:
		res = BoxMoveToBox(mapMess, pos);
		break;
	case EnumUnitType::Target:
		res = BoxMoveToTarget(mapMess, pos);
		break;
	case EnumUnitType::Box_On_Target:
		res = BoxMoveToBoxOnTarget(mapMess, pos);
		break;
	default:
		cout << "Unexpected Unit!!!" << endl;
		exit(-1);
	}

	return res;
}

// �¸�λ���ǿ���
bool BoxMoveToAir(EnumUnitType*& mapMess, const int pos)
{
	mapMess[pos] = EnumUnitType::Box;
	return true;
}

// �¸�λ����ǽ
bool BoxMoveToWall(EnumUnitType*& mapMess, const int pos)
{
	return false;
}

// �¸�λ��������
bool BoxMoveToBox(EnumUnitType*& mapMess, const int pos)
{
	return false;
}

// �¸�λ����Ŀ�ĵ�
bool BoxMoveToTarget(EnumUnitType*& mapMess, const int pos)
{
	mapMess[pos] = EnumUnitType::Box_On_Target;
	return true;
}

// �¸�λ���ǵ���Ŀ�ĵص�����
bool BoxMoveToBoxOnTarget(EnumUnitType*& mapMess, const int pos)
{
	return false;
}