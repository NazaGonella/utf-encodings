#include <stdio.h>


int CodepointToUTF32(unsigned int codepoint, unsigned char *output) {

    if (codepoint >= 0x0 && codepoint <= 0x10FFFF) {
        output[0] = (codepoint >> 24) & 0xFF;
        output[1] = (codepoint >> 16) & 0xFF;
        output[2] = (codepoint >> 8) & 0xFF;
        output[3] = codepoint & 0xFF;
        return 4;
    }

    // Invalid codepoint
    return 0;
}


int CodepointToUTF16(unsigned int codepoint, unsigned char *output) {

    if (codepoint <= 0xFFFF) {
        if (codepoint >= 0xD800 && codepoint <= 0xDFFF) return 0;
        output[0] = (unsigned char)((codepoint >> 8) & 0xFF);
        output[1] = (unsigned char)(codepoint & 0xFF);
        return 2;
    }
    else if (codepoint <= 0x10FFFF) {
        unsigned int codepoint_u = codepoint - 0b10000;
        unsigned int high = (0b110110 << 10) | ((codepoint_u >> 10) & 0x3FF);
        unsigned int low  = (0b110111 << 10) | (codepoint_u & 0x3FF);

        output[0] = (high >> 8) & 0xFF;
        output[1] = high & 0xFF;
        output[2] = (low >> 8) & 0xFF;
        output[3] = low & 0xFF;
        return 4;
    }

    // Invalid codepoint
    return 0;
}


int CodepointToUTF8(unsigned int codepoint, unsigned char *output) {

    // codepoint: U+uvwxyz
    if (codepoint <= 0x7F) {
        output[0] = (unsigned char)codepoint;
        return 1;
    } else if (codepoint <= 0x7FF) {
        output[0] = (unsigned char)(0b11000000 | ((codepoint >> 6) & 0x1F));   // (110)0 0000 | 000x xxyy = (110)x xxyy
        output[1] = (unsigned char)(0b10000000 | (codepoint & 0x3F));          // (10)00 0000 | 00yy zzzz = (10)yy zzzz
        return 2;
    } else if (codepoint <= 0xFFFF) {
        output[0] = (unsigned char)(0b11100000 | ((codepoint >> 12) & 0x0F));  // (1110) 0000 | 0000 wwww = (1110) wwww
        output[1] = (unsigned char)(0b10000000 | ((codepoint >> 6) & 0x3F));   // (10)00 0000 | 00xx xxyy = (10)xx xxyy
        output[2] = (unsigned char)(0b10000000 | (codepoint & 0x3F));          // (10)00 0000 | 00yy zzzz = (10)yy zzzz
        return 3;
    } else if (codepoint <= 0x10FFFF) {
        output[0] = (unsigned char)(0b11110000 | ((codepoint >> 18) & 0x07));  // (1111 0)000 | 0000 0uvv = (1111 0)uvv
        output[1] = (unsigned char)(0b10000000 | ((codepoint >> 12) & 0x3F));  // (10)00 0000 | 00vv wwww = (10)vv wwww
        output[2] = (unsigned char)(0b10000000 | ((codepoint >> 6) & 0x3F));   // (10)00 0000 | 00xx xxyy = (10)xx xxyy
        output[3] = (unsigned char)(0b10000000 | (codepoint & 0x3F));          // (10)00 0000 | 00yy zzzz = (10)yy zzzz
        return 4;
    }

    // Invalid codepoint
    return 0;
}

int main(void) {
    unsigned char utf[10];
    unsigned char* p = utf;
    p += CodepointToUTF8(0x0065, utf);
    size_t len = CodepointToUTF8(0x0301, p);
    utf[len] = '\0';
    printf("%s\n", utf);    // OUTPUT: é
    return 0;
}

// int main(void) {
//     unsigned char zalgo[30];
//     unsigned char* p = zalgo;
//     p += CodepointToUTF8(0x0041, p);
//     p += CodepointToUTF8(0x0315, p);
//     p += CodepointToUTF8(0x0300, p);
//     p += CodepointToUTF8(0x0338, p);
//     p += CodepointToUTF8(0x0341, p);
//     p += CodepointToUTF8(0x035B, p);
//     CodepointToUTF8(0x0323, p);

//     printf("%s\n", zalgo);     // OUTPUT: Ạ̸̀́͛̕
// }