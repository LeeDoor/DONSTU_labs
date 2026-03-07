// test_darray.cpp
#include <gtest/gtest.h>
#include "d_array.hpp"

class DArrayTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(DArrayTest, DefaultConstructor) {
    DArray arr;
}

TEST_F(DArrayTest, ParameterizedConstructor) {
    DArray arr(100);
}

TEST_F(DArrayTest, ZeroSizeConstructor) {
    DArray arr(0);
}

TEST_F(DArrayTest, CopyConstructor) {
    DArray original(100);
    DArray copy(original);
}

TEST_F(DArrayTest, MoveConstructor) {
    DArray original(100);
    DArray moved(std::move(original));
}

TEST_F(DArrayTest, CopyAssignment) {
    DArray arr1(100);
    DArray arr2(50);
    arr2 = arr1;
}

TEST_F(DArrayTest, MoveAssignment) {
    DArray arr1(100);
    DArray arr2(50);
    arr2 = std::move(arr1);
}

TEST_F(DArrayTest, SelfCopyAssignment) {
    DArray arr(100);
    arr = arr;
}

TEST_F(DArrayTest, SelfMoveAssignment) {
    DArray arr(100);
    arr = std::move(arr);
}

TEST_F(DArrayTest, AllocSingleBlock) {
    DArray arr(100);
    DArray::Iterator it = arr.alloc(10);
    arr.free(it);
}

TEST_F(DArrayTest, AllocMultipleBlocks) {
    DArray arr(100);
    DArray::Iterator it1 = arr.alloc(10);
    DArray::Iterator it2 = arr.alloc(20);
    DArray::Iterator it3 = arr.alloc(30);
    arr.free(it1);
    arr.free(it2);
    arr.free(it3);
}

TEST_F(DArrayTest, AllocExactFit) {
    DArray arr(32);
    DArray::Iterator it = arr.alloc(32);
    arr.free(it);
}

TEST_F(DArrayTest, AllocExceedTotalSize) {
    DArray arr(50);
    DArray::Iterator it = arr.alloc(60);
}

TEST_F(DArrayTest, AllocZeroSize) {
    DArray arr(100);
    DArray::Iterator it = arr.alloc(0);
}

TEST_F(DArrayTest, AllocFromEmptyArray) {
    DArray arr(0);
    DArray::Iterator it = arr.alloc(10);
}

TEST_F(DArrayTest, AllocMultipleUntilFull) {
    DArray arr(100);
    std::vector<DArray::Iterator> its;
    for(int i = 0; i < 5; ++i) {
        its.push_back(arr.alloc(20));
    }
    DArray::Iterator it = arr.alloc(1);
    for(auto& i : its) {
        arr.free(i);
    }
    arr.free(it);
}

TEST_F(DArrayTest, AllocFragmented) {
    DArray arr(100);
    DArray::Iterator it1 = arr.alloc(30);
    DArray::Iterator it2 = arr.alloc(30);
    arr.free(it1);
    DArray::Iterator it3 = arr.alloc(20);
    arr.free(it2);
    arr.free(it3);
}

TEST_F(DArrayTest, FreeSingleBlock) {
    DArray arr(100);
    DArray::Iterator it = arr.alloc(10);
    arr.free(it);
}

TEST_F(DArrayTest, FreeMultipleBlocks) {
    DArray arr(100);
    DArray::Iterator it1 = arr.alloc(10);
    DArray::Iterator it2 = arr.alloc(20);
    DArray::Iterator it3 = arr.alloc(30);
    arr.free(it2);
    arr.free(it1);
    arr.free(it3);
}

TEST_F(DArrayTest, FreeAndRealloc) {
    DArray arr(100);
    DArray::Iterator it1 = arr.alloc(40);
    DArray::Iterator it2 = arr.alloc(40);
    arr.free(it1);
    DArray::Iterator it3 = arr.alloc(40);
    arr.free(it2);
    arr.free(it3);
}

TEST_F(DArrayTest, FreeInvalidIterator) {
    DArray arr(100);
    DArray::Iterator it = arr.create_iterator(nullptr, 10);
    arr.free(it);
}

TEST_F(DArrayTest, FreeAlreadyFreed) {
    DArray arr(100);
    DArray::Iterator it = arr.alloc(10);
    arr.free(it);
    arr.free(it);
}

TEST_F(DArrayTest, FreeClearedIterator) {
    DArray arr(100);
    DArray::Iterator it = arr.alloc(10);
    it.clear();
    arr.free(it);
}

TEST_F(DArrayTest, AllocAfterFree) {
    DArray arr(100);
    DArray::Iterator it1 = arr.alloc(50);
    arr.free(it1);
    DArray::Iterator it2 = arr.alloc(50);
    arr.free(it2);
}

TEST_F(DArrayTest, AllocAfterMultipleFrees) {
    DArray arr(100);
    DArray::Iterator it1 = arr.alloc(30);
    DArray::Iterator it2 = arr.alloc(30);
    DArray::Iterator it3 = arr.alloc(30);
    arr.free(it1);
    arr.free(it3);
    DArray::Iterator it4 = arr.alloc(60);
    arr.free(it2);
    arr.free(it4);
}

TEST_F(DArrayTest, AllocAndFreeStress) {
    DArray arr(1000);
    std::vector<DArray::Iterator> its;
    for(int i = 0; i < 50; ++i) {
        its.push_back(arr.alloc(20));
    }
    for(auto& it : its) {
        arr.free(it);
    }
    DArray::Iterator it = arr.alloc(1000);
    arr.free(it);
}

TEST_F(DArrayTest, AllocWithDifferentSizes) {
    DArray arr(100);
    std::vector<size_t> sizes = {1,2,4,8,16,32,64};
    std::vector<DArray::Iterator> its;
    for(size_t s : sizes) {
        its.push_back(arr.alloc(s));
    }
    for(auto& it : its) {
        arr.free(it);
    }
}

TEST_F(DArrayTest, FreeInReverseOrder) {
    DArray arr(100);
    std::vector<DArray::Iterator> its;
    for(int i = 0; i < 5; ++i) {
        its.push_back(arr.alloc(10));
    }
    for(int i = 4; i >= 0; --i) {
        arr.free(its[i]);
    }
}

TEST_F(DArrayTest, AllocAfterFreeInMiddle) {
    DArray arr(100);
    DArray::Iterator it1 = arr.alloc(20);
    DArray::Iterator it2 = arr.alloc(20);
    DArray::Iterator it3 = arr.alloc(20);
    arr.free(it2);
    DArray::Iterator it4 = arr.alloc(10);
    DArray::Iterator it5 = arr.alloc(10);
    arr.free(it1);
    arr.free(it3);
    arr.free(it4);
    arr.free(it5);
}

TEST_F(DArrayTest, AllocExactRemaining) {
    DArray arr(100);
    DArray::Iterator it1 = arr.alloc(40);
    DArray::Iterator it2 = arr.alloc(30);
    arr.free(it1);
    DArray::Iterator it3 = arr.alloc(40);
    arr.free(it2);
    arr.free(it3);
}

TEST_F(DArrayTest, AllocAndFreeWithBitmapBoundaries) {
    DArray arr(64);
    DArray::Iterator it1 = arr.alloc(32);
    DArray::Iterator it2 = arr.alloc(32);
    arr.free(it1);
    DArray::Iterator it3 = arr.alloc(32);
    arr.free(it2);
    arr.free(it3);
}

TEST_F(DArrayTest, AllocSpanningBitmapBoundary) {
    DArray arr(100);
    DArray::Iterator it1 = arr.alloc(30);
    DArray::Iterator it2 = arr.alloc(30);
    DArray::Iterator it3 = arr.alloc(30);
    arr.free(it2);
    DArray::Iterator it4 = arr.alloc(40);
    arr.free(it1);
    arr.free(it3);
    arr.free(it4);
}

TEST_F(DArrayTest, AllocAndFreeAll) {
    DArray arr(100);
    DArray::Iterator it1 = arr.alloc(30);
    DArray::Iterator it2 = arr.alloc(30);
    DArray::Iterator it3 = arr.alloc(30);
    arr.free(it1);
    arr.free(it2);
    arr.free(it3);
    DArray::Iterator it4 = arr.alloc(100);
    arr.free(it4);
}

TEST_F(DArrayTest, AllocAfterFreeCreatesHole) {
    DArray arr(100);
    DArray::Iterator it1 = arr.alloc(20);
    DArray::Iterator it2 = arr.alloc(20);
    DArray::Iterator it3 = arr.alloc(20);
    arr.free(it2);
    DArray::Iterator it4 = arr.alloc(15);
    DArray::Iterator it5 = arr.alloc(5);
    arr.free(it1);
    arr.free(it3);
    arr.free(it4);
    arr.free(it5);
}
