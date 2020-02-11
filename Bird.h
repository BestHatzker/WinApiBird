#pragma once

#include "Block.h"
#include "Singleton.h"
#include "CommonGlobal.h"
#include <fstream>
#include <ctime>
#include <iomanip>


#define gBird() Bird::GetInstance()

class Bird {
	SINGLTONE_BASE_DEFAULT(Bird);
private:
	int x, y;
	int bSize;
	int speed; // только вертикальная скорость
	int angle; // угол относительно вертикали
	bool alive;
	bool fallen;

public:
	Bird() {}
	void initBird(int birdSize) {
		x = fieldSizeX / 100;
		y = fieldSizeY / 2;
		bSize = birdSize;
		speed = 0;
		angle = 61;
		alive = true;
		fallen = false;
	};

	const bool isAlive() const { return alive;}
	const int getAngle() const { return angle;}
	const int getX() const { return x;}
	const int getY() const { return y;}
	const int getSize() const { return bSize;}

	void killBird() {
		alive = false;

		auto start = std::chrono::system_clock::now();
		auto end = std::chrono::system_clock::now();

		std::chrono::duration<double> elapsed_seconds = end - start;
		std::time_t end_time = std::chrono::system_clock::to_time_t(end);

		std::wofstream out("higthscore", std::ios::app);
		if (out.is_open())
		{
			out << higthscore << L" - " << std::ctime(&end_time) << std::endl;
		}
		out.close();

		std::wstring higthscoreStr = L"Ваша птица разбилась. Очки: ";
		higthscoreStr += std::to_wstring(higthscore);
		higthscoreStr += L". Хотите начать заново?";

		//LPCWSTR sw = higthscoreStr.c_str();
		if (IDNO == MessageBox(NULL, higthscoreStr.c_str(), L"Смерть", MB_YESNO))
			exit(0);
		else
			startingGame = true;
	}

	const bool checkAlive() const {
		for (auto curBlock : gBlocks()->getBlocks())
			if (doOverlap(x, y, x + bSize, y + bSize,
				curBlock.getX(), curBlock.getY(),
				curBlock.getX() + curBlock.getXsize(),
				curBlock.getY() + curBlock.getYsize()))
				return false;
		return true;
	}

	void fly() {

		if (!fallen) {
			if (angle < 60)
				angle = 60;
			else if (angle > 120)
				angle = 120;
			else
				angle -= 3;
		}

		if (fallen)
			speed = 0;
		else if (speed < -bSize/2)
			speed = -bSize/2;
		else if (speed > bSize/2)
			speed = bSize/2;
		else
			speed += bSize/35 * cos(angle*PI / 180);

		if (!fallen)
			y += speed;


		if (y < 0) {
			y = 0;
			speed /= 2;
		}
		else if (y > fieldSizeY - bSize) {
			y = fieldSizeY - bSize;
			fallen = true;
		}
	};

	void flyUp() {
		fallen = false;
		angle += 10;
		y -= bSize/6;

		if (angle < 60)
			angle = 60;
		else if (angle > 120)
			angle = 120;

		if (speed > bSize/2)
			speed = bSize / 2;
		else
			speed -= bSize/3;

		if (y < 0) {
			y = 0;
			speed /= 2;
		}
	};
};
