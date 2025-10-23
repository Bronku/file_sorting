<<<<<<< HEAD
// the tests are ai generated, with prompt based on the heap.h file
=======
// the tests are ai generated
>>>>>>> 00717dd038b2c024b529553b5c2a848fceccf2fb
#include "heap.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// Comparator for integers
int compare_ints(const void* a, const void* b)
{
    int ia = *(const int*)a;
    int ib = *(const int*)b;
    return (ia > ib) - (ia < ib);
}

// Comparator for doubles
int compare_doubles(const void* a, const void* b)
{
    double da = *(const double*)a;
    double db = *(const double*)b;
    if (da < db)
        return -1;
    if (da > db)
        return 1;
    return 0;
}

// Test structure
typedef struct {
    int priority;
    char name[32];
} Task;

// Comparator for tasks (by priority)
int compare_tasks(const void* a, const void* b)
{
    const Task* ta = (const Task*)a;
    const Task* tb = (const Task*)b;
    return (ta->priority > tb->priority) - (ta->priority < tb->priority);
}

void test_basic_push_pop()
{
    printf("Test: Basic push and pop... ");

    heap* h = create_heap(5, compare_ints);
    int values[] = { 5, 3, 7, 1, 9 };

    // Push all values
    for (int i = 0; i < 5; i++) {
        heap_push(h, &values[i]);
    }

    // Pop and verify they come out in sorted order
    void* out;
    int expected[] = { 1, 3, 5, 7, 9 };
    for (int i = 0; i < 5; i++) {
        heap_pop(h, &out);
        assert(*(int*)out == expected[i]);
    }

    destroy_heap(h);
    printf("PASSED\n");
}

void test_single_element()
{
    printf("Test: Single element... ");

    heap* h = create_heap(1, compare_ints);
    int val = 42;

    heap_push(h, &val);

    void* out;
    heap_pop(h, &out);
    assert(*(int*)out == 42);

    destroy_heap(h);
    printf("PASSED\n");
}

void test_already_sorted()
{
    printf("Test: Already sorted input... ");

    heap* h = create_heap(5, compare_ints);
    int values[] = { 1, 2, 3, 4, 5 };

    for (int i = 0; i < 5; i++) {
        heap_push(h, &values[i]);
    }

    void* out;
    for (int i = 0; i < 5; i++) {
        heap_pop(h, &out);
        assert(*(int*)out == i + 1);
    }

    destroy_heap(h);
    printf("PASSED\n");
}

void test_reverse_sorted()
{
    printf("Test: Reverse sorted input... ");

    heap* h = create_heap(5, compare_ints);
    int values[] = { 5, 4, 3, 2, 1 };

    for (int i = 0; i < 5; i++) {
        heap_push(h, &values[i]);
    }

    void* out;
    for (int i = 0; i < 5; i++) {
        heap_pop(h, &out);
        assert(*(int*)out == i + 1);
    }

    destroy_heap(h);
    printf("PASSED\n");
}

void test_duplicates()
{
    printf("Test: Duplicate values... ");

    heap* h = create_heap(7, compare_ints);
    int values[] = { 3, 1, 4, 1, 5, 9, 2 };

    for (int i = 0; i < 7; i++) {
        heap_push(h, &values[i]);
    }

    void* out;
    int expected[] = { 1, 1, 2, 3, 4, 5, 9 };
    for (int i = 0; i < 7; i++) {
        heap_pop(h, &out);
        assert(*(int*)out == expected[i]);
    }

    destroy_heap(h);
    printf("PASSED\n");
}

void test_with_doubles()
{
    printf("Test: Double precision values... ");

    heap* h = create_heap(5, compare_doubles);
    double values[] = { 3.14, 1.41, 2.71, 0.57, 1.61 };

    for (int i = 0; i < 5; i++) {
        heap_push(h, &values[i]);
    }

    void* out;
    double expected[] = { 0.57, 1.41, 1.61, 2.71, 3.14 };
    for (int i = 0; i < 5; i++) {
        heap_pop(h, &out);
        assert(*(double*)out == expected[i]);
    }

    destroy_heap(h);
    printf("PASSED\n");
}

void test_with_structs()
{
    printf("Test: Custom struct (tasks by priority)... ");

    heap* h = create_heap(4, compare_tasks);
    Task tasks[] = {
        { 3, "Low priority" },
        { 1, "High priority" },
        { 2, "Medium priority" },
        { 1, "Also high priority" }
    };

    for (int i = 0; i < 4; i++) {
        heap_push(h, &tasks[i]);
    }

    void* out;
    int expected_priorities[] = { 1, 1, 2, 3 };
    for (int i = 0; i < 4; i++) {
        heap_pop(h, &out);
        Task* t = (Task*)out;
        assert(t->priority == expected_priorities[i]);
    }

    destroy_heap(h);
    printf("PASSED\n");
}

void test_interleaved_operations()
{
    printf("Test: Interleaved push/pop... ");

    heap* h = create_heap(10, compare_ints);
    int values[] = { 5, 3, 7, 1, 9, 2, 8, 4, 6, 0 };

    // Push 5, pop 1, push 3 more, pop 2, etc.
    heap_push(h, &values[0]); // 5
    heap_push(h, &values[1]); // 3
    heap_push(h, &values[2]); // 7

    void* out;
    heap_pop(h, &out);
    assert(*(int*)out == 3);

    heap_push(h, &values[3]); // 1
    heap_push(h, &values[4]); // 9
    heap_push(h, &values[5]); // 2

    heap_pop(h, &out);
    assert(*(int*)out == 1);
    heap_pop(h, &out);
    assert(*(int*)out == 2);

    heap_push(h, &values[6]); // 8
    heap_push(h, &values[7]); // 4

    heap_pop(h, &out);
    assert(*(int*)out == 4);

    destroy_heap(h);
    printf("PASSED\n");
}

void test_large_heap()
{
    printf("Test: Large heap (1000 elements)... ");

    heap* h = create_heap(1000, compare_ints);
    int* values = malloc(1000 * sizeof(int));

    // Insert values in random-ish order
    for (int i = 0; i < 1000; i++) {
        values[i] = (i * 7 + 13) % 1000;
        heap_push(h, &values[i]);
    }

    // Pop all and verify sorted
    void* out;
    int prev = -1;
    for (int i = 0; i < 1000; i++) {
        heap_pop(h, &out);
        int curr = *(int*)out;
        assert(curr >= prev);
        prev = curr;
    }

    free(values);
    destroy_heap(h);
    printf("PASSED\n");
}

void test_all_same_values()
{
    printf("Test: All same values... ");

    heap* h = create_heap(5, compare_ints);
    int val = 42;
    int values[5];

    for (int i = 0; i < 5; i++) {
        values[i] = val;
        heap_push(h, &values[i]);
    }

    void* out;
    for (int i = 0; i < 5; i++) {
        heap_pop(h, &out);
        assert(*(int*)out == 42);
    }

    destroy_heap(h);
    printf("PASSED\n");
}

int main()
{
    printf("Running Min Heap Tests\n");
    printf("======================\n\n");

    test_basic_push_pop();
    test_single_element();
    test_already_sorted();
    test_reverse_sorted();
    test_duplicates();
    test_with_doubles();
    test_with_structs();
    test_interleaved_operations();
    test_large_heap();
    test_all_same_values();

    printf("\n======================\n");
    printf("All tests PASSED! ✓\n");

    return 0;
}
