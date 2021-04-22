//
// Created by Nicholas Miller on 2/28/18.
//

#include <iostream>
#include <iomanip>
#include <cmath>
#include "pa2.h"

int main(int argc, char** argv) {
    if (argv[1] != NULL) {
        std::string algorithm = argv[1];
        if (algorithm == "best" || algorithm == "worst") {
            return program_loop(algorithm);
        }
    }
    std::cout << "The First argument needs to be either best or worst";
    return 0;
}

// Node Template methods
template <class T>
Node<T>::Node(T data): data(data), next(NULL) {}

template <class T>
T Node<T>::get_data() {
    return data;
}

template <class T>
Node<T>* Node<T>::get_next() {
    return next;
}

template <class T>
void Node<T>::set_data(T data) {
    this->data = data;
}

template <class T>
void Node<T>::set_next(Node<T> *node) {
    next = node;
}


// LinkedList Template methods
template <class T>
LinkedList<T>::LinkedList() {
    head = NULL;
}

template <class T>
Node<T>* LinkedList<T>::get_head() {
    return head;
}

template <class T>
void LinkedList<T>::set_head(Node<T> *node) {
    head = node;
}

template <class T>
void LinkedList<T>::emplace_back(T data) {
    if (head == NULL) {
        head = new Node<T>(data);
        return;
    }
    Node<T> *temp = head;
    while (temp->get_next() != NULL) {
        temp = temp->get_next();
    }
    temp->set_next(new Node<T>(data));
}


// MemoryAllocator methods
MemoryAllocator::MemoryAllocator(std::string algorithm, int size) {
    this->algorithm = algorithm;
    std::cout << "Using the " << algorithm << " fit algorithm.\n\n";
    free_mem = LinkedList<FreeMemoryChunk>();
    free_mem.emplace_back(FreeMemoryChunk(0, size - 1));
    used_mem = LinkedList<UsedMemoryChunk>();
}

void MemoryAllocator::add_program(ProgramInfo program_info) {
    // checks if the size of the program is valid
    if (program_info.program_size <= 0) {
        std::cout << "Invalid size\n\n";
        return;
    }

    // checks whether or not the program is already running
    Node<UsedMemoryChunk> *used_current = used_mem.get_head();
    while (used_current != NULL) {
        if (used_current->get_data().program_name == program_info.program_name) {
            std::cout << "Error, Program " << program_info.program_name << " is already running.\n\n";
            return;
        } else {
            used_current = used_current->get_next();
        }
    }

    Node<FreeMemoryChunk> *free_current = free_mem.get_head();
    int num_pages = ceil(program_info.program_size / 4.0);
    int chunk_size, chunk_index = -1, index_count = 0;

    // Best Fit algorithm
    if (algorithm == "best") {
        chunk_size = 33;

        while (free_current != NULL) {
            int curr_chunk_size = free_current->get_data().end_page - free_current->get_data().start_page + 1;
            if (curr_chunk_size < chunk_size && curr_chunk_size >= num_pages) {
                chunk_size = curr_chunk_size;
                chunk_index = index_count;
                free_current = free_current->get_next();
                index_count++;
            } else {
                free_current = free_current->get_next();
                index_count++;
            }
        }
    }
        // Worst Fit algorithm
    else {
        chunk_size = -1;

        while (free_current != NULL) {
            int curr_chunk_size = free_current->get_data().end_page - free_current->get_data().start_page + 1;
            if (curr_chunk_size > chunk_size && curr_chunk_size >= num_pages) {
                chunk_size = curr_chunk_size;
                chunk_index = index_count;
                free_current = free_current->get_next();
                index_count++;
            } else {
                free_current = free_current->get_next();
                index_count++;
            }
        }
    }

    // returns if no valid chunk exists
    if (chunk_index == -1) {
        std::cout << "Error, Not enough memory for Program " << program_info.program_name << ".\n\n";
        return;
    }

    // sets a pointer to the correct node
    free_current = free_mem.get_head();
    Node<FreeMemoryChunk> *free_prev = NULL;
    for (int i = chunk_index; i > 0; i--) {
        free_prev = free_current;
        free_current = free_current->get_next();
    }

    // creates a new chunk of used memory
    UsedMemoryChunk new_program_info(free_current->get_data().start_page,
                                     free_current->get_data().start_page + num_pages - 1,
                                     program_info.program_name);
    Node<UsedMemoryChunk> *new_program = new Node<UsedMemoryChunk>(new_program_info);

    // splits the node in free_mem
    if (chunk_size == num_pages) {
        if (free_prev == NULL) {
            free_mem.set_head(free_current->get_next());
        } else {
            free_prev->set_next(free_current->get_next());
        }
    } else {
        FreeMemoryChunk split_free_mem(free_current->get_data().start_page + (int)num_pages,
                                       free_current->get_data().end_page);
        free_current->set_data(split_free_mem);
    }

    used_current = used_mem.get_head();
    Node<UsedMemoryChunk>* used_prev = used_mem.get_head();

    // adds the program to the front of the list
    if (used_current != NULL &&
        new_program->get_data().start_page < used_current->get_data().start_page)  {
        new_program->set_next(used_current);
        used_mem.set_head(new_program);
        std::cout << "Program " << new_program_info.program_name
                  << " added successfully, " << num_pages
                  << " page(s) used.\n\n";
        return;
    }

    // adds the program to the middle of the list
    while (used_current != NULL) {
        if (new_program->get_data().start_page < used_current->get_data().start_page) {
            used_prev->set_next(new_program);
            new_program->set_next(used_current);
            std::cout << "Program " << new_program->get_data().program_name
                      << " added successfully, " << num_pages
                      << " page(s) used.\n\n";
            return;
        } else {
            used_prev = used_current;
            used_current = used_current->get_next();
        }
    }

    // adds the program to the end of the list
    used_mem.emplace_back(new_program->get_data());
    std::cout << "Program " << new_program_info.program_name
              << " added successfully, " << num_pages
              << " page(s) used.\n\n";
}

void MemoryAllocator::kill_program(std::string program_name) {
    bool found = false;
    Node<UsedMemoryChunk> *used_current = used_mem.get_head();
    Node<UsedMemoryChunk> *used_prev = NULL;

    // searches for program and deletes it from used memory
    while (used_current != NULL && !found) {
        if (used_current->get_data().program_name == program_name) {
            if (used_prev != NULL) {
                used_prev->set_next(used_current->get_next());
            } else {
                used_mem.set_head(used_current->get_next());
            }
            found = true;
        } else {
            used_prev = used_current;
            used_current = used_current->get_next();
        }
    }

    // if the program isn't running return
    if (!found) {
        std::cout <<  "Error, Program" << program_name << " is not running.\n\n";
        return;
    }

    // creates a new free chunk
    FreeMemoryChunk freed_mem_chunk(used_current->get_data().start_page, used_current->get_data().end_page);
    Node<FreeMemoryChunk> *freed_chunk = new Node<FreeMemoryChunk>(freed_mem_chunk);

    // deallocate used memory and add it to a null free memory linked list
    if (free_mem.get_head() == NULL) {
        free_mem.set_head(freed_chunk);
        std::cout << program_name << " deleted. "
                  << (freed_chunk->get_data().end_page - freed_chunk->get_data().start_page + 1)
                  << " page(s) reclaimed.\n\n";
        return;
    }

    // if there are still chunks in the list
    Node<FreeMemoryChunk> *free_current = free_mem.get_head();
    Node<FreeMemoryChunk> *free_prev = NULL;
    while (free_current != NULL) {
        if (freed_chunk->get_data().start_page < free_current->get_data().start_page) {
            freed_chunk->set_next(free_current);
            if (free_prev == NULL) {
                free_mem.set_head(freed_chunk);
            } else {
                free_prev->set_next(freed_chunk);
            }
            defragment();
            std::cout << program_name << " successfully killed, "
                      << (freed_chunk->get_data().end_page - freed_chunk->get_data().start_page + 1)
                      << " page(s) reclaimed.\n\n";
            return;
        } else {
            free_prev = free_current;
            free_current = free_current->get_next();
        }
    }
}

void MemoryAllocator::defragment() {
    Node<FreeMemoryChunk> *free_current = free_mem.get_head();
    Node<FreeMemoryChunk> *free_prev = NULL;
    while (free_current != NULL) {
        if (free_prev == NULL) {
            free_prev = free_current;
            free_current = free_current->get_next();
        } else {
            if (free_prev->get_data().end_page + 1 == free_current->get_data().start_page) {
                FreeMemoryChunk defragmented_chunk(free_prev->get_data().start_page,
                                                   free_current->get_data().end_page);
                free_prev->set_data(defragmented_chunk);
                free_prev->set_next(free_current->get_next());
                return;
            } else {
                free_prev = free_current;
                free_current = free_current->get_next();
            }
        }
    }
}

void MemoryAllocator::fragment_count() {
    Node<FreeMemoryChunk> *free_current = free_mem.get_head();
    int fragment_count = 0;
    while (free_current != NULL) {
        free_current = free_current->get_next();
        fragment_count++;
    }
    std::cout << "There are " << fragment_count << " Fragment(s).\n\n" ;
}

void MemoryAllocator::print_memory() {
    Node<UsedMemoryChunk> *used_current = used_mem.get_head();
    int index_count = 0;
    std::string memory_pages[32];

    if (used_current == NULL) {
        while (index_count < 32) {
            memory_pages[index_count] = "Free";
            index_count++;
        }
    } else {
        while (used_current != NULL) {
            int diff = used_current->get_data().start_page - index_count;
            for(int i = diff; i > 0; i--) {
                memory_pages[index_count] = "Free";
                index_count++;
            }
            for (int i = used_current->get_data().end_page - used_current->get_data().start_page + 1; i > 0; i--) {
                memory_pages[index_count] = used_current->get_data().program_name;
                index_count++;
            }
            used_current = used_current->get_next();
        }
    }
    while (index_count < 32) {
        memory_pages[index_count] = "Free";
        index_count++;
    }

    // prints the array
    for (int i = 0; i < 32; i++) {
        if (i % 8 == 0) std::cout << "\n";
        std::cout << std::left << std::setw(6) << memory_pages[i] << " ";
    }
    std::cout << "\n\n";
}


// methods used to aid the program
std::string get_program_name() {
    std::string name;
    std::cout << "Program name - ";
    std::cin >> name;
    return name;
}

ProgramInfo get_program_info() {
    std::string name = get_program_name();
    int size;
    std::cout << "Program size (KB) - ";
    try {
        std::cin >> size;
    } catch (std::exception e){
        std::cout << "Invalid program size";
        std::cin.clear();
        exit(0);
    }
    return ProgramInfo(name, size);
}

int program_loop(std::string algorithm) {
    MemoryAllocator mem_alloc(algorithm, 32);

    // program choices
    std::cout << "1. Add program\n"
              << "2. Kill program\n"
              << "3. Fragmentation\n"
              << "4. Print memory\n"
              << "5. Exit\n\n";

    int choice = 0;
    while (choice != 5) {
        std::cout << "Choice - ";
        try {
            std::cin >> choice;
        } catch (std::exception e) {
            std::cout << "Invalid choice";
            std::cin.clear();
            exit(0);
        }

        switch (choice) {
            case 1:
                mem_alloc.add_program(get_program_info());
                break;
            case 2:
                mem_alloc.kill_program(get_program_name());
                break;
            case 3:
                mem_alloc.fragment_count();
                break;
            case 4:
                mem_alloc.print_memory();
                break;
            case 5:
                break;
            default:
                std::cout << "Invalid choice\n";
                return 0;
        }
    }
    return 0;
}