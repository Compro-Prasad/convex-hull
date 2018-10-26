#include <algorithm>
#include <math.h>
#include "circular_doubly_linked_list.hpp"
#include "angle.hpp"

#define SWAP(x, y, type) {							\
		type z = x;									\
		x = y, y = z;								\
	}

template <typename T>
class convex_hull : public CDLlist< point< T > >
{
	point<T> mid_pt;

public:
	convex_hull() : mid_pt(0, 0) {}

	int8_t compare_by_angle_from_vertical(const point<T> p1, const point<T> p2) const {
		angle<T> a1(0, DEG), a2(0, DEG);
		a1.angle_from_vertical(this->mid_pt, p1);
		a2.angle_from_vertical(this->mid_pt, p2);
		if (abs(p2.x - this->mid_pt.x) < 0e-5 || abs(p2.x - this->mid_pt.x) >= 1e-5) {
			if (p2.x < this->mid_pt.x)
				a2 = -a2 + a2.one_rotation();
			if (p1.x < this->mid_pt.x)
				a1 = -a1 + a1.one_rotation();
		}
		return a1 == a2 ? 0 : a1 > a2 ? 1 : -1;
	}

	void insert(point<T> p) {

		if (this->n == 0 || (this->n == 1 && this->rear->value != p)) {
			this->push(p);
			return;
		}

		point<T> p1 = this->rear->value;
		point<T> p2 = this->rear->next->value;
		Dlink<point<T> > *newNode;

		if (this->n == 2) {
			triangle<T> t(p1, p2, p);
			if (t.area() == 0) {
				/* The 3 points are in the same line 
				 * So there cannot be more than 2 points
				 * covering the region of 3 points.
				 */
				T p_rear = p.distance(p1);
				T p_rear_next = p.distance(p2);
				T rear_rear_next = p1.distance(p2);
				point<T> to_be_pushed;
				if (p_rear > p_rear_next) {
					if (p_rear > rear_rear_next) {
						this->remove(p2);
						this->push(p);
					}
				} else if (p_rear_next > rear_rear_next) {
						this->remove(p1);
						this->push(p);
				}
			}

			else {
				/* The case, when for the first time we get 3 points 
				 * So we first find the centroid of the triangle 
				 * and set it to 'mid_pt' which will further be used
				 * as a point inside the convex hull 
				 */
				this->mid_pt = t.centroid();
				int8_t a, b, c, f = 1;

			recalculate:
				/* The previous two points may not be sorted */
				a = this->compare_by_angle_from_vertical(p, p1);
				b = this->compare_by_angle_from_vertical(p, p2);
				c = this->compare_by_angle_from_vertical(p1, p2);

				if (c > 0 && f) {
					/* So we first sort them if necessary */
					f = 0; // prevent infinite looping due to goto
					SWAP(this->rear, this->front, Dlink<point<T> > *);
					SWAP(p1, p2, point<T>);
					goto recalculate;
				}

				/* then push point 'p' accordingly */
				if (a < 0)
					this->push(p);
				else if (b < 0) {
					newNode = new Dlink<point<T> >(p);
					newNode->join_after(&this->rear);
					++this->n;
				} else {
					newNode = new Dlink<point<T> >(p);
					newNode->join_after(&this->front);
					this->front = newNode;
					++this->n;
				}
			}
		}

		else if (this->compare_by_angle_from_vertical(p, this->rear->value) < 0)
			/* Push 'p' normally if it forms the least angle from vertical */
			this->push(p);

		else if (this->compare_by_angle_from_vertical(p, this->front->value) > 0) {
			/* Push 'p' after front if it forms the max angle from vertical */
			newNode = new Dlink<point<T> >(p);
			newNode->join_after(&this->front);
			this->front = newNode;
			++this->n;
		}

		else {
			/* Find the specific position for point 'p' */
			Dlink<point<T> > **ptr = &this->rear->next;
			int8_t check = this->compare_by_angle_from_vertical(p, (*ptr)->value);
			while (*ptr != this->rear && check < 0) {
				ptr = &(*ptr)->next;
				check = this->compare_by_angle_from_vertical(p, (*ptr)->value);
			}
			if (*ptr) {
				if (check > 0) {
					newNode = new Dlink<point<T> >(p);
					newNode->join_after(ptr);
					++this->n;
				} else if (check == 0 &&
						 p.distance(this->mid_pt) > (*ptr)->value.distance(this->mid_pt)) {
					newNode = new Dlink<point<T> >(p);
					newNode->replace_on(ptr);
					++this->n;
				}
			}
		}
	}

	void eliminate() {
		if (this->n > 2) {
			T d1, d2, d3;
			Dlink<point<T> > **x = &this->rear,
				**rx = &this->rear->next,
				**rrx = &this->rear->next->next;
			while (*rx != this->rear) {
				d1 = (*x)->value.distance(this->mid_pt);
				d2 = (*rx)->value.distance(this->mid_pt);
				d3 = (*rrx)->value.distance(this->mid_pt);
				if (d2 < d1 && d2 < d3) {
					this->remove_node(rx);
					rx = x;
					x = &(*x)->prev;
				} else
					x = rx, rx = rrx, rrx = &(*rrx)->next;
			}
		}
	}
};

int main(int argc, char* argv[]) {

	FILE *ifp, *ofp;

	if(argc < 2){
		fprintf(stderr, "Error: We Expect atleast one argument, as Input text file.\n"
			"Usage - You provide Input text file as argument, you can also give output file "
			"in which you want to save output, if you don't provide any output file then "
			"result is shown in STDOUT.\n");
		exit(1);
	}

	ifp = freopen(argv[1], "r", stdin);

	if (ifp == NULL) {
		fprintf(stderr, "Can't open input file %s\n", argv[1]);
		exit(1);
	}

	if(argc > 2)
		freopen(argv[2], "w+", stdout);

	convex_hull<double> list;

	long long int no_of_points;
	if(scanf("%lld",&no_of_points) != 1)
	{
		fprintf(stderr, "File is malformed. It should have two space separated real numbers per line.\n"
			"First line denoting no. of points(N)\n"
			"Following N lines containing two space separated real numbers denoting "
			"x and y coordinates respectively.\n"
			"Example :\n"
			"2\n"
			"1 1\n"
			"2 2\n");
		exit(1);
	}
	while(no_of_points--)
	{
		double xx, yy;

		if(scanf("%lf",&xx) != 1)
		{
			fprintf(stderr, "Error, please enter a valid number.\n");
			exit(1);
		}
		if(scanf("%lf",&yy) != 1)
		{
			fprintf(stderr, "Error, please enter a valid number.\n");
			exit(1);
		}
		point<double> p1;
		p1.x = xx;
		p1.y = yy;
		list.insert(p1);
	}

	list.display_forwards();
	list.eliminate();
}
