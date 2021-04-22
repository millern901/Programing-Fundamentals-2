//
// Created by Nicholas Miller on 2/28/18.
//

#ifndef PA2_PA2_H
#define PA2_PA2_H

#include "string"

// This method gets the name of a program to be deleted
std::string get_program_name();

// This is a method for the loop that executes within my program
int program_loop(std::string algorithm);

// This struct represents information passed for a new program
struct ProgramInfo {
    std::string program_name;
    int program_size;

    ProgramInfo(std::string name, int size) {
        program_name = name;
        program_size = size;
    }
};

// This method obtains the information of a program to be added
ProgramInfo get_program_info();

// This struct represent a chunk of free memory
struct FreeMemoryChunk {
    int start_page, end_page;

    FreeMemoryChunk(int start, int end) {
        start_page = start;
        end_page = end;
    }
};

// This struct represent a chunk of used memory
struct UsedMemoryChunk : FreeMemoryChunk {
    std::string program_name;

    UsedMemoryChunk(int start, int end, std::string name):
            FreeMemoryChunk(start, end) {
        start_page = start;
        end_page = end;
        program_name = name;
    }
};

// This template class represents the nodes in my Linked List
template <class T>
class Node {
private:
    T data;
    Node<T> *next;

public:
    // constructor
    Node(T data);

    // getters
    T get_data();
    Node<T>* get_next();

    // setters
    void set_data(T data);
    void set_next(Node<T>* node);
};

// This template class represents my Linked List
template <class T>
class LinkedList {
private:
    Node<T> *head;

public:
    // constructor
    LinkedList();

    // getter
    Node<T>* get_head();

    // setter
    void set_head(Node<T>* node);

    // adds a node to the end of the Linked List
    void emplace_back(T data);
};

// This class represent my memory allocator
class MemoryAllocator {
private:
    std::string algorithm;
    LinkedList<FreeMemoryChunk> free_mem;
    LinkedList<UsedMemoryChunk> used_mem;

public:
    MemoryAllocator(std::string algorithm, int size);

    // This method determine, which chunk of free memory is a viable fit for a new program.
    // If there is no such chunk, then the method halts; otherwise, it creates a new used memory chunk and
    // adds it to used_mem. Also the original free memory chunk is split with respect to the size of the program.
    void add_program(ProgramInfo program_info);

    // This method kills a program currently running by deleting the program from used_mem then adding the
    // newly freed memory to free_mem in its respective position. Then it defragments free_mem.
    void kill_program(std::string program_name);

    // This method defragments chunks within free_mem by combining nodes, which share a bound.
    void defragment();

    // This method counts the number of free memory chunks within free_mem.
    void fragment_count();

    // This method prints out either "Free" or the name of a program currently running according
    // to its respective position in memory in a 4X8 block.
    void print_memory();
};

#endif //PA2_PA2_H
