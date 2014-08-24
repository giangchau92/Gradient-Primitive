#pragma once

#include <cocos2d.h>

USING_NS_CC;

class GradientPrimitive
{
private:
	static GLProgram* _shader;
public:
	GradientPrimitive(void);
	~GradientPrimitive(void);

	static void init();

	static void drawLine(const Point &p1, const Point &p2, const float width, const Color4F &color1, const Color4F &color2);
	static void drawCircle(const Point &p, const float r, const float startAngle, const float angle, const unsigned int segments);
	static void drawCircle(const Point &p, const float r, const float width, const float startAngle, const float angle, const unsigned int segments);
	static void drawCircle(const Point &p, const float r, const float width, const float startAngle, const float angle, const unsigned int segments,
		const Color4F color1, const Color4F color2);
};

