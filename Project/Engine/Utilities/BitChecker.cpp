#include "BitChecker.h"

bool CheckBit(uint32_t bitA, uint32_t bitB, CheckType type) {
    if (type == CheckType::HAS) {
        return HasBit(bitA, bitB);
    }else if(type == CheckType::EQUAL){
        return EqualBit(bitA, bitB);
    }

    return false;
}

bool HasBit(uint32_t bitA, uint32_t bitB) {
    return (bitA & bitB) != 0;
}

bool EqualBit(uint32_t bitA, uint32_t bitB) {
    return (bitA == bitB);
}
