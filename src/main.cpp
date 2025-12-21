#include <iostream>
#include <string>
#include <cstdint>

using namespace std;

constexpr uint32_t S[64]{
    7,12,17,22, 7,12,17,22, 7,12,17,22, 7,12,17,22,
    5, 9,14,20, 5, 9,14,20, 5, 9,14,20, 5, 9,14,20,
    4,11,16,23, 4,11,16,23, 4,11,16,23, 4,11,16,23,
    6,10,15,21, 6,10,15,21, 6,10,15,21, 6,10,15,21
};
constexpr uint32_t T[64] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

uint32_t A = 0x67452301;
uint32_t B = 0xefcdab89;
uint32_t C = 0x98badcfe;
uint32_t D = 0x10325476;

uint32_t F(uint32_t x, uint32_t y, uint32_t z){
    return (x & y) | (~x & z);
}
uint32_t G(uint32_t x, uint32_t y, uint32_t z){
    return (x & z) | (y & ~z);
}
uint32_t H(uint32_t x, uint32_t y, uint32_t z){
    return x ^ y ^ z;
}
uint32_t I(uint32_t x, uint32_t y, uint32_t z){
    return y ^ (x | ~z);
}
uint32_t rot_left(uint32_t x, uint32_t s){
    return (x << s) | (x >> (32 - s));
}


int main(){
    string input;
    getline(cin, input);
    
    uint64_t original_bits = input.size() * 8;
    input += '\x80';

    size_t pad_bytes = (56 - (input.size() % 64) + 64) % 64;
    input.append(pad_bytes, '\0');

    for(auto i{0}; i < 8; i++){
        input += static_cast<char>((original_bits >> (8 * i)) & 0xFF);
    }

    for(auto i{0}; i < input.size(); i += 64){

        uint32_t AA = A, BB = B, CC = C, DD = D;

        uint32_t X[16];
        for(auto j{0}; j < 16; j++){
            X[j] = 
            ((uint32_t)(unsigned char)input[i + j * 4]) |
            ((uint32_t)(unsigned char)input[i + j * 4 + 1] << 8) |
            ((uint32_t)(unsigned char)input[i + j * 4 + 2] << 16) |
            ((uint32_t)(unsigned char)input[i + j * 4 + 3] << 24);
        }

        
        for(auto k{0}; k < 64; k++){
            uint32_t f,g;

            if(k < 16){
                f = F(B, C, D);
                g = k;
            }else if(k < 32){
                f = G(B, C, D);
                g = (5 * k + 1) % 16;
            }else if(k < 48){
                f = H(B, C, D);
                g = (3 * k + 5) % 16;
            }else{
                f = I(B, C, D);
                g = (7 * k) % 16;
            }

            uint32_t temp = D;
            D = C;
            C = B;
            B = B + rot_left(A + f + T[k] + X[g], S[k]);
            A = temp;
        }

        //add original values
        A += AA;
        B += BB;
        C += CC;
        D += DD;
    }

    auto print_le = [](uint32_t w) {
    for (int i = 0; i < 4; i++) {
        printf("%02x", (w >> (8 * i)) & 0xFF);
    }
    };

    print_le(A);
    print_le(B);
    print_le(C);
    print_le(D);
    printf("\n");
    return 0;
}