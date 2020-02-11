#pragma once
#include "Singleton.h"
#include "CommonGlobal.h"

#define gBlocks() Blocks::GetInstance()

class Block {
	int x, y;
	int xSize, ySize;
public:
	Block(int _x, int _y, int _xSize, int _ySize) {
		x = _x;
		y = _y;
		xSize = _xSize;
		ySize = _ySize;
	}
	void changeX(int _x) { x += _x; }
	void changeY(int _y) { y += _y; }
	void setX(int _x) { x += _x; }
	void setY(int _y) { y += _y; }
	void setYSize(int _ySize) { ySize = _ySize; }
	const int getX() const { return x; }
	const int getY() const { return y; }
	const int getXsize() const { return xSize; }
	const int getYsize() const { return ySize; }
};

class Blocks {
	SINGLTONE_BASE_DEFAULT(Blocks);
	std::vector<Block> vecBlocks;
public:
	Blocks() {}
	const std::vector<Block> getBlocks() const { return vecBlocks; }
	void clearBlocks() { vecBlocks.clear(); }

	void generateBlocks() {
		int blockSize = fieldSizeX / 10;
		int xStart = fieldSizeX / 2;
		int yStart = 20 + rand() % 2 * (fieldSizeY / 4);

		for (int i = 0; i < MAX_BLOCKS; i++) {
			Block b1(xStart, 0, blockSize, yStart);
			vecBlocks.push_back(b1);
			Block b2(xStart, yStart + 4*blockMin, blockSize, fieldSizeY);
			vecBlocks.push_back(b2);

			xStart += 2 * blockSize + rand() % ( 2 * blockSize);
			yStart = 20 + rand() % 2 * (fieldSizeY / 4);
		}
	}
	void moveBlocks() {
		for (auto & curBlock : vecBlocks)
			curBlock.changeX(-blockMin/10);//двигаем блоки

		for (auto & curBlock = vecBlocks.begin(); curBlock != vecBlocks.end(); curBlock += 2) {
			if (curBlock->getX() + curBlock->getXsize() > 0)
				continue;

			higthscore++;// мы одолели блок + очко
			
			// если блоки вышли за границу, переместим их справа с рандомной задержкой
			int xStart = fieldSizeX + rand() % (fieldSizeX / 10);
			curBlock->setX(xStart);
			(curBlock + 1)->setX(xStart);


			int yStart = rand() % 2 * (fieldSizeY / 50);
			curBlock->setYSize(yStart);
			curBlock->setY(0);

			(curBlock+1)->setYSize(yStart);
			(curBlock+1)->setY(fieldSizeY - yStart + blockMin);
		}
	}
};