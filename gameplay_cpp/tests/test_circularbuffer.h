// Test.cpp

#include "godot_cpp/classes/packed_scene.hpp"
#include "godot_cpp/variant/utility_functions.hpp"
#include "godot_cpp/classes/resource_loader.hpp"

#include "SFT.hpp"
#include "utils/circularbuffer.h"

#ifdef TESTS_ENABLED

void test_circular_buffer() {
    CircularBuffer<int> buffer(5,0);
    for (int i=0; i<5; i++) {
        buffer.push(i);
    }
    TESTS(
        "Small circular buffer",
        buffer.get(0)==4, buffer.get(1)==3, buffer.get(2)==2, buffer.get(3)==1, buffer.get(4)==0,
        buffer.get_absolute(0)==0, buffer.get_absolute(1)==1, buffer.get_absolute(2)==2, buffer.get_absolute(3)==3, buffer.get_absolute(4)==4
    )
    buffer.push(5); buffer.push(6); buffer.push(7);
    TESTS(
        "Small circular buffer with overflow",
        buffer.get(0)==7, buffer.get(1)==6, buffer.get(2)==5, buffer.get(3)==4, buffer.get(4)==3, 
        buffer.get(5)==7, buffer.get(6)==6, buffer.get(7)==5,
        buffer.get_absolute(0)==5, buffer.get_absolute(1)==6, buffer.get_absolute(2)==7, buffer.get_absolute(3)==3, buffer.get_absolute(4)==4
    )
    buffer.push(8); buffer.push(9);
    int temp[10];
    for (int i=0; i<10; i++) {
        temp[i]=buffer.pop();
    }
    TESTS(
        "Small circular buffer with overflow and popping",
        temp[0]==9, temp[1]==8, temp[2]==7, temp[3]==6, temp[4]==5,
        temp[5]==9, temp[6]==8, temp[7]==7, temp[8]==6, temp[9]==5
    )
}

void do_tests() {
    test_circular_buffer();
}

#endif // TESTS_ENABLED