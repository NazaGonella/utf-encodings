#include <stdio.h>


int CodepointToUTF32LE(unsigned int codepoint, unsigned char *dst) {

    if (codepoint >= 0x0 && codepoint <= 0x10FFFF) {
        dst[0] = codepoint & 0xFF;
        dst[1] = (codepoint >> 8) & 0xFF;
        dst[2] = (codepoint >> 16) & 0xFF;
        dst[3] = (codepoint >> 24) & 0xFF;
        return 4;
    }

    // invalid codepoint
    return 0;
}


int CodepointToUTF32BE(unsigned int codepoint, unsigned char *dst) {

    if (codepoint >= 0x0 && codepoint <= 0x10FFFF) {
        dst[0] = (codepoint >> 24) & 0xFF;
        dst[1] = (codepoint >> 16) & 0xFF;
        dst[2] = (codepoint >> 8) & 0xFF;
        dst[3] = codepoint & 0xFF;
        return 4;
    }

    // invalid codepoint
    return 0;
}


int CodepointToUTF16LE(unsigned int codepoint, unsigned char *dst) {

    if (codepoint <= 0xFFFF) {
        if (codepoint >= 0xD800 && codepoint <= 0xDFFF) return 0;
        dst[0] = (unsigned char)(codepoint & 0xFF);
        dst[1] = (unsigned char)((codepoint >> 8) & 0xFF);
        return 2;
    }
    else if (codepoint <= 0x10FFFF) {
        unsigned int codepoint_u = codepoint - 0b10000;
        unsigned int high = (0b110110 << 10) | ((codepoint_u >> 10) & 0x3FF);
        unsigned int low  = (0b110111 << 10) | (codepoint_u & 0x3FF);

        dst[0] = high & 0xFF;
        dst[1] = (high >> 8) & 0xFF;
        dst[2] = low & 0xFF;
        dst[3] = (low >> 8) & 0xFF;
        return 4;
    }

    // invalid codepoint
    return 0;
}


int CodepointToUTF16BE(unsigned int codepoint, unsigned char *dst) {

    if (codepoint <= 0xFFFF) {
        if (codepoint >= 0xD800 && codepoint <= 0xDFFF) return 0;
        dst[0] = (unsigned char)((codepoint >> 8) & 0xFF);
        dst[1] = (unsigned char)(codepoint & 0xFF);
        return 2;
    }
    else if (codepoint <= 0x10FFFF) {
        unsigned int codepoint_u = codepoint - 0b10000;
        unsigned int high = (0b110110 << 10) | ((codepoint_u >> 10) & 0x3FF);
        unsigned int low  = (0b110111 << 10) | (codepoint_u & 0x3FF);

        dst[0] = (high >> 8) & 0xFF;
        dst[1] = high & 0xFF;
        dst[2] = (low >> 8) & 0xFF;
        dst[3] = low & 0xFF;
        return 4;
    }

    // invalid codepoint
    return 0;
}


int CodepointToUTF8(unsigned int codepoint, unsigned char *dst) {

    if (codepoint <= 0x7F) {
        dst[0] = (unsigned char)codepoint;
        return 1;
    } else if (codepoint <= 0x7FF) {
        dst[0] = (unsigned char)(0b11000000 | ((codepoint >> 6) & 0x1F));    // (110)0 0000 | 000x xxxx
        dst[1] = (unsigned char)(0b10000000 | (codepoint & 0x3F));           // (10)00 0000 | 00xx xxxx
        return 2;
    } else if (codepoint <= 0xFFFF) {
        dst[0] = (unsigned char)(0b11100000 | ((codepoint >> 12) & 0x0F));   // (1110) 0000 | 0000 xxxx
        dst[1] = (unsigned char)(0b10000000 | ((codepoint >> 6) & 0x3F));    // (10)00 0000 | 00xx xxxx
        dst[2] = (unsigned char)(0b10000000 | (codepoint & 0x3F));           // (10)00 0000 | 00xx xxxx
        return 3;
    } else if (codepoint <= 0x10FFFF) {
        dst[0] = (unsigned char)(0b11110000 | ((codepoint >> 18) & 0x07));   // (1111 0)000 | 0000 0xxx
        dst[1] = (unsigned char)(0b10000000 | ((codepoint >> 12) & 0x3F));   // (10)00 0000 | 00xx xxxx
        dst[2] = (unsigned char)(0b10000000 | ((codepoint >> 6) & 0x3F));    // (10)00 0000 | 00xx xxxx
        dst[3] = (unsigned char)(0b10000000 | (codepoint & 0x3F));           // (10)00 0000 | 00xx xxxx
        return 4;
    }

    // invalid codepoint
    return 0;
}


void PrintCodepointChar(int codepoint) {
    unsigned char encodedChar[5];   // a Unicode character doesn't take more than 4 bytes, the 5th byte is for the null terminator

    size_t len = CodepointToUTF8(codepoint, encodedChar);

    encodedChar[len] = '\0';
    printf("%s\n", encodedChar);
}


void PrintCodepointCombiningChar(int codepointBase, int codepointComb) {
    unsigned char encodedChars[9];
    unsigned char* p = encodedChars;
    p += CodepointToUTF8(codepointBase, encodedChars);
    p += CodepointToUTF8(codepointComb, p);

    *p = '\0';
    printf("%s\n", encodedChars);
}


int main(void) {
    // regular characters
    printf("Regular characters\n");
    PrintCodepointChar(0x0040);     // OUTPUT: @
    PrintCodepointChar(0xE9);       // OUTPUT: é
    PrintCodepointChar(0x03BB);     // OUTPUT: λ
    PrintCodepointChar(0x266A);     // OUTPUT: ♪
    PrintCodepointChar(0x1F60E);    // OUTPUT: 😎
    PrintCodepointChar(0x1F40C);    // OUTPUT: 🐌
    PrintCodepointChar(0x1F697);    // OUTPUT: 🚗
    PrintCodepointChar(0x1F43B);    // OUTPUT: 🐻

    // regular characters in a string
    printf("\nRegular characters in a string\n");
    PrintCodepointCombiningChar(0x1F47D, 0x1F916);  // OUTPUT: 👽🤖
    PrintCodepointCombiningChar(0x1F355, 0x1F62D);  // OUTPUT: 🍕😭

    // combining characters
    printf("\nCombining characters in a string\n");
    PrintCodepointChar(0x0065);                     // OUTPUT: e
    PrintCodepointCombiningChar(0x0065, 0x0301);    // OUTPUT: é
}