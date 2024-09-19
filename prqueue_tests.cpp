#include "prqueue.h"

#include "gtest/gtest.h"

using namespace std;

TEST(BlankTest, Test) {
    // TODO_STUDENT
}

TEST(PrQueueTests, ConstructorAndSize) {
    prqueue<int> pq;
    EXPECT_EQ(0, pq.size());
}

TEST(PrQueueTests, EnqueuePeekDequeue) {
    prqueue<int> pq;
    pq.enqueue(10, 1);
    pq.enqueue(20, 2);
    EXPECT_EQ(10, pq.peek());
    EXPECT_EQ(10, pq.dequeue());
    EXPECT_EQ(1, pq.size());
    EXPECT_EQ(20, pq.peek());
}

TEST(PrQueueTests, EnqueueMultipleElements) {
    prqueue<int> pq;
    pq.enqueue(30, 3); // Third in priority
    pq.enqueue(10, 1); // First in priority
    pq.enqueue(20, 2); // Second in priority
    EXPECT_EQ(10, pq.peek()); // Expect the element with highest priority (lowest number)
    pq.dequeue();
    EXPECT_EQ(20, pq.peek()); // Next highest priority
    pq.dequeue();
    EXPECT_EQ(30, pq.peek()); // Last priority
}


TEST(PrQueueTests, DequeueUntilEmpty) {
    prqueue<int> pq;
    pq.enqueue(30, 3);
    pq.enqueue(10, 1);
    pq.enqueue(20, 2);
    EXPECT_EQ(10, pq.dequeue());
    EXPECT_EQ(20, pq.dequeue());
    EXPECT_EQ(30, pq.dequeue());
    EXPECT_EQ(0, pq.size());
    EXPECT_EQ(int(), pq.dequeue()); // Dequeue from empty queue, expect default int.
}


TEST(PrQueueTests, AsString) {
    prqueue<int> pq;
    pq.enqueue(10, 1);
    pq.enqueue(20, 2);
    string expected = "1 value: 10\n2 value: 20\n";
    EXPECT_EQ(expected, pq.as_string());
}

TEST(PrQueueTests, Clear) {
    prqueue<int> pq;
    pq.enqueue(10, 1);
    pq.clear();
    EXPECT_EQ(0, pq.size());
}

TEST(PrQueueTests, ClearAndReuseQueue) {
    prqueue<int> pq;
    pq.enqueue(10, 1);
    pq.clear();
    EXPECT_EQ(0, pq.size());
    // Reuse after clear
    pq.enqueue(20, 2);
    EXPECT_EQ(1, pq.size());
    EXPECT_EQ(20, pq.peek());
}


TEST(PrQueueTests, BeginMethod) {
    prqueue<int> pq;
    pq.enqueue(30, 3);
    pq.enqueue(10, 1);
    pq.enqueue(20, 2);
    pq.begin();
    int value;
    int priority;
    EXPECT_TRUE(pq.next(value, priority));
    EXPECT_EQ(value, 10);
    EXPECT_EQ(priority, 1);
}

TEST(PrQueueTests, NextMethod) {
    prqueue<int> pq;
    pq.enqueue(30, 3);
    pq.enqueue(10, 1);
    pq.enqueue(20, 2);
    pq.begin();
    int value, priority;
    EXPECT_TRUE(pq.next(value, priority));
    EXPECT_EQ(value, 10);
    EXPECT_TRUE(pq.next(value, priority));
    EXPECT_EQ(value, 20);
    EXPECT_TRUE(pq.next(value, priority));
    EXPECT_EQ(value, 30);
    EXPECT_FALSE(pq.next(value, priority)); // No more elements
}


TEST(PrQueueTests, EnqueueWithDuplicates) {
    prqueue<string> pq;
    pq.enqueue("A", 2);
    pq.enqueue("B", 2);  // Duplicate priority
    pq.enqueue("C", 1);

    EXPECT_EQ("C", pq.peek());
    pq.dequeue();
    EXPECT_EQ("A", pq.peek());  // Check the order of duplicates
}

TEST(PrQueueTests, CopyConstructor) {
    prqueue<int> pq;
    pq.enqueue(10, 1);
    pq.enqueue(20, 2);

    prqueue<int> pqCopy = pq;  // Using copy constructor
    EXPECT_EQ(pqCopy.dequeue(), 10);
    EXPECT_EQ(pqCopy.dequeue(), 20);
    EXPECT_EQ(pqCopy.size(), 0);
}

TEST(PrQueueTests, AssignmentOperator) {
    prqueue<int> pq;
    pq.enqueue(30, 3);
    prqueue<int> pqAssign;
    pqAssign = pq;  // Using assignment operator
    EXPECT_EQ(pqAssign.dequeue(), 30);
    EXPECT_EQ(pqAssign.size(), 0);
}

TEST(PrQueueTests, EqualityOperator) {
    prqueue<int> pq1;
    pq1.enqueue(10, 1);
    pq1.enqueue(20, 2);

    prqueue<int> pq2;
    pq2.enqueue(10, 1);
    pq2.enqueue(20, 2);

    EXPECT_TRUE(pq1 == pq2);
}

TEST(PrQueueTests, IterativeInOrderTraversal) {
    prqueue<int> pq;
    pq.enqueue(40, 4);
    pq.enqueue(20, 2);
    pq.enqueue(30, 3);
    pq.enqueue(10, 1);

    pq.begin();
    int value;
    int priority;
    string traversal;
    while (pq.next(value, priority)) {
        traversal += to_string(value) + " ";
    }

    EXPECT_EQ(traversal, "10 20 30 40 ");
}

TEST(PrQueueTests, IterativeInOrderTraversalWithDuplicates) {
    prqueue<int> pq;
    pq.enqueue(40, 4);
    pq.enqueue(20, 2);
    pq.enqueue(20, 2); // Duplicate
    pq.enqueue(10, 1);

    pq.begin();
    int value;
    int priority;
    string traversal;
    while (pq.next(value, priority)) {
        traversal += to_string(value) + " ";
    }

    // The expected order is: lowest priority first, with duplicates following the order they were enqueued
    EXPECT_EQ(traversal, "10 20 20 40 ");
}

TEST(PrQueueTests, EqualityOperatorWithDifferentTrees) {
    prqueue<int> pq1;
    pq1.enqueue(10, 1);
    pq1.enqueue(30, 3); // Different structure and values compared to pq2

    prqueue<int> pq2;
    pq2.enqueue(10, 1);
    pq2.enqueue(20, 2); // Different structure and values compared to pq1

    EXPECT_FALSE(pq1 == pq2);
}

TEST(PrQueueTests, ComplexTreeInOrderTraversal) {
    prqueue<int> pq;
    // Constructing a tree with a mix of left and right children
    pq.enqueue(50, 5);
    pq.enqueue(20, 2);  
    pq.enqueue(70, 7);  
    pq.enqueue(10, 1);  
    pq.enqueue(30, 3); 
    pq.enqueue(60, 6);  
    pq.enqueue(80, 8);  
    pq.begin();
    int value;
    int priority;
    string traversal;
    while (pq.next(value, priority)) {
        traversal += std::to_string(value) + " ";
    }

    // Expected in-order traversal order
    EXPECT_EQ(traversal, "10 20 30 50 60 70 80 ");
}

