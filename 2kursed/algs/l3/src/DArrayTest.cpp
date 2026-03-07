// test_iterator.cpp
#include <gtest/gtest.h>
#include "d_array.hpp"

class IteratorTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(IteratorTest, CopyConstructor) {
    DArray arr(100);
    DArray::Iterator it1 = arr.alloc(10);
    DArray::Iterator it2(it1);
    arr.free(it1);
    arr.free(it2);
}

TEST_F(IteratorTest, MoveConstructor) {
    DArray arr(100);
    DArray::Iterator it1 = arr.alloc(10);
    DArray::Iterator it2(std::move(it1));
    arr.free(it2);
}

TEST_F(IteratorTest, CopyAssignment) {
    DArray arr(100);
    DArray::Iterator it1 = arr.alloc(10);
    DArray::Iterator it2 = arr.create_iterator(nullptr, 0);
    it2 = it1;
    arr.free(it1);
    arr.free(it2);
}

TEST_F(IteratorTest, MoveAssignment) {
    DArray arr(100);
    DArray::Iterator it1 = arr.alloc(10);
    DArray::Iterator it2 = arr.create_iterator(nullptr, 0);
    it2 = std::move(it1);
    arr.free(it2);
}

TEST_F(IteratorTest, SelfCopyAssignment) {
    DArray arr(100);
    DArray::Iterator it = arr.alloc(10);
    it = it;
    arr.free(it);
}

TEST_F(IteratorTest, SelfMoveAssignment) {
    DArray arr(100);
    DArray::Iterator it = arr.alloc(10);
    it = std::move(it);
    arr.free(it);
}

TEST_F(IteratorTest, Dereference) {
    DArray arr(100);
    DArray::Iterator it = arr.alloc(10);
    ByteType* ptr = *it;
    ByteType* ptr2 = it.operator->();
    ByteType* ptr3 = it.read();
    arr.free(it);
}

TEST_F(IteratorTest, Size) {
    DArray arr(100);
    DArray::Iterator it = arr.alloc(15);
    EXPECT_EQ(15, it.size());
    arr.free(it);
}

TEST_F(IteratorTest, Write) {
    DArray arr(100);
    DArray::Iterator it = arr.alloc(10);
    ByteType data[10] = {1,2,3,4,5,6,7,8,9,0};
    it.write(data, 10);
    arr.free(it);
}

TEST_F(IteratorTest, WritePartial) {
    DArray arr(100);
    DArray::Iterator it = arr.alloc(20);
    ByteType data[10] = {1,2,3,4,5,6,7,8,9,0};
    it.write(data, 10);
    arr.free(it);
}

TEST_F(IteratorTest, WriteExceedSize) {
    DArray arr(100);
    DArray::Iterator it = arr.alloc(10);
    ByteType data[20] = {0};
    EXPECT_THROW(it.write(data, 20), std::runtime_error);
    arr.free(it);
}

TEST_F(IteratorTest, Clear) {
    DArray arr(100);
    DArray::Iterator it = arr.alloc(10);
    it.clear();
}

TEST_F(IteratorTest, ClearTwice) {
    DArray arr(100);
    DArray::Iterator it = arr.alloc(10);
    it.clear();
    it.clear();
}

TEST_F(IteratorTest, WriteAfterClear) {
    DArray arr(100);
    DArray::Iterator it = arr.alloc(10);
    it.clear();
    ByteType data[5] = {0};
    EXPECT_THROW(it.write(data, 5), std::runtime_error);
}

TEST_F(IteratorTest, MultipleCopiesOfSameBlock) {
    DArray arr(100);
    DArray::Iterator it1 = arr.alloc(20);
    DArray::Iterator it2(it1);
    DArray::Iterator it3(it2);
    DArray::Iterator it4 = it3;
    arr.free(it1);
    arr.free(it2);
    arr.free(it3);
    arr.free(it4);
}

TEST_F(IteratorTest, AssignmentChain) {
    DArray arr(100);
    DArray::Iterator it1 = arr.alloc(10);
    DArray::Iterator it2 = arr.create_iterator(nullptr, 0);
    DArray::Iterator it3 = arr.create_iterator(nullptr, 0);
    it2 = it1;
    it3 = it2;
    arr.free(it1);
    arr.free(it2);
    arr.free(it3);
}

TEST_F(IteratorTest, MoveAssignmentChain) {
    DArray arr(100);
    DArray::Iterator it1 = arr.alloc(10);
    DArray::Iterator it2 = arr.create_iterator(nullptr, 0);
    DArray::Iterator it3 = arr.create_iterator(nullptr, 0);
    it2 = std::move(it1);
    it3 = std::move(it2);
    arr.free(it3);
}

TEST_F(IteratorTest, InterleavedCopyAndMove) {
    DArray arr(100);
    DArray::Iterator it1 = arr.alloc(30);
    DArray::Iterator it2(it1);
    DArray::Iterator it3(std::move(it2));
    DArray::Iterator it4 = arr.create_iterator(nullptr, 0);
    it4 = it1;
    DArray::Iterator it5(std::move(it4));
    arr.free(it1);
    arr.free(it3);
    arr.free(it5);
}

TEST_F(IteratorTest, WriteAfterFree) {
    DArray arr(100);
    DArray::Iterator it = arr.alloc(10);
    arr.free(it);
    ByteType data[5] = {0};
    EXPECT_THROW(it.write(data, 5), std::runtime_error);
}

TEST_F(IteratorTest, CopyAfterFree) {
    DArray arr(100);
    DArray::Iterator it1 = arr.alloc(10);
    DArray::Iterator it2(it1);
    arr.free(it1);
    ByteType data[5] = {0};
    EXPECT_THROW(it2.write(data, 5), std::runtime_error);
    arr.free(it2);
}

TEST_F(IteratorTest, MoveAfterFree) {
    DArray arr(100);
    DArray::Iterator it1 = arr.alloc(10);
    DArray::Iterator it2(std::move(it1));
    arr.free(it2);
    ByteType data[5] = {0};
    EXPECT_THROW(it2.write(data, 5), std::runtime_error);
}

TEST_F(IteratorTest, MultipleIteratorsToSameBlock) {
    DArray arr(100);
    DArray::Iterator it1 = arr.alloc(20);
    DArray::Iterator it2(it1);
    arr.free(it1);
    arr.free(it2);
    ByteType data[5] = {0};
    EXPECT_THROW(it2.write(data, 5), std::runtime_error);
}

TEST_F(IteratorTest, DestructorWarning) {
    DArray arr(100);
    DArray::Iterator it = arr.alloc(10);
}
