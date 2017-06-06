#include <iostream>

template <typename ItemType, int MAX_SIZE>
class RingQueue;

template <typename ItemType, int MAX_SIZE>
//RQ Class
class RingQueue {
public:
	class iterator;

	// Aliases
	typedef ItemType* pointer;
	typedef ItemType& reference;

public:
	//ITERATOR class
	class iterator {
	
	//PRIVATE VARIABLES
	private:
		RingQueue* parent;
		int offset; //Position within RQ is det by how far ahead we are from beg of queue

	//PRIVATE FUNCTIONS/CLASSES 
	private:  // Private constructor
		iterator(RingQueue* _parent, int _offset = 0) : parent(_parent), offset(_offset) { }
		friend class RingQueue<ItemType, MAX_SIZE>; 

	//PUBLIC FUNCTIONS 
	public:
		reference operator*() {
			return (parent->buffer[((parent->begin_index) + offset) % MAX_SIZE]);
		}
		iterator& operator++() {
			//if the offset is equal to the current ring_size, then that means the begin_index is at 0, which means offset needs to be at 0 as well 
			if (offset == MAX_SIZE) 
			{
				offset = 0;
			}
			//otherwise, just increment offset 
			else 
			{
				++offset;
			}
			return *this;
		}
		iterator operator++(int unused) {
			iterator clone(*this); //making copy
			++(*this);
			return clone;
		}
		bool operator==(const iterator& rhs) const {
			if (this != rhs)
			{
				return false;
			}
			else return true;
		}
		bool operator!=(const iterator& rhs) const {
			if ((parent != rhs.parent) || (offset != rhs.offset))
			{
				return true;
			}
			else return false;
		}
	};
	
	//do i need this? just return const ref

	//CONST ITERATOR CLASS
	/*
	//Only RingQueue objects can create const_iterators...however, const_iterators can be 'copied'.
	class const_iterator{
	//CONST ITERATOR PRIVATE VARIABLES
	private:
	RingQueue* parent;
	int offset; 

	//CONST ITERATOR PRIVATE CONSTRUCTOR 
	private: 
	const_iterator();
	
	//CONST ITERATOR PUBLIC 
	public:
	const_iterator(const const_iterator& a)
	{
		iterator copy = a.parent;
		parent = copy;
		offset = a.offset;
	}

	friend class RingQueue<ItemType,MAX_SIZE>;
	};

	friend class const_iterator;  // not implemented... yet.
	*/

	friend class iterator; //Friendship goes both ways here.
	
//PRIVATE VARIABLES OF RQ
private:
	ItemType buffer[MAX_SIZE]; //A fixed-size static array with constant capacity that represents the RQ

	int begin_index; 
	int ring_size; 

	// A helper function that computes the index of 'the end' of the RQ
	int end_index() const {
		/*
		For example: 
		If begin_index is 0 and the ring_size is 0, we % this by 7 and get 0. 
		If begin_index is 0 and the ring_size is 1, we % this by 7 and get 1. 
		If begin_index is 0 and the ring_size is 2, we % this by 7 and get 2.
		If begin_index is 1 and the ring_size is 2, we % this by 7 and get 3. 
		*/
		return ((begin_index + ring_size) % MAX_SIZE);
	}

//PUBLIC FUNTIONS OF RQ
public:
	// Constructor
	RingQueue() : begin_index(0), ring_size(0) { }

	// Accessors. Note: 'back()' is not considered part of the array.
	ItemType front() const {
		if (ring_size == 0)
		{
			std::cerr << "Warning: Empty ring!\n";
		}
		return buffer[begin_index];
	}
	ItemType back() const {
		if (ring_size == 0) std::cerr << "Warning: Empty ring!\n";
		return buffer[end_index];
	}

	// Mutators
	void push_back(const ItemType& value) {
		/*
		if something pushed back: 
		begin_index stays the same UNLESS the ring_size is equal to the max_size
			if it is in this condition, then we increment begin_index 
		ring_size increases UNLESS the ring_size is equal to the max_size 
			if in this condition, it stays the same
		end_index increases UNTIL it is 
		*/
		
		buffer[end_index()] = value;
		
		if (ring_size == MAX_SIZE) //meaning that we've reached the capacity
		{
			++begin_index;
			//ring_size is the same if we've hit MAX_SIZE
			
			//need to set end_index back to 0 
		}
		else
		{
			//begin_index same 
			++ring_size; 

			//need to increment end_index 
		}
		return;
	}
	void pop_front() {
		//(begin_index+1)	% MAX_SIZE
		++begin_index;
		--ring_size;
		return;
	}

	// Functions that return iterators
	iterator begin() {
		return iterator(this, 0);
	}
	iterator end() {
		return iterator(this, ring_size);
	}

	// Miscellaneous functions
	size_t size() const {
		return ring_size;
	}

	// Debugging functions aka COUT function
	void dump_queue() const {
		std::cout << "Raw queue...\n";
		for (size_t i = 0; i < MAX_SIZE; ++i)
		{
			std::cout << "Val: " << buffer[i] << ", at: " << buffer + i << '\n';
		}
		std::cout << '\n';
		return;
	}
};

int main() {
	RingQueue<int, 7> rq;
	rq.dump_queue();

	for (int i = 0; i < 8; ++i)
		rq.push_back(i + 1);

	rq.dump_queue();
	rq.pop_front();

	std::cout << "Queue via size: \n";

	auto it = rq.begin();
	for (size_t i = 0; i < rq.size(); ++i) {
		std::cout << "Value: " << *it << ", address: " << &(*it) << '\n';
		++it;
	}
	std::cout << '\n';

	std::cout << "Queue via iterators: \n";
	for (auto it = rq.begin(); it != rq.end(); ++it)
	{
		std::cout << "Value: " << *it << ", address: " << &(*it) << '\n';
	}
	std::cout << '\n';

	rq.dump_queue();

	system("pause");
	return 0;
}

/**
+++++++++++++++++++++++++++++++++++++++++++++++
The output of your program [once the missing code is added] should look more or less like the output below.

Note:
It is dependent on the device where this driver is executed. 
However, it should be clear that the difference between consecutive memory addresses is 
equal to the number reported by 'size_of( int )'.
+++++++++++++++++++++++++++++++++++++++++++++++

Raw queue...
Val: 2, at: 0x7ffcdeeaab40
Val: 0, at: 0x7ffcdeeaab44
Val: 4198285, at: 0x7ffcdeeaab48
Val: 0, at: 0x7ffcdeeaab4c
Val: 0, at: 0x7ffcdeeaab50
Val: 0, at: 0x7ffcdeeaab54
Val: 0, at: 0x7ffcdeeaab58

Raw queue...
Val: 8, at: 0x7ffcdeeaab40
Val: 2, at: 0x7ffcdeeaab44
Val: 3, at: 0x7ffcdeeaab48
Val: 4, at: 0x7ffcdeeaab4c
Val: 5, at: 0x7ffcdeeaab50
Val: 6, at: 0x7ffcdeeaab54
Val: 7, at: 0x7ffcdeeaab58

Queue via size:
Value: 3, address: 0x7ffcdeeaab48
Value: 4, address: 0x7ffcdeeaab4c
Value: 5, address: 0x7ffcdeeaab50
Value: 6, address: 0x7ffcdeeaab54
Value: 7, address: 0x7ffcdeeaab58
Value: 8, address: 0x7ffcdeeaab40

Queue via iterators:
Value: 3, address: 0x7ffcdeeaab48
Value: 4, address: 0x7ffcdeeaab4c
Value: 5, address: 0x7ffcdeeaab50
Value: 6, address: 0x7ffcdeeaab54
Value: 7, address: 0x7ffcdeeaab58
Value: 8, address: 0x7ffcdeeaab40

Raw queue...
Val: 8, at: 0x7ffcdeeaab40
Val: 2, at: 0x7ffcdeeaab44
Val: 3, at: 0x7ffcdeeaab48
Val: 4, at: 0x7ffcdeeaab4c
Val: 5, at: 0x7ffcdeeaab50
Val: 6, at: 0x7ffcdeeaab54
Val: 7, at: 0x7ffcdeeaab58
*/