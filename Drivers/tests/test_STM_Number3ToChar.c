#include <stdint.h>
#include <string.h>
#include <assert.h>

void STM_Number3ToChar(uint16_t pNum, char pOut[3]);

void test_STM_Number3ToChar_zero(void) {
    char result[3];
    STM_Number3ToChar(0, result);
    assert(result[0] == '0');
    assert(result[1] == '0');
    assert(result[2] == '0');
}

void test_STM_Number3ToChar_single_digit(void) {
    char result[3];
    STM_Number3ToChar(5, result);
    assert(result[0] == '0');
    assert(result[1] == '0');
    assert(result[2] == '5');
}

void test_STM_Number3ToChar_two_digits(void) {
    char result[3];
    STM_Number3ToChar(42, result);
    assert(result[0] == '0');
    assert(result[1] == '4');
    assert(result[2] == '2');
}

void test_STM_Number3ToChar_three_digits(void) {
    char result[3];
    STM_Number3ToChar(789, result);
    assert(result[0] == '7');
    assert(result[1] == '8');
    assert(result[2] == '9');
}

void test_STM_Number3ToChar_max_value(void) {
    char result[3];
    STM_Number3ToChar(999, result);
    assert(result[0] == '9');
    assert(result[1] == '9');
    assert(result[2] == '9');
}

void test_STM_Number3ToChar_hundred(void) {
    char result[3];
    STM_Number3ToChar(100, result);
    assert(result[0] == '1');
    assert(result[1] == '0');
    assert(result[2] == '0');
}

void test_STM_Number3ToChar_ten(void) {
    char result[3];
    STM_Number3ToChar(10, result);
    assert(result[0] == '0');
    assert(result[1] == '1');
    assert(result[2] == '0');
}