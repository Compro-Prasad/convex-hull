#include <stdio.h>
#include <math.h>
#include <stdint.h>

#define DEG 0
#define RAD 1
#define PI M_PI

template <typename T>
struct point
{
	T x, y;

	point() {
		this->x = this->y = 0;
	}

	point(T x, T y) {
		this->x = x;
		this->y = y;
	}

	void print() {
		printf("%lf, %lf\n", this->x, this->y);
	}

	point operator+(point<T> p) {
		point<T> p1(this->x + p.x, this->y + p.y);
		return p1;
	}

	point operator-(point<T> p) {
		point<T> p1(this->x - p.x, this->y - p.y);
		return p1;
	}

	point& operator/(const T times) {
		this->x /= times;
		this->y /= times;
		return *this;
	}

	void operator+=(point<T> p) {
		this->x += p.x;
		this->y += p.y;
	}

	void operator-=(point<T> p) {
		this->x -= p.x;
		this->y -= p.y;
	}

	bool operator==(point<T> &p) const {
		return this->x == p.x && this->y == p.y;
	}

	bool operator!=(point<T> &p) const {
		return this->x != p.x || this->y != p.y;
	}

	T slope(const point p) const {
		return (this->y - p.y) / (this->x - p.x);
	}

	T distance(const point p) const {
		return sqrtl(powl(this->y - p.y, 2) + powl(this->x - p.x, 2));
	}
};

template <typename T>
class angle
{
	T value;
	uint8_t type;
public:
	angle() {
		this->value = 0;
		this->type = RAD;
	}

	angle(const angle<T> &x) {
		this->value = x.value;
		this->type = x.type;
	}

	angle(T value) {
		this->value = fmodl(value, 2*PI);
		this->type = RAD;
	}

	angle(point<T> p1, point<T> p2, point<T> p3) {
		this->type = RAD;
		this->to_angle(p1, p2, p3);
	}

	angle(uint8_t type, point<T> p1, point<T> p2, point<T> p3) {
		this->type = type;
		this->to_angle(p1, p2, p3);
	}

	angle(T value, uint8_t type) {
		this->value = value;
		this->type = type;
	}

	T half_rotation() const {
		return this->type == RAD ? PI : 180.0;
	}

	T one_rotation() {
		return this->type == RAD ? 2*PI : 360.0;
	}

	angle& angle_from_vertical(const point<T> p1, const point<T> p2) {
		//this->type = RAD;
		point<T> arbitrary(p1.x, p1.y - 1); // point just below p1
		*this = this->to_angle(arbitrary, p1, p2).to_positive_normal();
		return *this;
	}

	angle& to_degree() {
		if (this->type == RAD) {
			this->type = DEG;
			this->value = this->value * 180.0 / PI;
		}
		return *this;
	}

	angle& to_radian() {
		if (this->type == DEG) {
			this->type = RAD;
			this->value = this->value * PI / 180.0;
		}
		return *this;
	}

	angle& to_angle(point<T> p1, point<T> p2, point<T> p3) {
		T x = p1.distance(p2), y = p2.distance(p3), z = p3.distance(p1);
		this->value = acosl((x*x + y*y - z*z) / (2*x*y));
		this->value *= (this->type == DEG ? 180.0 / PI : 1.0);
		return *this;
	}

	angle to_convex() {
		angle xyz;
		xyz.type = this->type;
		xyz.value =	fmodl(this->value < 0 ? -this->value : this->value, xyz.one_rotation());
		xyz.value =	this->value > xyz.half_rotation() ?
			xyz.one_rotation() - xyz.value : xyz.value;
		return xyz;
	}

	angle to_concave() {
		angle xyz;
		xyz.type = this->type;
		if (this->value < 0)
			xyz.value = fmodl(-this->value, xyz.one_rotation());
		xyz.value = this->value <= this->half_rotation() ?
			this->one_rotation() - this->value : this->value;
		return xyz;
	}

	angle to_positive_normal() {
		angle xyz;
		xyz.type = this->type;
		if (this->value < 0)
			xyz.value = xyz.one_rotation() + this->value;
		else
			xyz.value = this->value;
		return xyz;
	}

	angle to_normal() {
		angle xyz;
		xyz.type = this->type;
		if (this->value > xyz.half_rotation())
			xyz.value = -xyz.one_rotation() + this->value;
		else
			xyz.value = this->value;
		return xyz;
	}

	angle operator-(const angle<T> x) const {
		angle xyz = *this;
		xyz.value = fmodl(xyz.value - x.value, xyz.one_rotation());
		return xyz;
	}

	angle operator-() const {
		angle<T> xyz = *this;
		xyz.value = -xyz.value;
		return xyz;
	}

	angle operator-=(const angle<T> x) {
		this->value = fmodl(this->value - x.value, this->one_rotation());
		return *this;
	}

	angle operator-(const T x) const {
		angle xyz = *this;
		xyz.value = fmodl(xyz.value - x, xyz.one_rotation());
		return xyz;
	}

	angle operator-=(const T x) {
		this->value = fmodl(this->value - x, this->one_rotation());
		return *this;
	}

	angle operator+(const angle<T> x) const {
		angle xyz = *this;
		xyz.value = fmodl(xyz.value + x.value, xyz.one_rotation());
		return xyz;
	}

	angle operator+=(const angle<T> x) {
		this->value = fmodl(this->value + x.value, this->one_rotation());
		return *this;
	}

	angle operator+(const T x) const {
		angle xyz = *this;
		xyz.value = fmodl(xyz.value + x, xyz.one_rotation());
		return xyz;
	}

	angle operator+=(const T x) {
		this->value = fmodl(this->value + x, this->one_rotation());
		return *this;
	}

	angle& operator=(const angle &x) {
		this->value = x.value;
		this->type = x.type;
		return *this;
	}

	angle& operator=(const T x) {
		this->value = fmodl(x, this->type == DEG ? 360 : 2*PI);
		return *this;
	}

	bool operator==(angle<T> x) const {
		if (this->type == x.type)
			return abs(this->value - x.value) >= 0e-5 && abs(this->value - x.value) < 1e-5;
		return (this->type == RAD ? x.to_radian().value : x.to_degree().value) == this->value;
	}

	bool operator!=(angle<T> x) const {
		if (this->type == x.type)
			return this->value != x.value;
		return (this->type == RAD ? x.to_radian().value : x.to_degree().value) != this->value;
	}

	bool operator<(angle<T> x) const {
		if (this->type == x.type)
			return this->value < x.value;
		return (this->type == RAD ? x.to_radian().value : x.to_degree().value) < this->value;
	}

	bool operator>(angle<T> x) const {
		if (this->type == x.type)
			return this->value > x.value;
		return (this->type == RAD ? x.to_radian().value : x.to_degree().value) > this->value;
	}

	bool operator<=(angle<T> x) const {
		if (this->type == x.type)
			return this->value <= x.value;
		return (this->type == RAD ? x.to_radian().value : x.to_degree().value) <= this->value;
	}

	bool operator>=(angle<T> x) const {
		if (this->type == x.type)
			return this->value >= x.value;
		return (this->type == RAD ? x.to_radian().value : x.to_degree().value) >= this->value;
	}

	T       angleValue() const { return this->value; }
	uint8_t angleType()  const { return this->type; }
};

template <typename T>
class triangle
{
	point<T> A, B, C;
public:
	triangle() {
		this->A = this->B = this->C = 0;
	}

	triangle(point<T> a, point<T> b, point<T> c) {
		this->A = a;
		this->B = b;
		this->C = c;
	}

	angle<T> angleA() {
		angle<T> a(this->B, this->A, this->C);
		return a.to_convex();
	}

	angle<T> angleB() {
		angle<T> a(this->B, this->A, this->C);
		return a.to_convex();
	}

	angle<T> angleC() {
		angle<T> a(this->B, this->A, this->C);
		return a.to_convex();
	}

	point<T> centroid() {
		point<T> G;
		G = (this->A + this->B + this->C) / 3;
		return G;
	}

	T area() {
		return (
			this->A.x * (this->C.y - this->B.y) +
			this->B.x * (this->A.y - this->C.y) +
			this->C.x * (this->B.y - this->A.y)) / 2;
	}
};

// int main()
// {
// 	point<double> p(3, 1), p1(2, 3), p2(1, 3), mid(2, 2), m(2, 1);
// 	angle<double> a(0, DEG);
// 	printf("%lf,%lf,%lf\n",
// 		   a.to_angle(m, mid, p).angleValue(),
// 		   a.to_angle(m, mid, p1).angleValue(),
// 		   a.to_angle(m, mid, p2).angleValue());
// 	return 0;
// }
