#ifndef INTERACTION_H
#define INTERACTION_H

#define interact_id int

template <class Entry_t> class Interaction_system
{
private:
	Entry_t* id_table;
	int num_id;

	int* occupancy_map;
	const int WIDTH, HEIGHT;

public:
	Interaction_system(int size_i, int w, int h);
	~Interaction_system();

	Entry_t access_obj_at_pos(int x, int y);
	Entry_t access_obj_by_id(int id);
	void insert_obj(int p, Entry_t e);

	int get_id(int x, int y);
	void modify(int x, int y, int v);

	const int get_width(){return WIDTH;}
	const int get_height(){return HEIGHT;}
};

template <class Entry_t>
Interaction_system<Entry_t>::Interaction_system(int size_i, int w, int h)
	: WIDTH(w)
	, HEIGHT(h)
	, num_id(size_i)
{
	id_table = new Entry_t[size_i];
	occupancy_map = new int[w*h]();

	
}


template <class Entry_t>
Interaction_system<Entry_t>::~Interaction_system()
{
	delete [] id_table;
	delete [] occupancy_map;

	
}



template <class Entry_t>
Entry_t Interaction_system<Entry_t>::access_obj_at_pos(int x, int y)
{
	int id = occupancy_map[x + y*WIDTH];
	return id_table[id];
}


template <class Entry_t>
Entry_t Interaction_system<Entry_t>::access_obj_by_id(int id)
{
	return id_table[id];
}


template <class Entry_t>
void Interaction_system<Entry_t>::insert_obj(int p, Entry_t e)
{
	id_table[p] = e;
}


template <class Entry_t>
int Interaction_system<Entry_t>::get_id(int x, int y)
{
	return occupancy_map[x + y*WIDTH];
}


template <class Entry_t>
void Interaction_system<Entry_t>::modify(int x, int y, int v)
{
	occupancy_map[x + y*WIDTH] = v;
}



#endif