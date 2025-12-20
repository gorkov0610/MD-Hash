#include <iostream>
#include <string>
#include <cstdint>

using namespace std;

constexpr uint32_t K2 = 0x5a827999;
constexpr uint32_t K3 = 0x6ed9eba1;

uint32_t F(uint32_t x, uint32_t y, uint32_t z){
    return (x & y) | (~x & z);
}
uint32_t G(uint32_t x, uint32_t y, uint32_t z){
    return (x & y) | (x & z) | (y & z);
}
uint32_t H(uint32_t x, uint32_t y, uint32_t z){
    return x ^ y ^ z;
}
uint32_t rot_left(uint32_t x, uint32_t s){
    return (x << s) | (x >> (32 - s));
}
int main(){
    uint32_t A = 0x67452301;
    uint32_t B = 0xefcdab89;
    uint32_t C = 0x98badcfe;
    uint32_t D = 0x10325476;
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

        //round 1
        A = rot_left(A + F(B, C, D) + X[0], 3);
        D = rot_left(D + F(A, B, C) + X[1], 7);
        C = rot_left(C + F(D, A, B) + X[2], 11);
        B = rot_left(B + F(C, D, A) + X[3], 19);

        A = rot_left(A + F(B, C, D) + X[4], 3);
        D = rot_left(D + F(A, B, C) + X[5], 7);
        C = rot_left(C + F(D, A, B) + X[6], 11);
        B = rot_left(B + F(C, D, A) + X[7], 19);

        A = rot_left(A + F(B, C, D) + X[8], 3);
        D = rot_left(D + F(A, B, C) + X[9], 7);
        C = rot_left(C + F(D, A, B) + X[10], 11);
        B = rot_left(B + F(C, D, A) + X[11], 19);

        A = rot_left(A + F(B, C, D) + X[12], 3);
        D = rot_left(D + F(A, B, C) + X[13], 7);
        C = rot_left(C + F(D, A, B) + X[14], 11);
        B = rot_left(B + F(C, D, A) + X[15], 19);

        //round 2
        A = rot_left(A + G(B, C, D) + X[0] + K2, 3);
        D = rot_left(D + G(A, B, C) + X[4] + K2, 5);
        C = rot_left(C + G(D, A, B) + X[8] + K2, 9);
        B = rot_left(B + G(C, D, A) + X[12] + K2, 13);

        A = rot_left(A + G(B, C, D) + X[1] + K2, 3);
        D = rot_left(D + G(A, B, C) + X[5] + K2, 5);
        C = rot_left(C + G(D, A, B) + X[9] + K2, 9);
        B = rot_left(B + G(C, D, A) + X[13] + K2, 13);

        A = rot_left(A + G(B, C, D) + X[2] + K2, 3);
        D = rot_left(D + G(A, B, C) + X[6] + K2, 5);
        C = rot_left(C + G(D, A, B) + X[10] + K2, 9);
        B = rot_left(B + G(C, D, A) + X[14] + K2, 13);

        A = rot_left(A + G(B, C, D) + X[3] + K2, 3);
        D = rot_left(D + G(A, B, C) + X[7] + K2, 5);
        C = rot_left(C + G(D, A, B) + X[11] + K2, 9);
        B = rot_left(B + G(C, D, A) + X[15] + K2, 13);

        //round 3
        int s[4] = {3, 9, 11, 15};
        int order[16] = {0,8,4,12,2,10,6,14,1,9,5,13,3,11,7,15};
        for (int j = 0; j < 16; ++j) {
            int shift = s[j%4];
            switch (j%4) {
                case 0: A = rot_left(A + H(B,C,D) + X[order[j]] + K3, shift); break;
                case 1: D = rot_left(D + H(A,B,C) + X[order[j]] + K3, shift); break;
                case 2: C = rot_left(C + H(D,A,B) + X[order[j]] + K3, shift); break;
                case 3: B = rot_left(B + H(C,D,A) + X[order[j]] + K3, shift); break;
            }
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