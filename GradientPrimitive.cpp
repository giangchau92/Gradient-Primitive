#include "GradientPrimitive.h"


GradientPrimitive::GradientPrimitive(void)
{
}


GradientPrimitive::~GradientPrimitive(void)
{
}

void GradientPrimitive::init()
{
	//_shader = new GLProgram();
	//bool check = _shader->initWithFilenames("D:\\position_gradientcolor_vert.txt", "D:\\position_gradientcolor_frag.txt");
	_shader = ShaderCache::getInstance()->getProgram(GLProgram::SHADER_NAME_POSITION_COLOR);
	_shader->bindAttribLocation("in_position", 0);
	_shader->bindAttribLocation("in_color", 1);
	_shader->link();
	_shader->updateUniforms();
	_shader->retain();
}

void GradientPrimitive::drawLine( const Point &p1, const Point &p2, const float width, const Color4F &color1, const Color4F &color2 )
{
	GLfloat* vertices = new GLfloat[8];
	GLfloat* colors = new GLfloat[16];

	Point line = Point(p2.x - p1.x, p2.y - p1.y);
	Point normal = Point(-line.y, line.x).normalize();
	Point a = p1 - normal * width / 2.0f;
	Point b = p1 + normal * width / 2.0f;
	Point c = p2 - normal * width / 2.0f;
	Point d = p2 + normal * width / 2.0f;

	vertices[0] = a.x;
	vertices[1] = a.y;
	vertices[2] = b.x;
	vertices[3] = b.y;
	vertices[4] = c.x;
	vertices[5] = c.y;
	vertices[6] = d.x;
	vertices[7] = d.y;

	colors[0] = colors[4] = color1.r;
	colors[1] = colors[5] = color1.g;
	colors[2] = colors[6] = color1.b;
	colors[3] = colors[7] = color1.a;

	colors[8] = colors[12] = color2.r;
	colors[9] = colors[13] = color2.g;
	colors[10] = colors[14] = color2.b;
	colors[11] = colors[15] = color2.a;

	_shader->use();
	_shader->setUniformsForBuiltins();
	GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_COLOR);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, vertices);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, colors);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void GradientPrimitive::drawCircle( const Point &p, const float r, const float startAngle, const float angle, const unsigned int segments )
{
	const float rad_per_segment = 2.0f * (float)M_PI/segments;
	GLfloat* vertices = new GLfloat[segments * 2];

	for (unsigned int i = 0; i < segments; i++ )
	{
		float rads = i*rad_per_segment;
		GLfloat j = r * cosf(startAngle + angle + rads) + p.x;
		GLfloat k = r * sinf(startAngle + angle + rads) + p.y;

		vertices[i*2] = j;
		vertices[i*2+1] = k;
	}


	glLineWidth(10);
	glShadeModel(GL_SMOOTH);
	_shader->use();
	_shader->setUniformsForBuiltins();
	GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, vertices);
	glDrawArrays(GL_LINE_LOOP, 0, segments);

	::free(vertices);
}

void GradientPrimitive::drawCircle( const Point &p, const float r, const float width, const float startAngle, const float angle, const unsigned int segments )
{
	const float rad_per_segment = 2.0f * (float)M_PI/segments;
	GLfloat* vertices = new GLfloat[segments * 4 + 4];
	unsigned int i;
	for ( i = 0; i < segments; i++ )
	{
		float rads = i*rad_per_segment;
		GLfloat j = (r + width/2) * cosf(startAngle + angle + rads) + p.x;
		GLfloat k = (r + width/2) * sinf(startAngle + angle + rads) + p.y;
		GLfloat g = (r - width/2) * cosf(startAngle + angle + rads) + p.x;
		GLfloat h = (r - width/2) * sinf(startAngle + angle + rads) + p.y;

		vertices[i*4]   = j;
		vertices[i*4+1] = k;
		vertices[i*4+2] = g;
		vertices[i*4+3] = h;
	}
	vertices[i*4]		=  vertices[0];
	vertices[i*4+1]		=  vertices[1];
	vertices[i*4+2]		=  vertices[2];
	vertices[i*4+3]		=  vertices[3];


	glLineWidth(1);
	glShadeModel(GL_SMOOTH);
	_shader->use();
	_shader->setUniformsForBuiltins();
	GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, vertices);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, segments*2+2);

	::free(vertices);
}

void GradientPrimitive::drawCircle( const Point &p, const float r, const float width, const float startAngle, const float angle,
								   const unsigned int segments, const Color4F color1, const Color4F color2 )
{
	const float rad_per_segment = 2.0f * (float)M_PI/segments;
	GLfloat* vertices = new GLfloat[segments * 2 * 2 + 4];
	GLfloat* colors = new GLfloat[segments * 2 * 4 + 8];
	unsigned int i;
	for ( i = 0; i < segments; i++ )
	{
		float rads = i*rad_per_segment;
		GLfloat j = (r + width/2) * cosf(startAngle + angle + rads) + p.x;
		GLfloat k = (r + width/2) * sinf(startAngle + angle + rads) + p.y;
		GLfloat m = (r - width/2) * cosf(startAngle + angle + rads) + p.x;
		GLfloat n = (r - width/2) * sinf(startAngle + angle + rads) + p.y;
		vertices[i*4]   = j;
		vertices[i*4+1] = k;
		vertices[i*4+2] = m;
		vertices[i*4+3] = n;

		GLfloat r =	color1.r + (1.0f/segments * i) * (color2.r - color1.r);
		GLfloat g =	color1.g + (1.0f/segments * i) * (color2.g - color1.g);
		GLfloat b =	color1.b + (1.0f/segments * i) * (color2.b - color1.b);
		GLfloat a =	color1.a + (1.0f/segments * i) * (color2.a - color1.a);
		colors[i*8]		= r;
		colors[i*8+1]	= g;
		colors[i*8+2]	= b;
		colors[i*8+3]	= a;
		colors[i*8+4]	= r;
		colors[i*8+5]	= g;
		colors[i*8+6]	= b;
		colors[i*8+7]	= a;
	}
	vertices[i*4]		=  vertices[0];
	vertices[i*4+1]		=  vertices[1];
	vertices[i*4+2]		=  vertices[2];
	vertices[i*4+3]		=  vertices[3];

	colors[i*8]		= colors[0];
	colors[i*8+1]	= colors[1];
	colors[i*8+2]	= colors[2];
	colors[i*8+3]	= colors[3];
	colors[i*8+4]	= colors[4];
	colors[i*8+5]	= colors[5];
	colors[i*8+6]	= colors[6];
	colors[i*8+7]	= colors[7];


	glLineWidth(1);
	glShadeModel(GL_SMOOTH);
	_shader->use();
	_shader->setUniformsForBuiltins();
	GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_COLOR);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, vertices);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, colors);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, segments*2+2);

	::free(vertices);
	::free(colors);
}

GLProgram* GradientPrimitive::_shader = NULL;

