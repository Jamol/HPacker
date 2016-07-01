//
//  main.cpp
//  hpack-test
//
//  Created by Jamol Bao on 6/29/16.
//  Copyright © 2016 jamol. All rights reserved.
//

#include <iostream>
#include "HPacker.h"

const std::string hex_str = "820481634188353daded6ae43d3f877abdd07f66a281b0dae053fad0321aa49d13fda992a49685340c8a6adca7e28102e10fda9677b8d05707f6a62293a9d810020004015309ac2ca7f2c3415c1f53b0497ca589d34d1f43aeba0c41a4c7a98f33a69a3fdf9a68fa1d75d0620d263d4c79a68fbed00177febe58f9fbed00177b518b2d4b70ddf45abefb4005db901f1184ef034eff609cb60725034f48e1561c8469669f081678ae3eb3afba465f7cb234db9f4085aec1cd48ff86a8eb10649cbf";

// RFC 7541 Appendix C.3
const std::string hex_no_huff_req1 = "8286 8441 0f77 7777 2e65 7861 6d70 6c65 2e63 6f6d";
const std::string hex_no_huff_req2 = "8286 84be 5808 6e6f 2d63 6163 6865";
const std::string hex_no_huff_req3 = "8287 85bf 400a 6375 7374 6f6d 2d6b 6579 0c63 7573 746f 6d2d 7661 6c75 65";

// RFC 7541 Appendix C.4
const std::string hex_huff_req1 = "8286 8441 8cf1 e3c2 e5f2 3a6b a0ab 90f4 ff";
const std::string hex_huff_req2 = "8286 84be 5886 a8eb 1064 9cbf";
const std::string hex_huff_req3 = "8287 85bf 4088 25a8 49e9 5ba9 7d7f 8925 a849 e95b b8e8 b4bf";

// RFC 7541 Appendix C.5
const std::string hex_no_huff_rsp1 =
"4803 3330 3258 0770 7269 7661 7465 611d"
"4d6f 6e2c 2032 3120 4f63 7420 3230 3133"
"2032 303a 3133 3a32 3120 474d 546e 1768"
"7474 7073 3a2f 2f77 7777 2e65 7861 6d70"
"6c65 2e63 6f6d";
const std::string hex_no_huff_rsp2 = "4803 3330 37c1 c0bf";
const std::string hex_no_huff_rsp3 =
"88c1 611d 4d6f 6e2c 2032 3120 4f63 7420"
"3230 3133 2032 303a 3133 3a32 3220 474d"
"54c0 5a04 677a 6970 7738 666f 6f3d 4153"
"444a 4b48 514b 425a 584f 5157 454f 5049"
"5541 5851 5745 4f49 553b 206d 6178 2d61"
"6765 3d33 3630 303b 2076 6572 7369 6f6e"
"3d31";

// RFC 7541 Appendix C.6
const std::string hex_huff_rsp1 =
"4882 6402 5885 aec3 771a 4b61 96d0 7abe"
"9410 54d4 44a8 2005 9504 0b81 66e0 82a6"
"2d1b ff6e 919d 29ad 1718 63c7 8f0b 97c8"
"e9ae 82ae 43d3";
const std::string hex_huff_rsp2 = "4883 640e ffc1 c0bf";
const std::string hex_huff_rsp3 =
"88c1 6196 d07a be94 1054 d444 a820 0595"
"040b 8166 e084 a62d 1bff c05a 839b d9ab"
"77ad 94e7 821d d7f2 e6c7 b335 dfdf cd5b"
"3960 d5af 2708 7f36 72c1 ab27 0fb5 291f"
"9587 3160 65c0 03ed 4ee5 b106 3d50 07";

using namespace hpack;

uint8_t cb[256];
const char *number1 = "0123456789";
const char *number2 = "abcdef";
const char *number3 = "ABCDEF";

std::vector<uint8_t> hexStringToArray(const std::string &hex_str) {
    memset(cb, 0, sizeof(cb));
    for (int i = 0; i < 10; ++i) {
        cb[number1[i]] = i;
    }
    for (int i = 0; i < 6; ++i) {
        cb[number2[i]] = i + 10;
        cb[number3[i]] = i + 10;
    }
    std::vector<uint8_t> vec;
    const char* ptr = hex_str.c_str();
    for (int i = 0; i < hex_str.length() - 1; ) {
        if (ptr[i] == ' ') {
            ++i;
            continue;
        }
        uint8_t u8 = 0;
        u8 = (cb[ptr[i]] << 4) | cb[ptr[i + 1]];
        vec.push_back(u8);
        i += 2;
    }
    return vec;
}

int main(int argc, const char * argv[]) {
    // insert code here...
    
    HPacker hp;
    HPacker::KeyValueVector headers;
    auto vec = hexStringToArray(hex_str);
    int ret = hp.decode(&vec[0], vec.size(), headers);
    printf("ret=%d\n", ret);
    for (auto &kv : headers) {
        printf("%s: %s\n", kv.first.c_str(), kv.second.c_str());
    }
    size_t len = hex_str.length();
    uint8_t *buf = new uint8_t[len];
    HPacker enhp;
    ret = enhp.encode(headers, buf, len);
    if (ret < 0) {
        printf("encode failed\n");
        return 1;
    }
    printf("en_len=%d\n", ret);
    headers.clear();
    HPacker dehp;
    ret = dehp.decode(buf, ret, headers);
    if (ret < 0) {
        printf("decode failed\n");
        return 1;
    }
    printf("de_len=%d\n", ret);
    for (auto &kv : headers) {
        printf("%s: %s\n", kv.first.c_str(), kv.second.c_str());
    }
    
    printf("\ntest request without huffman coding.....................\n");
    vec = hexStringToArray(hex_no_huff_req1);
    HPacker hpc3;
    ret = hpc3.decode(&vec[0], vec.size(), headers);
    if (ret < 0) {
        printf("decode failed\n");
        return 1;
    }
    printf("de_len=%d\n", ret);
    for (auto &kv : headers) {
        printf("%s: %s\n", kv.first.c_str(), kv.second.c_str());
    }
    printf("\nrequest 2 ..............\n");
    vec = hexStringToArray(hex_no_huff_req2);
    ret = hpc3.decode(&vec[0], vec.size(), headers);
    if (ret < 0) {
        printf("decode failed\n");
        return 1;
    }
    printf("de_len=%d\n", ret);
    for (auto &kv : headers) {
        printf("%s: %s\n", kv.first.c_str(), kv.second.c_str());
    }
    printf("\nrequest 3 ..............\n");
    vec = hexStringToArray(hex_no_huff_req3);
    ret = hpc3.decode(&vec[0], vec.size(), headers);
    if (ret < 0) {
        printf("decode failed\n");
        return 1;
    }
    printf("de_len=%d\n", ret);
    for (auto &kv : headers) {
        printf("%s: %s\n", kv.first.c_str(), kv.second.c_str());
    }
    
    printf("\ntest request with huffman coding.....................\n");
    vec = hexStringToArray(hex_huff_req1);
    HPacker hpc4;
    ret = hpc4.decode(&vec[0], vec.size(), headers);
    if (ret < 0) {
        printf("decode failed\n");
        return 1;
    }
    printf("de_len=%d\n", ret);
    for (auto &kv : headers) {
        printf("%s: %s\n", kv.first.c_str(), kv.second.c_str());
    }
    printf("\nrequest 2 ..............\n");
    vec = hexStringToArray(hex_huff_req2);
    ret = hpc4.decode(&vec[0], vec.size(), headers);
    if (ret < 0) {
        printf("decode failed\n");
        return 1;
    }
    printf("de_len=%d\n", ret);
    for (auto &kv : headers) {
        printf("%s: %s\n", kv.first.c_str(), kv.second.c_str());
    }
    printf("\nrequest 3 ..............\n");
    vec = hexStringToArray(hex_huff_req3);
    ret = hpc4.decode(&vec[0], vec.size(), headers);
    if (ret < 0) {
        printf("decode failed\n");
        return 1;
    }
    printf("de_len=%d\n", ret);
    for (auto &kv : headers) {
        printf("%s: %s\n", kv.first.c_str(), kv.second.c_str());
    }
    
    printf("\ntest response without huffman coding.....................\n");
    HPacker hpc5;
    hpc5.setMaxTableSize(256);
    vec = hexStringToArray(hex_no_huff_rsp1);
    ret = hpc5.decode(&vec[0], vec.size(), headers);
    if (ret < 0) {
        printf("decode failed\n");
        return 1;
    }
    printf("de_len=%d\n", ret);
    for (auto &kv : headers) {
        printf("%s: %s\n", kv.first.c_str(), kv.second.c_str());
    }
    printf("\nresponse 2 ..............\n");
    vec = hexStringToArray(hex_no_huff_rsp2);
    ret = hpc5.decode(&vec[0], vec.size(), headers);
    if (ret < 0) {
        printf("decode failed\n");
        return 1;
    }
    printf("de_len=%d\n", ret);
    for (auto &kv : headers) {
        printf("%s: %s\n", kv.first.c_str(), kv.second.c_str());
    }
    printf("\nresponse 3 ..............\n");
    vec = hexStringToArray(hex_no_huff_rsp3);
    ret = hpc5.decode(&vec[0], vec.size(), headers);
    if (ret < 0) {
        printf("decode failed\n");
        return 1;
    }
    printf("de_len=%d\n", ret);
    for (auto &kv : headers) {
        printf("%s: %s\n", kv.first.c_str(), kv.second.c_str());
    }
    
    printf("\ntest response with huffman coding.....................\n");
    HPacker hpc6;
    hpc6.setMaxTableSize(256);
    vec = hexStringToArray(hex_huff_rsp1);
    ret = hpc6.decode(&vec[0], vec.size(), headers);
    if (ret < 0) {
        printf("decode failed\n");
        return 1;
    }
    printf("de_len=%d\n", ret);
    for (auto &kv : headers) {
        printf("%s: %s\n", kv.first.c_str(), kv.second.c_str());
    }
    printf("\nresponse 2 ..............\n");
    vec = hexStringToArray(hex_huff_rsp2);
    ret = hpc6.decode(&vec[0], vec.size(), headers);
    if (ret < 0) {
        printf("decode failed\n");
        return 1;
    }
    printf("de_len=%d\n", ret);
    for (auto &kv : headers) {
        printf("%s: %s\n", kv.first.c_str(), kv.second.c_str());
    }
    printf("\nresponse 3 ..............\n");
    vec = hexStringToArray(hex_huff_rsp3);
    ret = hpc6.decode(&vec[0], vec.size(), headers);
    if (ret < 0) {
        printf("decode failed\n");
        return 1;
    }
    printf("de_len=%d\n", ret);
    for (auto &kv : headers) {
        printf("%s: %s\n", kv.first.c_str(), kv.second.c_str());
    }
    return 0;
}
