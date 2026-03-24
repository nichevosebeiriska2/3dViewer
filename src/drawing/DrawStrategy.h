#ifndef DRAWING_STRATEGY_H
#define DRAWING_STRATEGY_H

#include "common.h"

class DrawingStrategy
{
public:
	DrawingStrategy() = default;
	virtual void Draw() = 0;
};

class DrawingStrategyMesh : public DrawingStrategy
{
public:
	DrawingStrategyMesh() = default;
	void Draw() override;
};

class DrawingStrategyWireFrame : public DrawingStrategy
{
public:
	DrawingStrategyWireFrame() = default;
	void Draw() override;
};

#endif // !DRAWING_STRATEGY_H