#include "varbyte.hpp"

#include <bitset>
#include <math.h>
#include <algorithm>

//https://nlp.stanford.edu/IR-book/html/htmledition/variable-byte-codes-1.html

std::vector<uint8_t> VBEncode(unsigned int num) {
    std::vector<uint8_t> bytes;

    while(true) {
        bytes.push_back(num % 128);
        if(num < 128)
            break;
        num /= 128;
    }
    std::reverse(bytes.begin(), bytes.end());
    bytes[bytes.size() - 1] += 128;
    return bytes;
}

std::vector<unsigned int> VBDecode(std::vector<uint8_t>& bytestream) {
    std::vector<unsigned int> numbers;
    int n = 0;

    for(size_t i = 0; i < bytestream.size(); i++) {
        if(bytestream[i] < 128) {
            n = 128 * n + bytestream[i];
        }
        else {
            n = 128 * n + (bytestream[i] - 128);
            numbers.push_back(n);
            n = 0;
        }
    }
    return numbers;
}