#include "cpu.h"
#include "cpu_test.h"

int main() {
    test_initial_state();
    test_set_flags();

    LDA();
    
    return 0;
}