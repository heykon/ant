#ifndef INTERACTION_H
#define INTERACTION_H

#include <cstdint>
#include <fstream>
#include <string>
#include <stdio.h>

#define interact_id int


template <class Entry_t> class Interaction_system
{
private:
	Entry_t* id_table;
	int num_id;

	uint8_t* occupancy_map;
	int width, height;

public:
	Interaction_system(int size_i, int w, int h);
	Interaction_system(int size_i, std::string bitmap_file); // flip before read
	~Interaction_system();

	Entry_t access_obj_at_pos(int x, int y);
	Entry_t access_obj_by_id(int id);
	void insert_obj(int p, Entry_t e);

	int get_id(int x, int y);
	void modify(int x, int y, int v);

	int get_width(){return width;}
	int get_height(){return height;}
};

template <class Entry_t>
Interaction_system<Entry_t>::Interaction_system(int size_i, int w, int h)
	: width(w)
	, height(h)
	, num_id(size_i)
{
	id_table = new Entry_t[size_i];
	occupancy_map = new uint8_t[w*h]();

	
}

inline uint32_t reverse(char* b, int pos)
{
	return b[pos] | (b[pos+1] << 8) | (b[pos+2] << 16) | (b[pos+3] << 24);
}

template <class Entry_t>
Interaction_system<Entry_t>::Interaction_system(int size_i, std::string bitmap_file)
{
	id_table = new Entry_t[size_i];

	std::ifstream bitmap(bitmap_file);
	char buffer[27];
	bitmap.read(buffer, 26);

	uint32_t offset = reverse(buffer, 10);
	width = reverse(buffer, 18);
	height = reverse(buffer, 22);

	occupancy_map = new uint8_t[width*height];

	bitmap.seekg(offset);
	bitmap.read((char*)occupancy_map, width*height);

	
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
	int id = occupancy_map[x + y*width];
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
	return occupancy_map[x + y*width];
}


template <class Entry_t>
void Interaction_system<Entry_t>::modify(int x, int y, int v)
{
	occupancy_map[x + y*width] = v;
}



#endif