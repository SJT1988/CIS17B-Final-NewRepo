#include "Vector2D.h"

Vector2D::Vector2D()
{
	x = 0.0f;
	y = 0.0f;
}

Vector2D::Vector2D(float x, float y)
{
	this->x = x;
	this->y = y;
}

Vector2D & Vector2D::Add(const Vector2D & vec)
{
	this->x += vec.x;
	this->y += vec.y;

	return *this;
}

Vector2D & Vector2D::Subtract(const Vector2D & vec)
{
	this->x -= vec.x;
	this->y -= vec.y;

	return *this;
}

Vector2D & Vector2D::Multiply(const Vector2D & vec)
{
	this->x *= vec.x;
	this->y *= vec.y;

	return *this;
}

Vector2D & Vector2D::Divide(const Vector2D & vec)
{
	this->x /= vec.x;
	this->y /= vec.y;

	return *this;
}

float Vector2D::Norm()
{
	return sqrt(pow(this->x,2) + pow(this->y,2));
}

Vector2D & operator+(Vector2D & v1, const Vector2D & v2)
{
	return v1.Add(v2);
}

Vector2D & operator-(Vector2D & v1, const Vector2D & v2)
{
	return v1.Subtract(v2);
}

Vector2D & operator*(Vector2D & v1, const Vector2D & v2)
{
	return v1.Multiply(v2);
}

Vector2D & operator/(Vector2D & v1, const Vector2D & v2)
{
	return v1.Divide(v2);
}

std::ostream & operator<<(std::ostream & stream, const Vector2D & vec)
{
	stream << "<" << vec.x << ", " << vec.y << ">";
	return stream;
}

Vector2D & Vector2D::operator=(const Vector2D & vec)
{
	this->x = vec.x;
	this->y = vec.y;
	return *this;
}

Vector2D & Vector2D::operator+=(const Vector2D & vec)
{
	return this->Add(vec);
}

Vector2D & Vector2D::operator-=(const Vector2D & vec)
{
	return this->Subtract(vec);
}

Vector2D & Vector2D::operator*=(const Vector2D & vec)
{
	return this->Multiply(vec);
}

Vector2D & Vector2D::operator/=(const Vector2D & vec)
{
	return this->Divide(vec);
}
/*
Vector2D & Vector2D::operator*(const float & f)
{
	this->x *= f;
	this->y *= f;
	return *this;
}
*/
inline Vector2D operator*(Vector2D vec, const float& f)
{
	Vector2D result;
	result.x = vec.x * f;
	result.y = vec.y * f;
	return result;
}
inline Vector2D operator*(const float& f, Vector2D vec)
{
	Vector2D result;
	result.x = vec.x * f;
	result.y = vec.y * f;
	return result;
}


Vector2D & Vector2D::Zero()
{
	this->x = 0;
	this->y = 0;
	return *this;
}

bool operator== (const Vector2D &v1, const Vector2D &v2)
{
	if (v1.x == v2.x && v1.y == v2.y) return true;
	else return false;
}
bool operator!= (const Vector2D &v1, const Vector2D &v2)
{
	if (v1.x == v2.x && v1.y == v2.y) return false;
	else return true;
}