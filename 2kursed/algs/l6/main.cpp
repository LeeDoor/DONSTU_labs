#include "array_stack.hpp"
#include "lab_runner.hpp"
#include "linked_stack.hpp"
#include "stack_concepts.hpp"

static_assert(StackType<LinkedStack<int>, int>);
static_assert(StackType<ArrayStack<int>, int>);

int main() {
    LabRunner lab_runner;
    lab_runner.run();
    return 0;
}
