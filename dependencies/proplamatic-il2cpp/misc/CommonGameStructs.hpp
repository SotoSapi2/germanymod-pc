#pragma once
#pragma warning(push)
#pragma warning(disable: C4244)
#pragma warning(disable: C4700)

#define _USE_MATH_DEFINES
#include <math.h>

struct Color
{
	float r;
	float g;
	float b;
	float a;

	Color(float r, float g, float b)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = 1;
	}

	Color(float r, float g, float b, float alpha)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = alpha;
	}
};

struct Vector2
{
	union
	{
		struct
		{
			float X;
			float Y;
		};
		float data[2];
	};


	/**
	 * Constructors.
	 */
	inline Vector2();
	inline Vector2(float data[]);
	inline Vector2(float value);
	inline Vector2(float x, float y);


	/**
	 * Constants for common vectors.
	 */
	static inline Vector2 Zero();
	static inline Vector2 One();
	static inline Vector2 Right();
	static inline Vector2 Left();
	static inline Vector2 Up();
	static inline Vector2 Down();


	/**
	 * Returns the angle between two vectors in radians.
	 * @param a: The first vector.
	 * @param b: The second vector.
	 * @return: A scalar value.
	 */
	static inline float Angle(Vector2 a, Vector2 b);

	/**
	 * Returns a vector with its magnitude clamped to maxLength.
	 * @param vector: The target vector.
	 * @param maxLength: The maximum length of the return vector.
	 * @return: A new vector.
	 */
	static inline Vector2 ClampMagnitude(Vector2 vector, float maxLength);

	/**
	 * Returns the component of a in the direction of b (scalar projection).
	 * @param a: The target vector.
	 * @param b: The vector being compared against.
	 * @return: A scalar value.
	 */
	static inline float Component(Vector2 a, Vector2 b);

	/**
	 * Returns the distance between a and b.
	 * @param a: The first point.
	 * @param b: The second point.
	 * @return: A scalar value.
	 */
	static inline float Distance(Vector2 a, Vector2 b);

	/**
	 * Returns the dot product of two vectors.
	 * @param lhs: The left side of the multiplication.
	 * @param rhs: The right side of the multiplication.
	 * @return: A scalar value.
	 */
	static inline float Dot(Vector2 lhs, Vector2 rhs);

	/**
	 * Converts a polar representation of a vector into cartesian
	 * coordinates.
	 * @param rad: The magnitude of the vector.
	 * @param theta: The angle from the X axis.
	 * @return: A new vector.
	 */
	static inline Vector2 FromPolar(float rad, float theta);

	/**
	 * Returns a vector linearly interpolated between a and b, moving along
	 * a straight line. The vector is clamped to never go beyond the end points.
	 * @param a: The starting point.
	 * @param b: The ending point.
	 * @param t: The interpolation value [0-1].
	 * @return: A new vector.
	 */
	static inline Vector2 Lerp(Vector2 a, Vector2 b, float t);

	/**
	 * Returns a vector linearly interpolated between a and b, moving along
	 * a straight line.
	 * @param a: The starting point.
	 * @param b: The ending point.
	 * @param t: The interpolation value [0-1] (no actual bounds).
	 * @return: A new vector.
	 */
	static inline Vector2 LerpUnclamped(Vector2 a, Vector2 b, float t);

	/**
	 * Returns the magnitude of a vector.
	 * @param v: The vector in question.
	 * @return: A scalar value.
	 */
	static inline float Magnitude(Vector2 v);

	/**
	 * Returns a vector made from the largest components of two other vectors.
	 * @param a: The first vector.
	 * @param b: The second vector.
	 * @return: A new vector.
	 */
	static inline Vector2 Max(Vector2 a, Vector2 b);

	/**
	 * Returns a vector made from the smallest components of two other vectors.
	 * @param a: The first vector.
	 * @param b: The second vector.
	 * @return: A new vector.
	 */
	static inline Vector2 Min(Vector2 a, Vector2 b);

	/**
	 * Returns a vector "maxDistanceDelta" units closer to the target. This
	 * interpolation is in a straight line, and will not overshoot.
	 * @param current: The current position.
	 * @param target: The destination position.
	 * @param maxDistanceDelta: The maximum distance to move.
	 * @return: A new vector.
	 */
	static inline Vector2 MoveTowards(Vector2 current, Vector2 target,
		float maxDistanceDelta);

	/**
	 * Returns a new vector with magnitude of one.
	 * @param v: The vector in question.
	 * @return: A new vector.
	 */
	static inline Vector2 Normalized(Vector2 v);

	/**
	 * Creates a new coordinate system out of the two vectors.
	 * Normalizes "normal" and normalizes "tangent" and makes it orthogonal to
	 * "normal"..
	 * @param normal: A reference to the first axis vector.
	 * @param tangent: A reference to the second axis vector.
	 */
	static inline void OrthoNormalize(Vector2& normal, Vector2& tangent);

	/**
	 * Returns the vector projection of a onto b.
	 * @param a: The target vector.
	 * @param b: The vector being projected onto.
	 * @return: A new vector.
	 */
	static inline Vector2 Project(Vector2 a, Vector2 b);

	/**
	 * Returns a vector reflected about the provided line.
	 * This behaves as if there is a plane with the line as its normal, and the
	 * vector comes in and bounces off this plane.
	 * @param vector: The vector traveling inward at the imaginary plane.
	 * @param line: The line about which to reflect.
	 * @return: A new vector pointing outward from the imaginary plane.
	 */
	static inline Vector2 Reflect(Vector2 vector, Vector2 line);

	/**
	 * Returns the vector rejection of a on b.
	 * @param a: The target vector.
	 * @param b: The vector being projected onto.
	 * @return: A new vector.
	 */
	static inline Vector2 Reject(Vector2 a, Vector2 b);

	/**
	 * Rotates vector "current" towards vector "target" by "maxRadiansDelta".
	 * This treats the vectors as directions and will linearly interpolate
	 * between their magnitudes by "maxMagnitudeDelta". This function does not
	 * overshoot. If a negative delta is supplied, it will rotate away from
	 * "target" until it is pointing the opposite direction, but will not
	 * overshoot that either.
	 * @param current: The starting direction.
	 * @param target: The destination direction.
	 * @param maxRadiansDelta: The maximum number of radians to rotate.
	 * @param maxMagnitudeDelta: The maximum delta for magnitude interpolation.
	 * @return: A new vector.
	 */
	static inline Vector2 RotateTowards(Vector2 current, Vector2 target,
		float maxRadiansDelta,
		float maxMagnitudeDelta);

	/**
	 * Multiplies two vectors component-wise.
	 * @param a: The lhs of the multiplication.
	 * @param b: The rhs of the multiplication.
	 * @return: A new vector.
	 */
	static inline Vector2 Scale(Vector2 a, Vector2 b);

	/**
	 * Returns a vector rotated towards b from a by the percent t.
	 * Since interpolation is done spherically, the vector moves at a constant
	 * angular velocity. This rotation is clamped to 0 <= t <= 1.
	 * @param a: The starting direction.
	 * @param b: The ending direction.
	 * @param t: The interpolation value [0-1].
	 */
	static inline Vector2 Slerp(Vector2 a, Vector2 b, float t);

	/**
	 * Returns a vector rotated towards b from a by the percent t.
	 * Since interpolation is done spherically, the vector moves at a constant
	 * angular velocity. This rotation is unclamped.
	 * @param a: The starting direction.
	 * @param b: The ending direction.
	 * @param t: The interpolation value [0-1].
	 */
	static inline Vector2 SlerpUnclamped(Vector2 a, Vector2 b, float t);

	/**
	 * Returns the squared magnitude of a vector.
	 * This is useful when comparing relative lengths, where the exact length
	 * is not important, and much time can be saved by not calculating the
	 * square root.
	 * @param v: The vector in question.
	 * @return: A scalar value.
	 */
	static inline float SqrMagnitude(Vector2 v);

	/**
	 * Calculates the polar coordinate space representation of a vector.
	 * @param vector: The vector to convert.
	 * @param rad: The magnitude of the vector.
	 * @param theta: The angle from the X axis.
	 */
	static inline void ToPolar(Vector2 vector, float& rad, float& theta);


	/**
	 * Operator overloading.
	 */
	inline struct Vector2& operator+=(const float rhs);
	inline struct Vector2& operator-=(const float rhs);
	inline struct Vector2& operator*=(const float rhs);
	inline struct Vector2& operator/=(const float rhs);
	inline struct Vector2& operator+=(const Vector2 rhs);
	inline struct Vector2& operator-=(const Vector2 rhs);
};

inline Vector2 operator-(Vector2 rhs);
inline Vector2 operator+(Vector2 lhs, const float rhs);
inline Vector2 operator-(Vector2 lhs, const float rhs);
inline Vector2 operator*(Vector2 lhs, const float rhs);
inline Vector2 operator/(Vector2 lhs, const float rhs);
inline Vector2 operator+(const float lhs, Vector2 rhs);
inline Vector2 operator-(const float lhs, Vector2 rhs);
inline Vector2 operator*(const float lhs, Vector2 rhs);
inline Vector2 operator/(const float lhs, Vector2 rhs);
inline Vector2 operator+(Vector2 lhs, const Vector2 rhs);
inline Vector2 operator-(Vector2 lhs, const Vector2 rhs);
inline bool operator==(const Vector2 lhs, const Vector2 rhs);
inline bool operator!=(const Vector2 lhs, const Vector2 rhs);



/*******************************************************************************
 * Implementation
 */

Vector2::Vector2() : X(0), Y(0)
{
}
Vector2::Vector2(float data[]) : X(data[0]), Y(data[1])
{
}
Vector2::Vector2(float value) : X(value), Y(value)
{
}
Vector2::Vector2(float x, float y) : X(x), Y(y)
{
}


Vector2 Vector2::Zero()
{
	return Vector2(0, 0);
}
Vector2 Vector2::One()
{
	return Vector2(1, 1);
}
Vector2 Vector2::Right()
{
	return Vector2(1, 0);
}
Vector2 Vector2::Left()
{
	return Vector2(-1, 0);
}
Vector2 Vector2::Up()
{
	return Vector2(0, 1);
}
Vector2 Vector2::Down()
{
	return Vector2(0, -1);
}


float Vector2::Angle(Vector2 a, Vector2 b)
{
	float v = Dot(a, b) / (Magnitude(a) * Magnitude(b));
	v = fmax(v, -1.0);
	v = fmin(v, 1.0);
	return acos(v);
}

Vector2 Vector2::ClampMagnitude(Vector2 vector, float maxLength)
{
	float length = Magnitude(vector);
	if (length > maxLength)
		vector *= maxLength / length;
	return vector;
}

float Vector2::Component(Vector2 a, Vector2 b)
{
	return Dot(a, b) / Magnitude(b);
}

float Vector2::Distance(Vector2 a, Vector2 b)
{
	return Vector2::Magnitude(a - b);
}

float Vector2::Dot(Vector2 lhs, Vector2 rhs)
{
	return lhs.X * rhs.X + lhs.Y * rhs.Y;
}

Vector2 Vector2::FromPolar(float rad, float theta)
{
	Vector2 v;
	v.X = rad * cos(theta);
	v.Y = rad * sin(theta);
	return v;
}

Vector2 Vector2::Lerp(Vector2 a, Vector2 b, float t)
{
	if (t < 0) return a;
	else if (t > 1) return b;
	return LerpUnclamped(a, b, t);
}

Vector2 Vector2::LerpUnclamped(Vector2 a, Vector2 b, float t)
{
	return (b - a) * t + a;
}

float Vector2::Magnitude(Vector2 v)
{
	return sqrt(SqrMagnitude(v));
}

Vector2 Vector2::Max(Vector2 a, Vector2 b)
{
	float x = a.X > b.X ? a.X : b.X;
	float y = a.Y > b.Y ? a.Y : b.Y;
	return Vector2(x, y);
}

Vector2 Vector2::Min(Vector2 a, Vector2 b)
{
	float x = a.X > b.X ? b.X : a.X;
	float y = a.Y > b.Y ? b.Y : a.Y;
	return Vector2(x, y);
}

Vector2 Vector2::MoveTowards(Vector2 current, Vector2 target,
	float maxDistanceDelta)
{
	Vector2 d = target - current;
	float m = Magnitude(d);
	if (m < maxDistanceDelta || m == 0)
		return target;
	return current + (d * maxDistanceDelta / m);
}

Vector2 Vector2::Normalized(Vector2 v)
{
	float mag = Magnitude(v);
	if (mag == 0)
		return Vector2::Zero();
	return v / mag;
}

void Vector2::OrthoNormalize(Vector2& normal, Vector2& tangent)
{
	normal = Normalized(normal);
	tangent = Reject(tangent, normal);
	tangent = Normalized(tangent);
}

Vector2 Vector2::Project(Vector2 a, Vector2 b)
{
	float m = Magnitude(b);
	return Dot(a, b) / (m * m) * b;
}

Vector2 Vector2::Reflect(Vector2 vector, Vector2 planeNormal)
{
	return vector - 2 * Project(vector, planeNormal);
}

Vector2 Vector2::Reject(Vector2 a, Vector2 b)
{
	return a - Project(a, b);
}

Vector2 Vector2::RotateTowards(Vector2 current, Vector2 target,
	float maxRadiansDelta,
	float maxMagnitudeDelta)
{
	float magCur = Magnitude(current);
	float magTar = Magnitude(target);
	float newMag = magCur + maxMagnitudeDelta *
		((magTar > magCur) - (magCur > magTar));
	newMag = fmin(newMag, fmax(magCur, magTar));
	newMag = fmax(newMag, fmin(magCur, magTar));

	float totalAngle = Angle(current, target) - maxRadiansDelta;
	if (totalAngle <= 0)
		return Normalized(target) * newMag;
	else if (totalAngle >= M_PI)
		return Normalized(-target) * newMag;

	float axis = current.X * target.Y - current.Y * target.X;
	axis = axis / fabs(axis);
	if (!(1 - fabs(axis) < 0.00001))
		axis = 1;
	current = Normalized(current);
	Vector2 newVector = current * cos(maxRadiansDelta) +
		Vector2(-current.Y, current.X) * sin(maxRadiansDelta) * axis;
	return newVector * newMag;
}

Vector2 Vector2::Scale(Vector2 a, Vector2 b)
{
	return Vector2(a.X * b.X, a.Y * b.Y);
}

Vector2 Vector2::Slerp(Vector2 a, Vector2 b, float t)
{
	if (t < 0) return a;
	else if (t > 1) return b;
	return SlerpUnclamped(a, b, t);
}

Vector2 Vector2::SlerpUnclamped(Vector2 a, Vector2 b, float t)
{
	float magA = Magnitude(a);
	float magB = Magnitude(b);
	a /= magA;
	b /= magB;
	float dot = Dot(a, b);
	dot = fmax(dot, -1.0);
	dot = fmin(dot, 1.0);
	float theta = acos(dot) * t;
	Vector2 relativeVec = Normalized(b - a * dot);
	Vector2 newVec = a * cos(theta) + relativeVec * sin(theta);
	return newVec * (magA + (magB - magA) * t);
}

float Vector2::SqrMagnitude(Vector2 v)
{
	return v.X * v.X + v.Y * v.Y;
}

void Vector2::ToPolar(Vector2 vector, float& rad, float& theta)
{
	rad = Magnitude(vector);
	theta = atan2(vector.Y, vector.X);
}


struct Vector2& Vector2::operator+=(const float rhs)
{
	X += rhs;
	Y += rhs;
	return *this;
}

struct Vector2& Vector2::operator-=(const float rhs)
{
	X -= rhs;
	Y -= rhs;
	return *this;
}

struct Vector2& Vector2::operator*=(const float rhs)
{
	X *= rhs;
	Y *= rhs;
	return *this;
}

struct Vector2& Vector2::operator/=(const float rhs)
{
	X /= rhs;
	Y /= rhs;
	return *this;
}

struct Vector2& Vector2::operator+=(const Vector2 rhs)
{
	X += rhs.X;
	Y += rhs.Y;
	return *this;
}

struct Vector2& Vector2::operator-=(const Vector2 rhs)
{
	X -= rhs.X;
	Y -= rhs.Y;
	return *this;
}

Vector2 operator-(Vector2 rhs)
{
	return rhs * -1;
}
Vector2 operator+(Vector2 lhs, const float rhs)
{
	return lhs += rhs;
}
Vector2 operator-(Vector2 lhs, const float rhs)
{
	return lhs -= rhs;
}
Vector2 operator*(Vector2 lhs, const float rhs)
{
	return lhs *= rhs;
}
Vector2 operator/(Vector2 lhs, const float rhs)
{
	return lhs /= rhs;
}
Vector2 operator+(const float lhs, Vector2 rhs)
{
	return rhs += lhs;
}
Vector2 operator-(const float lhs, Vector2 rhs)
{
	return rhs -= lhs;
}
Vector2 operator*(const float lhs, Vector2 rhs)
{
	return rhs *= lhs;
}
Vector2 operator/(const float lhs, Vector2 rhs)
{
	return rhs /= lhs;
}
Vector2 operator+(Vector2 lhs, const Vector2 rhs)
{
	return lhs += rhs;
}
Vector2 operator-(Vector2 lhs, const Vector2 rhs)
{
	return lhs -= rhs;
}

bool operator==(const Vector2 lhs, const Vector2 rhs)
{
	return lhs.X == rhs.X && lhs.Y == rhs.Y;
}

bool operator!=(const Vector2 lhs, const Vector2 rhs)
{
	return !(lhs == rhs);
}

struct Vector3
{
	union
	{
		struct
		{
			float X;
			float Y;
			float Z;
		};
		float data[3];
	};


	/**
	 * Constructors.
	 */
	inline Vector3();
	inline Vector3(float data[]);
	inline Vector3(float value);
	inline Vector3(float x, float y);
	inline Vector3(float x, float y, float z);

	/**
	 * Constants for common vectors.
	 */
	static inline Vector3 Zero();
	static inline Vector3 One();
	static inline Vector3 Right();
	static inline Vector3 Left();
	static inline Vector3 Up();
	static inline Vector3 Down();
	static inline Vector3 Forward();
	static inline Vector3 Backward();


	/**
	 * Returns the angle between two vectors in radians.
	 * @param a: The first vector.
	 * @param b: The second vector.
	 * @return: A scalar value.
	 */
	static inline float Angle(Vector3 a, Vector3 b);

	/**
	 * Returns a vector with its magnitude clamped to maxLength.
	 * @param vector: The target vector.
	 * @param maxLength: The maximum length of the return vector.
	 * @return: A new vector.
	 */
	static inline Vector3 ClampMagnitude(Vector3 vector, float maxLength);

	/**
	 * Retorna o componente de a na dire��o de b (proje��o escalar).
	 * @param a: O vetor de destino.
	 * @param b: O vetor que est� sendo comparado.
	 * @return: Um valor escalar.
	 */
	static inline float Component(Vector3 a, Vector3 b);

	/**
	 * Retorna o produto vetorial de dois vetores.
	 * @param lhs: O lado esquerdo da multiplica��o.
	 * @param rhs: O lado direito da multiplica��o.
	 * @return: Um novo vetor.
	 */
	static inline Vector3 Cross(Vector3 lhs, Vector3 rhs);

	/**
	 * Returns the distance between a and b.
	 * @param a: The first point.
	 * @param b: The second point.
	 * @return: A scalar value.
	 */
	static inline float Distance(Vector3 a, Vector3 b);

	static inline char ToChar(Vector3 a);

	/**
	 * Returns the dot product of two vectors.
	 * @param lhs: The left side of the multiplication.
	 * @param rhs: The right side of the multiplication.
	 * @return: A scalar value.
	 */
	static inline float Dot(Vector3 lhs, Vector3 rhs);

	/**
	 * Converte uma representa��o esf�rica de um vetor em cartesiano
	 * coordenadas.
	 * Isso usa a conven��o ISO (raio r, inclina��o theta, azimute phi).
	 * @param rad: A magnitude do vetor.
	 * @param theta: O �ngulo no plano XY do eixo X.
	 * @param phi: O �ngulo do eixo Z positivo para o vetor.
	 * @return: Um novo vetor.
	 */
	static inline Vector3 FromSpherical(float rad, float theta, float phi);

	/**
	 * Returns a vector linearly interpolated between a and b, moving along
	 * a straight line. The vector is clamped to never go beyond the end points.
	 * @param a: The starting point.
	 * @param b: The ending point.
	 * @param t: The interpolation value [0-1].
	 * @return: A new vector.
	 */
	static inline Vector3 Lerp(Vector3 a, Vector3 b, float t);

	/**
	 * Returns a vector linearly interpolated between a and b, moving along
	 * a straight line.
	 * @param a: The starting point.
	 * @param b: The ending point.
	 * @param t: The interpolation value [0-1] (no actual bounds).
	 * @return: A new vector.
	 */
	static inline Vector3 LerpUnclamped(Vector3 a, Vector3 b, float t);

	/**
	 * Returns the magnitude of a vector.
	 * @param v: The vector in question.
	 * @return: A scalar value.
	 */
	static inline float Magnitude(Vector3 v);

	/**
	 * Returns a vector made from the largest components of two other vectors.
	 * @param a: The first vector.
	 * @param b: The second vector.
	 * @return: A new vector.
	 */
	static inline Vector3 Max(Vector3 a, Vector3 b);

	/**
	 * Returns a vector made from the smallest components of two other vectors.
	 * @param a: The first vector.
	 * @param b: The second vector.
	 * @return: A new vector.
	 */
	static inline Vector3 Min(Vector3 a, Vector3 b);

	/**
	 * Returns a vector "maxDistanceDelta" units closer to the target. This
	 * interpolation is in a straight line, and will not overshoot.
	 * @param current: The current position.
	 * @param target: The destination position.
	 * @param maxDistanceDelta: The maximum distance to move.
	 * @return: A new vector.
	 */
	static inline Vector3 MoveTowards(Vector3 current, Vector3 target,
		float maxDistanceDelta);

	/**
	 * Returns a new vector with magnitude of one.
	 * @param v: The vector in question.
	 * @return: A new vector.
	 */
	static inline Vector3 Normalized(Vector3 v);

	/**
	 * Returns an arbitrary vector orthogonal to the input.
	 * This vector is not normalized.
	 * @param v: The input vector.
	 * @return: A new vector.
	 */
	static inline Vector3 Orthogonal(Vector3 v);

	/**
	 * Creates a new coordinate system out of the three vectors.
	 * Normalizes "normal", normalizes "tangent" and makes it orthogonal to
	 * "normal" and normalizes "binormal" and makes it orthogonal to both
	 * "normal" and "tangent".
	 * @param normal: A reference to the first axis vector.
	 * @param tangent: A reference to the second axis vector.
	 * @param binormal: A reference to the third axis vector.
	 */
	static inline void OrthoNormalize(Vector3& normal, Vector3& tangent,
		Vector3& binormal);

	/**
	 * Returns the vector projection of a onto b.
	 * @param a: The target vector.
	 * @param b: The vector being projected onto.
	 * @return: A new vector.
	 */
	static inline Vector3 Project(Vector3 a, Vector3 b);

	/**
	 * Returns a vector projected onto a plane orthogonal to "planeNormal".
	 * This can be visualized as the shadow of the vector onto the plane, if
	 * the light source were in the direction of the plane normal.
	 * @param vector: The vector to project.
	 * @param planeNormal: The normal of the plane onto which to project.
	 * @param: A new vector.
	 */
	static inline Vector3 ProjectOnPlane(Vector3 vector, Vector3 planeNormal);

	/**
	 * Returns a vector reflected off the plane orthogonal to the normal.
	 * The input vector is pointed inward, at the plane, and the return vector
	 * is pointed outward from the plane, like a beam of light hitting and then
	 * reflecting off a mirror.
	 * @param vector: The vector traveling inward at the plane.
	 * @param planeNormal: The normal of the plane off of which to reflect.
	 * @return: A new vector pointing outward from the plane.
	 */
	static inline Vector3 Reflect(Vector3 vector, Vector3 planeNormal);

	/**
	 * Returns the vector rejection of a on b.
	 * @param a: The target vector.
	 * @param b: The vector being projected onto.
	 * @return: A new vector.
	 */
	static inline Vector3 Reject(Vector3 a, Vector3 b);

	/**
	 * Rotates vector "current" towards vector "target" by "maxRadiansDelta".
	 * This treats the vectors as directions and will linearly interpolate
	 * between their magnitudes by "maxMagnitudeDelta". This function does not
	 * overshoot. If a negative delta is supplied, it will rotate away from
	 * "target" until it is pointing the opposite direction, but will not
	 * overshoot that either.
	 * @param current: The starting direction.
	 * @param target: The destination direction.
	 * @param maxRadiansDelta: The maximum number of radians to rotate.
	 * @param maxMagnitudeDelta: The maximum delta for magnitude interpolation.
	 * @return: A new vector.
	 */
	static inline Vector3 RotateTowards(Vector3 current, Vector3 target,
		float maxRadiansDelta,
		float maxMagnitudeDelta);

	/**
	 * Multiplies two vectors element-wise.
	 * @param a: The lhs of the multiplication.
	 * @param b: The rhs of the multiplication.
	 * @return: A new vector.
	 */
	static inline Vector3 Scale(Vector3 a, Vector3 b);

	/**
	 * Returns a vector rotated towards b from a by the percent t.
	 * Since interpolation is done spherically, the vector moves at a constant
	 * angular velocity. This rotation is clamped to 0 <= t <= 1.
	 * @param a: The starting direction.
	 * @param b: The ending direction.
	 * @param t: The interpolation value [0-1].
	 */
	static inline Vector3 Slerp(Vector3 a, Vector3 b, float t);

	/**
	 * Returns a vector rotated towards b from a by the percent t.
	 * Since interpolation is done spherically, the vector moves at a constant
	 * angular velocity. This rotation is unclamped.
	 * @param a: The starting direction.
	 * @param b: The ending direction.
	 * @param t: The interpolation value [0-1].
	 */
	static inline Vector3 SlerpUnclamped(Vector3 a, Vector3 b, float t);

	/**
	 * Returns the squared magnitude of a vector.
	 * This is useful when comparing relative lengths, where the exact length
	 * is not important, and much time can be saved by not calculating the
	 * square root.
	 * @param v: The vector in question.
	 * @return: A scalar value.
	 */
	static inline float SqrMagnitude(Vector3 v);

	/**
	 * Calculates the spherical coordinate space representation of a vector.
	 * This uses the ISO convention (radius r, inclination theta, azimuth phi).
	 * @param vector: The vector to convert.
	 * @param rad: The magnitude of the vector.
	 * @param theta: The angle in the XY plane from the X axis.
	 * @param phi: The angle from the positive Z axis to the vector.
	 */
	static inline void ToSpherical(Vector3 vector, float& rad, float& theta,
		float& phi);


	/**
	 * Operator overloading.
	 */
	inline struct Vector3& operator+=(const float rhs);
	inline struct Vector3& operator-=(const float rhs);
	inline struct Vector3& operator*=(const float rhs);
	inline struct Vector3& operator/=(const float rhs);
	inline struct Vector3& operator+=(const Vector3 rhs);
	inline struct Vector3& operator-=(const Vector3 rhs);
};

inline Vector3 operator-(Vector3 rhs);
inline Vector3 operator+(Vector3 lhs, const float rhs);
inline Vector3 operator-(Vector3 lhs, const float rhs);
inline Vector3 operator*(Vector3 lhs, const float rhs);
inline Vector3 operator/(Vector3 lhs, const float rhs);
inline Vector3 operator+(const float lhs, Vector3 rhs);
inline Vector3 operator-(const float lhs, Vector3 rhs);
inline Vector3 operator*(const float lhs, Vector3 rhs);
inline Vector3 operator/(const float lhs, Vector3 rhs);
inline Vector3 operator+(Vector3 lhs, const Vector3 rhs);
inline Vector3 operator-(Vector3 lhs, const Vector3 rhs);
inline bool operator==(const Vector3 lhs, const Vector3 rhs);
inline bool operator!=(const Vector3 lhs, const Vector3 rhs);



/*******************************************************************************
 * Implementation
 */

Vector3::Vector3() : X(0), Y(0), Z(0)
{
}
Vector3::Vector3(float data[]) : X(data[0]), Y(data[1]), Z(data[2])
{
}
Vector3::Vector3(float value) : X(value), Y(value), Z(value)
{
}
Vector3::Vector3(float x, float y) : X(x), Y(y), Z(0)
{
}
Vector3::Vector3(float x, float y, float z) : X(x), Y(y), Z(z)
{
}


Vector3 Vector3::Zero()
{
	return Vector3(0, 0, 0);
}
Vector3 Vector3::One()
{
	return Vector3(1, 1, 1);
}
Vector3 Vector3::Right()
{
	return Vector3(1, 0, 0);
}
Vector3 Vector3::Left()
{
	return Vector3(-1, 0, 0);
}
Vector3 Vector3::Up()
{
	return Vector3(0, 1, 0);
}
Vector3 Vector3::Down()
{
	return Vector3(0, -1, 0);
}
Vector3 Vector3::Forward()
{
	return Vector3(0, 0, 1);
}
Vector3 Vector3::Backward()
{
	return Vector3(0, 0, -1);
}


float Vector3::Angle(Vector3 a, Vector3 b)
{
	float v = Dot(a, b) / (Magnitude(a) * Magnitude(b));
	v = fmax(v, -1.0);
	v = fmin(v, 1.0);
	return acos(v);
}

Vector3 Vector3::ClampMagnitude(Vector3 vector, float maxLength)
{
	float length = Magnitude(vector);
	if (length > maxLength)
		vector *= maxLength / length;
	return vector;
}

float Vector3::Component(Vector3 a, Vector3 b)
{
	return Dot(a, b) / Magnitude(b);
}

Vector3 Vector3::Cross(Vector3 lhs, Vector3 rhs)
{
	float x = lhs.Y * rhs.Z - lhs.Z * rhs.Y;
	float y = lhs.Z * rhs.X - lhs.X * rhs.Z;
	float z = lhs.X * rhs.Y - lhs.Y * rhs.X;
	return Vector3(x, y, z);
}

float Vector3::Distance(Vector3 a, Vector3 b)
{
	return Vector3::Magnitude(a - b);
}

float Vector3::Dot(Vector3 lhs, Vector3 rhs)
{
	return lhs.X * rhs.X + lhs.Y * rhs.Y + lhs.Z * rhs.Z;
}

Vector3 Vector3::FromSpherical(float rad, float theta, float phi)
{
	Vector3 v;
	v.X = rad * sin(theta) * cos(phi);
	v.Y = rad * sin(theta) * sin(phi);
	v.Z = rad * cos(theta);
	return v;
}

Vector3 Vector3::Lerp(Vector3 a, Vector3 b, float t)
{
	if (t < 0) return a;
	else if (t > 1) return b;
	return LerpUnclamped(a, b, t);
}

Vector3 Vector3::LerpUnclamped(Vector3 a, Vector3 b, float t)
{
	return (b - a) * t + a;
}

float Vector3::Magnitude(Vector3 v)
{
	return sqrt(SqrMagnitude(v));
}

Vector3 Vector3::Max(Vector3 a, Vector3 b)
{
	float x = a.X > b.X ? a.X : b.X;
	float y = a.Y > b.Y ? a.Y : b.Y;
	float z = a.Z > b.Z ? a.Z : b.Z;
	return Vector3(x, y, z);
}

Vector3 Vector3::Min(Vector3 a, Vector3 b)
{
	float x = a.X > b.X ? b.X : a.X;
	float y = a.Y > b.Y ? b.Y : a.Y;
	float z = a.Z > b.Z ? b.Z : a.Z;
	return Vector3(x, y, z);
}

Vector3 Vector3::MoveTowards(Vector3 current, Vector3 target,
	float maxDistanceDelta)
{
	Vector3 d = target - current;
	float m = Magnitude(d);
	if (m < maxDistanceDelta || m == 0)
		return target;
	return current + (d * maxDistanceDelta / m);
}

Vector3 Vector3::Normalized(Vector3 v)
{
	float mag = Magnitude(v);
	if (mag == 0)
		return Vector3::Zero();
	return v / mag;
}

Vector3 Vector3::Orthogonal(Vector3 v)
{
	return v.Z < v.X ? Vector3(v.Y, -v.X, 0) : Vector3(0, -v.Z, v.Y);
}

void Vector3::OrthoNormalize(Vector3& normal, Vector3& tangent,
	Vector3& binormal)
{
	normal = Normalized(normal);
	tangent = ProjectOnPlane(tangent, normal);
	tangent = Normalized(tangent);
	binormal = ProjectOnPlane(binormal, tangent);
	binormal = ProjectOnPlane(binormal, normal);
	binormal = Normalized(binormal);
}

Vector3 Vector3::Project(Vector3 a, Vector3 b)
{
	float m = Magnitude(b);
	return Dot(a, b) / (m * m) * b;
}

Vector3 Vector3::ProjectOnPlane(Vector3 vector, Vector3 planeNormal)
{
	return Reject(vector, planeNormal);
}

Vector3 Vector3::Reflect(Vector3 vector, Vector3 planeNormal)
{
	return vector - 2 * Project(vector, planeNormal);
}

Vector3 Vector3::Reject(Vector3 a, Vector3 b)
{
	return a - Project(a, b);
}

Vector3 Vector3::RotateTowards(Vector3 current, Vector3 target,
	float maxRadiansDelta,
	float maxMagnitudeDelta)
{
	float magCur = Magnitude(current);
	float magTar = Magnitude(target);
	float newMag = magCur + maxMagnitudeDelta *
		((magTar > magCur) - (magCur > magTar));
	newMag = fmin(newMag, fmax(magCur, magTar));
	newMag = fmax(newMag, fmin(magCur, magTar));

	float totalAngle = Angle(current, target) - maxRadiansDelta;
	if (totalAngle <= 0)
		return Normalized(target) * newMag;
	else if (totalAngle >= M_PI)
		return Normalized(-target) * newMag;

	Vector3 axis = Cross(current, target);
	float magAxis = Magnitude(axis);
	if (magAxis == 0)
		axis = Normalized(Cross(current, current + Vector3(3.95, 5.32, -4.24)));
	else
		axis /= magAxis;
	current = Normalized(current);
	Vector3 newVector = current * cos(maxRadiansDelta) +
		Cross(axis, current) * sin(maxRadiansDelta);
	return newVector * newMag;
}

Vector3 Vector3::Scale(Vector3 a, Vector3 b)
{
	return Vector3(a.X * b.X, a.Y * b.Y, a.Z * b.Z);
}

Vector3 Vector3::Slerp(Vector3 a, Vector3 b, float t)
{
	if (t < 0) return a;
	else if (t > 1) return b;
	return SlerpUnclamped(a, b, t);
}

Vector3 Vector3::SlerpUnclamped(Vector3 a, Vector3 b, float t)
{
	float magA = Magnitude(a);
	float magB = Magnitude(b);
	a /= magA;
	b /= magB;
	float dot = Dot(a, b);
	dot = fmax(dot, -1.0);
	dot = fmin(dot, 1.0);
	float theta = acos(dot) * t;
	Vector3 relativeVec = Normalized(b - a * dot);
	Vector3 newVec = a * cos(theta) + relativeVec * sin(theta);
	return newVec * (magA + (magB - magA) * t);
}

float Vector3::SqrMagnitude(Vector3 v)
{
	return v.X * v.X + v.Y * v.Y + v.Z * v.Z;
}

void Vector3::ToSpherical(Vector3 vector, float& rad, float& theta,
	float& phi)
{
	rad = Magnitude(vector);
	float v = vector.Z / rad;
	v = fmax(v, -1.0);
	v = fmin(v, 1.0);
	theta = acos(v);
	phi = atan2(vector.Y, vector.X);
}


struct Vector3& Vector3::operator+=(const float rhs)
{
	X += rhs;
	Y += rhs;
	Z += rhs;
	return *this;
}

struct Vector3& Vector3::operator-=(const float rhs)
{
	X -= rhs;
	Y -= rhs;
	Z -= rhs;
	return *this;
}

struct Vector3& Vector3::operator*=(const float rhs)
{
	X *= rhs;
	Y *= rhs;
	Z *= rhs;
	return *this;
}

struct Vector3& Vector3::operator/=(const float rhs)
{
	X /= rhs;
	Y /= rhs;
	Z /= rhs;
	return *this;
}

struct Vector3& Vector3::operator+=(const Vector3 rhs)
{
	X += rhs.X;
	Y += rhs.Y;
	Z += rhs.Z;
	return *this;
}

struct Vector3& Vector3::operator-=(const Vector3 rhs)
{
	X -= rhs.X;
	Y -= rhs.Y;
	Z -= rhs.Z;
	return *this;
}

char Vector3::ToChar(Vector3 a)
{
	const char* x = (const char*)(int)a.X;
	const char* y = (const char*)(int)a.Y;
	const char* z = (const char*)(int)a.Z;
	char buffer[25];
	strncpy(buffer, x, sizeof(buffer));
	strncpy(buffer, ", ", sizeof(buffer));
	strncpy(buffer, y, sizeof(buffer));
	strncpy(buffer, ", ", sizeof(buffer));
	strncpy(buffer, z, sizeof(buffer));
	strncpy(buffer, ", ", sizeof(buffer));
	return buffer[24];
}

Vector3 operator-(Vector3 rhs)
{
	return rhs * -1;
}
Vector3 operator+(Vector3 lhs, const float rhs)
{
	return lhs += rhs;
}
Vector3 operator-(Vector3 lhs, const float rhs)
{
	return lhs -= rhs;
}
Vector3 operator*(Vector3 lhs, const float rhs)
{
	return lhs *= rhs;
}
Vector3 operator/(Vector3 lhs, const float rhs)
{
	return lhs /= rhs;
}
Vector3 operator+(const float lhs, Vector3 rhs)
{
	return rhs += lhs;
}
Vector3 operator-(const float lhs, Vector3 rhs)
{
	return rhs -= lhs;
}
Vector3 operator*(const float lhs, Vector3 rhs)
{
	return rhs *= lhs;
}
Vector3 operator/(const float lhs, Vector3 rhs)
{
	return rhs /= lhs;
}
Vector3 operator+(Vector3 lhs, const Vector3 rhs)
{
	return lhs += rhs;
}
Vector3 operator-(Vector3 lhs, const Vector3 rhs)
{
	return lhs -= rhs;
}

bool operator==(const Vector3 lhs, const Vector3 rhs)
{
	return lhs.X == rhs.X && lhs.Y == rhs.Y && lhs.Z == rhs.Z;
}

bool operator!=(const Vector3 lhs, const Vector3 rhs)
{
	return !(lhs == rhs);
}

#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

#define SMALL_float 0.0000000001

struct Quaternion
{
	union
	{
		struct
		{
			float X;
			float Y;
			float Z;
			float W;
		};
		float data[4];
	};


	/**
	 * Constructors.
	 */
	inline Quaternion();
	inline Quaternion(float data[]);
	inline Quaternion(Vector3 vector, float scalar);
	inline Quaternion(float x, float y, float z, float w);


	/**
	 * Constants for common quaternions.
	 */
	static inline Quaternion Identity();


	/**
	 * Returns the angle between two quaternions.
	 * The quaternions must be normalized.
	 * @param a: The first quaternion.
	 * @param b: The second quaternion.
	 * @return: A scalar value.
	 */
	static inline float Angle(Quaternion a, Quaternion b);

	/**
	 * Returns the conjugate of a quaternion.
	 * @param rotation: The quaternion in question.
	 * @return: A new quaternion.
	 */
	static inline Quaternion Conjugate(Quaternion rotation);

	/**
	 * Returns the dot product of two quaternions.
	 * @param lhs: The left side of the multiplication.
	 * @param rhs: The right side of the multiplication.
	 * @return: A scalar value.
	 */
	static inline float Dot(Quaternion lhs, Quaternion rhs);

	/**
	 * Creates a new quaternion from the angle-axis representation of
	 * a rotation.
	 * @param angle: The rotation angle in radians.
	 * @param axis: The vector about which the rotation occurs.
	 * @return: A new quaternion.
	 */
	static inline Quaternion FromAngleAxis(float angle, Vector3 axis);

	/**
	 * Create a new quaternion from the euler angle representation of
	 * a rotation. The z, x and y values represent rotations about those
	 * axis in that respective order.
	 * @param rotation: The x, y and z rotations.
	 * @return: A new quaternion.
	 */
	static inline Quaternion FromEuler(Vector3 rotation);

	/**
	 * Create a new quaternion from the euler angle representation of
	 * a rotation. The z, x and y values represent rotations about those
	 * axis in that respective order.
	 * @param x: The rotation about the x-axis in radians.
	 * @param y: The rotation about the y-axis in radians.
	 * @param z: The rotation about the z-axis in radians.
	 * @return: A new quaternion.
	 */
	static inline Quaternion FromEuler(float x, float y, float z);

	/**
	 * Create a quaternion rotation which rotates "fromVector" to "toVector".
	 * @param fromVector: The vector from which to start the rotation.
	 * @param toVector: The vector at which to end the rotation.
	 * @return: A new quaternion.
	 */
	static inline Quaternion FromToRotation(Vector3 fromVector,
		Vector3 toVector);

	/**
	 * Returns the inverse of a rotation.
	 * @param rotation: The quaternion in question.
	 * @return: A new quaternion.
	 */
	static inline Quaternion Inverse(Quaternion rotation);

	/**
	 * Interpolates between a and b by t, which is clamped to the range [0-1].
	 * The result is normalized before being returned.
	 * @param a: The starting rotation.
	 * @param b: The ending rotation.
	 * @return: A new quaternion.
	 */
	static inline Quaternion Lerp(Quaternion a, Quaternion b, float t);

	/**
	 * Interpolates between a and b by t. This normalizes the result when
	 * complete.
	 * @param a: The starting rotation.
	 * @param b: The ending rotation.
	 * @param t: The interpolation value.
	 * @return: A new quaternion.
	 */
	static inline Quaternion LerpUnclamped(Quaternion a, Quaternion b,
		float t);

	/**
	 * Creates a rotation with the specified forward direction. This is the
	 * same as calling LookRotation with (0, 1, 0) as the upwards vector.
	 * The output is undefined for parallel vectors.
	 * @param forward: The forward direction to look toward.
	 * @return: A new quaternion.
	 */
	static inline Quaternion LookRotation(Vector3 forward);

	/**
	 * Creates a rotation with the specified forward and upwards directions.
	 * The output is undefined for parallel vectors.
	 * @param forward: The forward direction to look toward.
	 * @param upwards: The direction to treat as up.
	 * @return: A new quaternion.
	 */
	static inline Quaternion LookRotation(Vector3 forward, Vector3 upwards);

	/**
	 * Returns the norm of a quaternion.
	 * @param rotation: The quaternion in question.
	 * @return: A scalar value.
	 */
	static inline float Norm(Quaternion rotation);

	/**
	 * Returns a quaternion with identical rotation and a norm of one.
	 * @param rotation: The quaternion in question.
	 * @return: A new quaternion.
	 */
	static inline Quaternion Normalized(Quaternion rotation);

	/**
	 * Returns a new Quaternion created by rotating "from" towards "to" by
	 * "maxRadiansDelta". This will not overshoot, and if a negative delta is
	 * applied, it will rotate till completely opposite "to" and then stop.
	 * @param from: The rotation at which to start.
	 * @param to: The rotation at which to end.
	 # @param maxRadiansDelta: The maximum number of radians to rotate.
	 * @return: A new Quaternion.
	 */
	static inline Quaternion RotateTowards(Quaternion from, Quaternion to,
		float maxRadiansDelta);

	/**
	 * Returns a new quaternion interpolated between a and b, using spherical
	 * linear interpolation. The variable t is clamped to the range [0-1]. The
	 * resulting quaternion will be normalized.
	 * @param a: The starting rotation.
	 * @param b: The ending rotation.
	 * @param t: The interpolation value.
	 * @return: A new quaternion.
	 */
	static inline Quaternion Slerp(Quaternion a, Quaternion b, float t);

	/**
	 * Returns a new quaternion interpolated between a and b, using spherical
	 * linear interpolation. The resulting quaternion will be normalized.
	 * @param a: The starting rotation.
	 * @param b: The ending rotation.
	 * @param t: The interpolation value.
	 * @return: A new quaternion.
	 */
	static inline Quaternion SlerpUnclamped(Quaternion a, Quaternion b,
		float t);

	/**
	 * Outputs the angle axis representation of the provided quaternion.
	 * @param rotation: The input quaternion.
	 * @param angle: The output angle.
	 * @param axis: The output axis.
	 */
	static inline void ToAngleAxis(Quaternion rotation, float& angle,
		Vector3& axis);

	/**
	 * Returns the Euler angle representation of a rotation. The resulting
	 * vector contains the rotations about the z, x and y axis, in that order.
	 * @param rotation: The quaternion to convert.
	 * @return: A new vector.
	 */
	static inline Vector3 ToEuler(Quaternion rotation);

	/**
	 * Operator overloading.
	 */
	inline struct Quaternion& operator+=(const float rhs);
	inline struct Quaternion& operator-=(const float rhs);
	inline struct Quaternion& operator*=(const float rhs);
	inline struct Quaternion& operator/=(const float rhs);
	inline struct Quaternion& operator+=(const Quaternion rhs);
	inline struct Quaternion& operator-=(const Quaternion rhs);
	inline struct Quaternion& operator*=(const Quaternion rhs);
};

inline Quaternion operator-(Quaternion rhs);
inline Quaternion operator+(Quaternion lhs, const float rhs);
inline Quaternion operator-(Quaternion lhs, const float rhs);
inline Quaternion operator*(Quaternion lhs, const float rhs);
inline Quaternion operator/(Quaternion lhs, const float rhs);
inline Quaternion operator+(const float lhs, Quaternion rhs);
inline Quaternion operator-(const float lhs, Quaternion rhs);
inline Quaternion operator*(const float lhs, Quaternion rhs);
inline Quaternion operator/(const float lhs, Quaternion rhs);
inline Quaternion operator+(Quaternion lhs, const Quaternion rhs);
inline Quaternion operator-(Quaternion lhs, const Quaternion rhs);
inline Quaternion operator*(Quaternion lhs, const Quaternion rhs);
inline Vector3 operator*(Quaternion lhs, const Vector3 rhs);
inline bool operator==(const Quaternion lhs, const Quaternion rhs);
inline bool operator!=(const Quaternion lhs, const Quaternion rhs);



/*******************************************************************************
 * Implementation
 */

Quaternion::Quaternion() : X(0), Y(0), Z(0), W(1)
{
}
Quaternion::Quaternion(float data[]) : X(data[0]), Y(data[1]), Z(data[2]),
W(data[3])
{
}
Quaternion::Quaternion(Vector3 vector, float scalar) : X(vector.X),
Y(vector.Y), Z(vector.Z), W(scalar)
{
}
Quaternion::Quaternion(float x, float y, float z, float w) : X(x), Y(y),
Z(z), W(w)
{
}


Quaternion Quaternion::Identity()
{
	return Quaternion(0, 0, 0, 1);
}


float Quaternion::Angle(Quaternion a, Quaternion b)
{
	float dot = Dot(a, b);
	return acos(fmin(fabs(dot), 1)) * 2;
}

Quaternion Quaternion::Conjugate(Quaternion rotation)
{
	return Quaternion(-rotation.X, -rotation.Y, -rotation.Z, rotation.W);
}

float Quaternion::Dot(Quaternion lhs, Quaternion rhs)
{
	return lhs.X * rhs.X + lhs.Y * rhs.Y + lhs.Z * rhs.Z + lhs.W * rhs.W;
}

Quaternion Quaternion::FromAngleAxis(float angle, Vector3 axis)
{
	Quaternion q;
	float m = sqrt(axis.X * axis.X + axis.Y * axis.Y + axis.Z * axis.Z);
	float s = sin(angle / 2) / m;
	q.X = axis.X * s;
	q.Y = axis.Y * s;
	q.Z = axis.Z * s;
	q.W = cos(angle / 2);
	return q;
}

Quaternion Quaternion::FromEuler(Vector3 rotation)
{
	return FromEuler(rotation.X, rotation.Y, rotation.Z);
}

Quaternion Quaternion::FromEuler(float x, float y, float z)
{
	float cx = cos(x * 0.5);
	float cy = cos(y * 0.5);
	float cz = cos(z * 0.5);
	float sx = sin(x * 0.5);
	float sy = sin(y * 0.5);
	float sz = sin(z * 0.5);
	Quaternion q;
	q.X = cx * sy * sz + cy * cz * sx;
	q.Y = cx * cz * sy - cy * sx * sz;
	q.Z = cx * cy * sz - cz * sx * sy;
	q.W = sx * sy * sz + cx * cy * cz;
	return q;
}

Quaternion Quaternion::FromToRotation(Vector3 fromVector, Vector3 toVector)
{
	float dot = Vector3::Dot(fromVector, toVector);
	float k = sqrt(Vector3::SqrMagnitude(fromVector) *
		Vector3::SqrMagnitude(toVector));
	if (fabs(dot / k + 1) < 0.00001)
	{
		Vector3 ortho = Vector3::Orthogonal(fromVector);
		return Quaternion(Vector3::Normalized(ortho), 0);
	}
	Vector3 cross = Vector3::Cross(fromVector, toVector);
	return Normalized(Quaternion(cross, dot + k));
}

Quaternion Quaternion::Inverse(Quaternion rotation)
{
	float n = Norm(rotation);
	return Conjugate(rotation) / (n * n);
}

Quaternion Quaternion::Lerp(Quaternion a, Quaternion b, float t)
{
	if (t < 0) return Normalized(a);
	else if (t > 1) return Normalized(b);
	return LerpUnclamped(a, b, t);
}

Quaternion Quaternion::LerpUnclamped(Quaternion a, Quaternion b, float t)
{
	Quaternion quaternion;
	if (Dot(a, b) >= 0)
		quaternion = a * (1 - t) + b * t;
	else
		quaternion = a * (1 - t) - b * t;
	return Normalized(quaternion);
}

Quaternion Quaternion::LookRotation(Vector3 forward)
{
	return LookRotation(forward, Vector3(0, 1, 0));
}

Quaternion Quaternion::LookRotation(Vector3 forward, Vector3 upwards)
{
	// Normalize inputs
	forward = Vector3::Normalized(forward);
	upwards = Vector3::Normalized(upwards);
	// Don't allow zero vectors
	if (Vector3::SqrMagnitude(forward) < SMALL_float || Vector3::SqrMagnitude(upwards) < SMALL_float)
		return Quaternion::Identity();
	// Handle alignment with up direction
	if (1 - fabs(Vector3::Dot(forward, upwards)) < SMALL_float)
		return FromToRotation(Vector3::Forward(), forward);
	// Get orthogonal vectors
	Vector3 right = Vector3::Normalized(Vector3::Cross(upwards, forward));
	upwards = Vector3::Cross(forward, right);
	// Calculate rotation
	Quaternion quaternion;
	float radicand = right.X + upwards.Y + forward.Z;
	if (radicand > 0)
	{
		quaternion.W = sqrt(1.0 + radicand) * 0.5;
		float recip = 1.0 / (4.0 * quaternion.W);
		quaternion.X = (upwards.Z - forward.Y) * recip;
		quaternion.Y = (forward.X - right.Z) * recip;
		quaternion.Z = (right.Y - upwards.X) * recip;
	}
	else if (right.X >= upwards.Y && right.X >= forward.Z)
	{
		quaternion.X = sqrt(1.0 + right.X - upwards.Y - forward.Z) * 0.5;
		float recip = 1.0 / (4.0 * quaternion.X);
		quaternion.W = (upwards.Z - forward.Y) * recip;
		quaternion.Z = (forward.X + right.Z) * recip;
		quaternion.Y = (right.Y + upwards.X) * recip;
	}
	else if (upwards.Y > forward.Z)
	{
		quaternion.Y = sqrt(1.0 - right.X + upwards.Y - forward.Z) * 0.5;
		float recip = 1.0 / (4.0 * quaternion.Y);
		quaternion.Z = (upwards.Z + forward.Y) * recip;
		quaternion.W = (forward.X - right.Z) * recip;
		quaternion.X = (right.Y + upwards.X) * recip;
	}
	else
	{
		quaternion.Z = sqrt(1.0 - right.X - upwards.Y + forward.Z) * 0.5;
		float recip = 1.0 / (4.0 * quaternion.Z);
		quaternion.Y = (upwards.Z + forward.Y) * recip;
		quaternion.X = (forward.X + right.Z) * recip;
		quaternion.W = (right.Y - upwards.X) * recip;
	}
	return quaternion;
}

float Quaternion::Norm(Quaternion rotation)
{
	return sqrt(rotation.X * rotation.X +
		rotation.Y * rotation.Y +
		rotation.Z * rotation.Z +
		rotation.W * rotation.W);
}

Quaternion Quaternion::Normalized(Quaternion rotation)
{
	return rotation / Norm(rotation);
}

Quaternion Quaternion::RotateTowards(Quaternion from, Quaternion to,
	float maxRadiansDelta)
{
	float angle = Quaternion::Angle(from, to);
	if (angle == 0)
		return to;
	maxRadiansDelta = fmax(maxRadiansDelta, angle - M_PI);
	float t = fmin(1, maxRadiansDelta / angle);
	return Quaternion::SlerpUnclamped(from, to, t);
}

Quaternion Quaternion::Slerp(Quaternion a, Quaternion b, float t)
{
	if (t < 0) return Normalized(a);
	else if (t > 1) return Normalized(b);
	return SlerpUnclamped(a, b, t);
}

Quaternion Quaternion::SlerpUnclamped(Quaternion a, Quaternion b, float t)
{
	float n1;
	float n2;
	float n3 = Dot(a, b);
	bool flag = false;
	if (n3 < 0)
	{
		flag = true;
		n3 = -n3;
	}
	if (n3 > 0.999999)
	{
		n2 = 1 - t;
		n1 = flag ? -t : t;
	}
	else
	{
		float n4 = acos(n3);
		float n5 = 1 / sin(n4);
		n2 = sin((1 - t) * n4) * n5;
		n1 = flag ? -sin(t * n4) * n5 : sin(t * n4) * n5;
	}
	Quaternion quaternion;
	quaternion.X = (n2 * a.X) + (n1 * b.X);
	quaternion.Y = (n2 * a.Y) + (n1 * b.Y);
	quaternion.Z = (n2 * a.Z) + (n1 * b.Z);
	quaternion.W = (n2 * a.W) + (n1 * b.W);
	return Normalized(quaternion);
}

void Quaternion::ToAngleAxis(Quaternion rotation, float& angle, Vector3& axis)
{
	if (rotation.W > 1)
		rotation = Normalized(rotation);
	angle = 2 * acos(rotation.W);
	float s = sqrt(1 - rotation.W * rotation.W);
	if (s < 0.00001)
	{
		axis.X = 1;
		axis.Y = 0;
		axis.Z = 0;
	}
	else
	{
		axis.X = rotation.X / s;
		axis.Y = rotation.Y / s;
		axis.Z = rotation.Z / s;
	}
}

Vector3 Quaternion::ToEuler(Quaternion rotation)
{
	float sqw = rotation.W * rotation.W;
	float sqx = rotation.X * rotation.X;
	float sqy = rotation.Y * rotation.Y;
	float sqz = rotation.Z * rotation.Z;
	// If normalized is one, otherwise is correction factor
	float unit = sqx + sqy + sqz + sqw;
	float test = rotation.X * rotation.W - rotation.Y * rotation.Z;
	Vector3 v;
	// Singularity at north pole
	if (test > 0.4995f * unit)
	{
		v.Y = 2 * atan2(rotation.Y, rotation.X);
		v.X = M_PI_2;
		v.Z = 0;
		return v;
	}
	// Singularity at south pole
	if (test < -0.4995f * unit)
	{
		v.Y = -2 * atan2(rotation.Y, rotation.X);
		v.X = -M_PI_2;
		v.Z = 0;
		return v;
	}
	// Yaw
	v.Y = atan2(2 * rotation.W * rotation.Y + 2 * rotation.Z * rotation.X,
		1 - 2 * (rotation.X * rotation.X + rotation.Y * rotation.Y));
	// Pitch
	v.X = asin(2 * (rotation.W * rotation.X - rotation.Y * rotation.Z));
	// Roll
	v.Z = atan2(2 * rotation.W * rotation.Z + 2 * rotation.X * rotation.Y,
		1 - 2 * (rotation.Z * rotation.Z + rotation.X * rotation.X));
	return v;
}

struct Quaternion& Quaternion::operator+=(const float rhs)
{
	X += rhs;
	Y += rhs;
	Z += rhs;
	W += rhs;
	return *this;
}

struct Quaternion& Quaternion::operator-=(const float rhs)
{
	X -= rhs;
	Y -= rhs;
	Z -= rhs;
	W -= rhs;
	return *this;
}

struct Quaternion& Quaternion::operator*=(const float rhs)
{
	X *= rhs;
	Y *= rhs;
	Z *= rhs;
	W *= rhs;
	return *this;
}

struct Quaternion& Quaternion::operator/=(const float rhs)
{
	X /= rhs;
	Y /= rhs;
	Z /= rhs;
	W /= rhs;
	return *this;
}

struct Quaternion& Quaternion::operator+=(const Quaternion rhs)
{
	X += rhs.X;
	Y += rhs.Y;
	Z += rhs.Z;
	W += rhs.W;
	return *this;
}

struct Quaternion& Quaternion::operator-=(const Quaternion rhs)
{
	X -= rhs.X;
	Y -= rhs.Y;
	Z -= rhs.Z;
	W -= rhs.W;
	return *this;
}

struct Quaternion& Quaternion::operator*=(const Quaternion rhs)
{
	Quaternion q;
	q.W = W * rhs.W - X * rhs.X - Y * rhs.Y - Z * rhs.Z;
	q.X = X * rhs.W + W * rhs.X + Y * rhs.Z - Z * rhs.Y;
	q.Y = W * rhs.Y - X * rhs.Z + Y * rhs.W + Z * rhs.X;
	q.Z = W * rhs.Z + X * rhs.Y - Y * rhs.X + Z * rhs.W;
	*this = q;
	return *this;
}

Quaternion operator-(Quaternion rhs)
{
	return rhs * -1;
}
Quaternion operator+(Quaternion lhs, const float rhs)
{
	return lhs += rhs;
}
Quaternion operator-(Quaternion lhs, const float rhs)
{
	return lhs -= rhs;
}
Quaternion operator*(Quaternion lhs, const float rhs)
{
	return lhs *= rhs;
}
Quaternion operator/(Quaternion lhs, const float rhs)
{
	return lhs /= rhs;
}
Quaternion operator+(const float lhs, Quaternion rhs)
{
	return rhs += lhs;
}
Quaternion operator-(const float lhs, Quaternion rhs)
{
	return rhs -= lhs;
}
Quaternion operator*(const float lhs, Quaternion rhs)
{
	return rhs *= lhs;
}
Quaternion operator/(const float lhs, Quaternion rhs)
{
	return rhs /= lhs;
}
Quaternion operator+(Quaternion lhs, const Quaternion rhs)
{
	return lhs += rhs;
}
Quaternion operator-(Quaternion lhs, const Quaternion rhs)
{
	return lhs -= rhs;
}
Quaternion operator*(Quaternion lhs, const Quaternion rhs)
{
	return lhs *= rhs;
}

Vector3 operator*(Quaternion lhs, const Vector3 rhs)
{
	Vector3 u = Vector3(lhs.X, lhs.Y, lhs.Z);
	float s = lhs.W;
	return u * (Vector3::Dot(u, rhs) * 2)
		+ rhs * (s * s - Vector3::Dot(u, u))
		+ Vector3::Cross(u, rhs) * (2.0 * s);
}

bool operator==(const Quaternion lhs, const Quaternion rhs)
{
	return lhs.X == rhs.X &&
		lhs.Y == rhs.Y &&
		lhs.Z == rhs.Z &&
		lhs.W == rhs.W;
}

bool operator!=(const Quaternion lhs, const Quaternion rhs)
{
	return !(lhs == rhs);
}

struct Ray
{
	Vector3 m_vOrigin;
	Vector3 m_vDirection;
};

struct RaycastHit
{
	float px;
	float py;
	float pz;
	float nx;
	float ny;
	float nz;
	unsigned int face_id;
	float distance;
	float u;
	float v;
	int collider;
};

#pragma warning(pop)