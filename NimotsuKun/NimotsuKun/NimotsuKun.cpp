// NimotsuKun.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <string>
#include <iostream>
using namespace std;

enum class EnumUnitType
{
	Unknown,				// 未知
	Air = ' ',				// 空气
	Wall = '#',				// 墙壁
	Player = 'p',			// 玩家
	Box = 'o',				// 箱子
	Target = '.',			// 目的地
	Player_On_Target = 'P', // 在目的地上的玩家
	Box_On_Target = 'O',	// 在目的地上的箱子
};

enum class EnumInputType
{
	Other,			// 其他
	Left = 'a',		// 左移
	Right = 'd',	// 右移
	Up = 'w',		// 上移
	Down = 's',		// 下移
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
	// 地图信息
	EnumUnitType* mapMess = nullptr;
	// 地图大小
	int width = 0, height = 0;
	// 玩家坐标
	int posX = -1, posY = -1;
	// 玩家输入
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

static const string Win_Tip = "You Win!!!"; // 胜利提示

// 是否已经获胜
bool hasWon(const EnumUnitType* mapMess, const int width, const int height)
{
	for (int i = height * width - 1; ~i; --i)
	{
		// 还有未到达目的地的箱子就继续游戏
		if (mapMess[i] == EnumUnitType::Box)
		{
			return false;
		}
	}
	cout << Win_Tip << endl;
	return true;
}

// 初始化数据
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

	// 玩家
	posX = 5;
	posY = 1;
	mapMess[posY * width + posX] = EnumUnitType::Player;

	// 墙壁
	for (int x = 0; x < 8; ++x) mapMess[0 * width + x] = mapMess[4 * width + x] = EnumUnitType::Wall;
	for (int y = 0; y < 5; ++y) mapMess[y * width + 0] = mapMess[y * width + 7] = EnumUnitType::Wall;

	// 箱子
	mapMess[2 * width + 2] = mapMess[2 * width + 3] = EnumUnitType::Box;

	// 目的地
	mapMess[1 * width + 2] = mapMess[1 * width + 3] = EnumUnitType::Target;
}

// 绘制
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

static const char* Tip = "Left:%c Right:%c Up:%c Down:%c command?"; // 输入提示

// 获取输入
void getInput(EnumInputType& player_input)
{
	// 输出提示
	printf(Tip, EnumInputType::Left, EnumInputType::Right, EnumInputType::Up, EnumInputType::Down);

	// 获取输入
	char input;
	cin >> input;

	// 转换输入
	if (input == (char)EnumInputType::Left) player_input = EnumInputType::Left;
	else if (input == (char)EnumInputType::Right) player_input = EnumInputType::Right;
	else if (input == (char)EnumInputType::Up) player_input = EnumInputType::Up;
	else if (input == (char)EnumInputType::Down) player_input = EnumInputType::Down;
	else player_input = EnumInputType::Other;
}

// 游戏更新
void updateGame(EnumUnitType*& mapMess, const int width, const int height, int& posX, int& posY, const EnumInputType player_input)
{
	// 根据输入获取移动变化量
	int dx = 0, dy = 0;
	if (player_input == EnumInputType::Left) --dx;
	else if (player_input == EnumInputType::Right) ++dx;
	else if (player_input == EnumInputType::Up) --dy;
	else if (player_input == EnumInputType::Down) ++dy;
	else return;

	Move(mapMess, width, height, posX, posY, dx, dy);
}

// 玩家移动
void Move(EnumUnitType*& mapMess, const int width, const int height, int& posX, int& posY, const int dx, const int dy)
{
	// 下一位置(一维)
	int nexPos = getNexPos(width, height, posX, posY, dx, dy);
	if (nexPos < 0)
	{
		return;
	}

	// 下下位置(一维)
	int nexNexPos = getNexPos(width, height, posX + dx, posY + dy, dx, dy);
	
	// 移动结果
	bool res = false;
	// 当前位置(一维)
	int pos = posY * width + posX;

	// 玩家位置合法性检测
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

	// 移动失败 不做处理
	if (!res)
	{
		return;
	}

	// 更新玩家所在单位
	mapMess[pos] = (mapMess[pos] == EnumUnitType::Player)? EnumUnitType::Air: EnumUnitType::Target;

	// 更新玩家位置
	posX = posX + dx;
	posY = posY + dy;
}

int getNexPos(const int width, const int height, const int posX, const int posY, const int dx, const int dy)
{
	int nexPosX = posX + dx, nexPosY = posY + dy;

	// 越界检测
	if (nexPosX < 0 || nexPosX >= width) return -1;
	if (nexPosY < 0 || nexPosY >= height) return -1;

	return nexPosY * width + nexPosX;
}

// 下个位置是空气
bool MoveToAir(EnumUnitType*& mapMess, const int pos)
{
	mapMess[pos] = EnumUnitType::Player;
	return true;
}

// 下个位置是墙
bool MoveToWall(EnumUnitType*& mapMess, const int pos)
{
	return false;
}

// 下个位子是箱子
bool MoveToBox(EnumUnitType*& mapMess, const int pos, const int nexPos)
{
	if (!BoxMove(mapMess, nexPos))
	{
		return false;
	}

	mapMess[pos] = EnumUnitType::Player;
	return true;
}

// 下个位置是目的地
bool MoveToTarget(EnumUnitType*& mapMess, const int pos)
{
	mapMess[pos] = EnumUnitType::Player_On_Target;
	return true;
}

// 下个位置是到达目的地的箱子
bool MoveToBoxOnTarget(EnumUnitType*& mapMess, const int pos, const int nexPos)
{
	if (!BoxMove(mapMess, nexPos))
	{
		return false;
	}

	mapMess[pos] = EnumUnitType::Player_On_Target;
	return true;
}

// 箱子移动
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

// 下个位置是空气
bool BoxMoveToAir(EnumUnitType*& mapMess, const int pos)
{
	mapMess[pos] = EnumUnitType::Box;
	return true;
}

// 下个位置是墙
bool BoxMoveToWall(EnumUnitType*& mapMess, const int pos)
{
	return false;
}

// 下个位子是箱子
bool BoxMoveToBox(EnumUnitType*& mapMess, const int pos)
{
	return false;
}

// 下个位置是目的地
bool BoxMoveToTarget(EnumUnitType*& mapMess, const int pos)
{
	mapMess[pos] = EnumUnitType::Box_On_Target;
	return true;
}

// 下个位置是到达目的地的箱子
bool BoxMoveToBoxOnTarget(EnumUnitType*& mapMess, const int pos)
{
	return false;
}