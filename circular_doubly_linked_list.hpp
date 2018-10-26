#include <stdio.h>

#define CDLlistDisplay(ptr, n) {										\
		if (this->rear) {												\
			Dlink<T> *t = ptr;											\
			do {														\
				t->value.print();										\
				t = n;													\
			} while (t != ptr);											\
		}																\
}

template <typename T>
struct Dlink
{
	T value;
	Dlink<T> *next, *prev;

	Dlink() {
		this->value = 0;
		this->next = this->prev = NULL;
	}

	Dlink(T value) {
		this->value = value;
		this->next = this->prev = NULL;
	}

	Dlink(T value, Dlink<T> *next, Dlink<T> *prev) {
		this->value = value;
		this->next = next;
		this->prev = prev;
	}

	void join_after(Dlink<T> **ptr) {
		// 'this' is the newly created Node
		this->next         = (*ptr)->next;
		this->prev         = *ptr;
		(*ptr)->next->prev = this;
		(*ptr)->next       = this;
	}

	void join_before(Dlink<T> **ptr) {
		// 'this' is the newly created Node
		this->next         = *ptr;
		this->prev         = (*ptr)->prev;
		(*ptr)->prev->next = this;
		(*ptr)->prev       = this;
	}

	void replace_on(Dlink<T> **ptr) {
		Dlink<T> *z = *ptr;
		this->next         = (*ptr)->next;
		this->prev         = (*ptr)->prev;
		(*ptr)->prev->next = this;
		(*ptr)->next->prev = this;
		*ptr               = this;
		delete z;
	}
};

template <typename T>
class CDLlist
{
protected:
	Dlink<T> *front, *rear;
	size_t n;

	void remove_node(Dlink<T> **t) {
		if (*t) {
			Dlink<T> *z = *t;
			if (this->rear != this->front) {
				(*t)->prev->next = (*t)->next;
				(*t)->next->prev = (*t)->prev;
				if (this->rear == *t)
					this->rear  = this->rear->next;
				if (this->front == *t)
					this->front = this->front->prev;
			} else
				this->rear = this->front = NULL;
			delete z;
			--this->n;
		}
	}

	Dlink<T> **find_node(T element) {
		Dlink<T> **t = &this->rear;
		while (*t && (*t)->value != element)
			t = &(*t)->next;
		return t;
	}

public:
	CDLlist() {
		this->front = this->rear = NULL;
		this->n = 0;
	}

	~CDLlist() {
		this->deallocate();
	}

	void push(T x) {
		Dlink<T> *tmp = new Dlink<T>(x);
		tmp->next = this->rear ? this->rear : tmp;
		if (this->rear)
			this->rear->prev = tmp;
		this->front = this->rear ? this->front : tmp;
		this->rear = tmp;
		tmp->prev = this->front;
		this->front->next = tmp;
		++this->n;
	}

	void push_on(T x, T y) { // push x just above y
		if (!this->n)
			throw "No elements in the list\n";
		else if (this->rear->value == y)
			this->push(x);
		else {
			Dlink<T> **ptr = &this->rear->next;
			while (*ptr != this->rear && (*ptr)->value == y)
				ptr = &(*ptr)->next;
			if (*ptr) {
				Dlink<T> *newNode  = new Dlink<T>(x);
				newNode->next      = (*ptr)->prev->next;
				newNode->prev      = (*ptr)->prev;
				(*ptr)->prev->next = newNode;
				(*ptr)->prev       = newNode;
			}
		}
	}

	void pop() {
		if (this->rear) {
			Dlink<T> *t = this->rear;
			if (this->rear != this->front) {
				this->rear = this->rear->next;
				this->rear->prev = this->front;
				this->front->next = this->rear;
			} else
				this->rear = this->front = NULL;
			delete t;
			--this->n;
		}
	}

	void remove(T element) {
		this->remove_node(this->find_node(element));/*
		if (this->rear) {
			Dlink<T> **t = &this->rear;
			while (*t && (*t)->value != element)
				t = &(*t)->next;
			if (*t) {
				Dlink<T> *z = *t;
				if (this->rear != this->front) {
					(*t)->prev->next = (*t)->next;
					(*t)->next->prev = (*t)->prev;
					if (this->rear == *t)
						this->rear  = this->rear->next;
					if (this->front == *t)
						this->front = this->front->prev;
				} else
					this->rear = this->front = NULL;
				delete z;
			}
			}*/
	}

	void display_forwards() {
		CDLlistDisplay(this->rear, t->next);
	}

	void display_backwards() {
		CDLlistDisplay(this->front, t->prev);
	}

	void deallocate() {
		if (this->rear) {
			Dlink<T> *t, *z = this->rear;
			do {
				t = this->rear;
				this->rear = this->rear->next;
				delete t;
			} while (this->rear != z);
			this->rear = this->front = NULL;
		}
	}

	size_t node_count() {
		return this->n;
	}

	size_t memory_consumption() {
		return this->n * sizeof(T);
	}

	bool isEmpty() {
		return !this->rear;
	}
};

// int main()
// {
// 	CDLlist<long double> list;
// 	list.push(1);
// 	list.push(2);
// 	list.push(3);
// 	list.push(4);
// 	list.push(5);
// 	list.remove(1);
// 	list.display_forwards();
// }
