// Description: An incomplete implementation of iterators for a RingQueue class.
// Notes: The project DOES compile but there is no meaningful output]
// Your job: To complete this set of classes so that the output produced by 'main' (below), matches the sample file provided at the end of this file.

#include <iostream>

// Forward declaration
template <typename ItemType, int MAX_SIZE>
class RingQueue;

template <typename ItemType, int MAX_SIZE>
class RingQueue {
	// Nested Forward declaration of RingQueue<ItemType,MAX_SIZE>::iterator. This is needed if one plans to turn this home-made iterator 
	//into one of the special categories of iterators (e.g., input, output, forward, etc.).
public:
	class iterator;
	
	// Aliases
	typedef ItemType* pointer;
	typedef ItemType& reference;
	
	// Definition of RingQueue<ItemType,MAX_SIZE>::iterator
public:
	class iterator {
	private:
		RingQueue* parent; //Pointer to parent container
		int offset; //Position within Ring Queue is det by how far ahead we are from beg of queue

	private:  // Private constructor
		iterator(RingQueue* _parent, int _offset = 0) : parent(_parent), offset(_offset) { }
		friend class RingQueue<ItemType, MAX_SIZE>; // It is quite common for Containers and their iterators to be friends. After all, they should work closely together.
		
	public:
		reference operator*() {
			return (parent -> buffer[(((parent->begin_index) + offset) % MAX_SIZE)]); 
		}
		iterator& operator++() {
			if (offset == parent->ring_size)
			{
				offset = 0; 
			}
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
			if ((parent == rhs.parent) || (offset != rhs.offset))
			{
				return true; 
			}
			else return false; 
		}
	};

	/*
	class const_iterator{
	private:
		RingQueue* parent; //Pointer to parent container
		int offset; //Position within Ring Queue is det by how far ahead we are from beg of queue

	private:
	// Only RingQueue objects can create const_iterators...
		const_iterator();

	public:	// ... however, const_iterators can be 'copied'.
		const_iterator(const const_iterator& a)
		{
			iterator copy = a.parent; 
			parent = copy; 
			offset = a.offset; 
		}

	friend class RingQueue<ItemType,MAX_SIZE>;
	};
	*/

	friend class iterator; //Friendship goes both ways here.
	//friend class const_iterator;  // not implemented... yet.

private:
	ItemType buffer[MAX_SIZE]; // A fixed-size static array with constant capacity that represents the RingQueue
		
	int begin_index; // The starting index. It changes according to a very specific set of rules (below).
	int ring_size; // The actual size of the RingQueue. Not to be confused with its capacity.
	
	// A helper function that computes the index of 'the end' of the RingQueue
	int end_index() const {
		return ((begin_index + ring_size) % MAX_SIZE); 
	}
	
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
		buffer[end_index()] = value;

		/*
		if (ring_size == MAX_SIZE)
		{
			size_t temp = (++begin_index) % MAX_SIZE; 
			begin_index = end_index = temp; 
		}
		else (ring_size < MAX_SIZE)
		{
			end_index() = (end_index() + 1) % size; 
			++ring_size; 
		}
		*/
		
		if (ring_size < MAX_SIZE) //if ring_size is less than ring_capacity, then pushback value and increase the curr ring_size
		{
			++ring_size;
			//end_index() = (end_index() + 1) % MAX_SIZE; //BRENT
		}
		else //if ring_size is equal to ring_capacity, then pushback value and inc the begin_index (to get rid of oldest value) 
		{
			//check if begin_index has not reached ring_capacity...then just increment begin_index
			if (begin_index < MAX_SIZE)
			{
				//size_t temp = (++begin_index) % MAX_SIZE;
				//start = end = temp?? 
				++begin_index;
			}
			else //if it has reached MAX_SIZE, then reset back to 0
			{
				begin_index = 0; 
			}
		}
		
		return;
	}
	void pop_front() {
		/*
		if (ring_size == 0)
		{
			throw; 
		}
		else
		{
			--ring_size; 
			begin_index = (begin_index < (MAX_SIZE - 1)) ? begin_index + 1 : 0; 
		}
		*/
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

	// Debugging functions
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

	//RingQueue<int, 7>::iterator it = rq.begin(); 
	auto it = rq.begin();
	for (size_t i = 0; i < rq.size(); ++i) {
		std::cout << "Value: " << *it << ", address: " << &(*it) << '\n';
		++it;
	}
	std::cout << '\n';

	// Uncomment the block below only when you have a working implementation of RingQueue<ItemType,int>::end(). If the implementation is not correct, it might result in an infinite loop.
	std::cout << "Queue via iterators: \n";
	for ( auto it = rq.begin() ; it != rq.end() ; ++it ) 
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
It is dependent on the device where this driver is executed. However, it should be clear that the difference between consecutive memory addresses is equal to the number reported by 'size_of( int )'.
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