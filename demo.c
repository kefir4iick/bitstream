#include "bitstream.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

static void print_bytes(const char *label, const uint8_t *x, size_t n)
{
    printf("%s:", label);
    for (size_t i = 0; i < n; i++)
        printf(" %02X", x[i]);
    printf("\n");
}

int main(void) {
    int fd = open("demo.bin", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    BitStream *bs = BitStreamOpenFD(fd, 1);

    uint8_t s1[] = {0xD6, 0x98};  
    WriteBitSequence(bs, s1, 13);

    uint8_t s2[] = {0xBC, 0x55, 0x80};
    WriteBitSequence(bs, s2, 17);

    uint8_t s3[] = {0xE5, 0xCD, 0x59, 0xC8};
    WriteBitSequence(bs, s3, 29);

    uint8_t s4[] = {0xAF, 0x16, 0x9E, 0xB4, 0xB6, 0x80};
    WriteBitSequence(bs, s4, 41);

    WriteUInt64(bs, 0x123456789ABCDEFULL, 60);

    BitStreamClose(bs);
    close(fd);

    printf("recording end\n");

    fd = open("demo.bin", O_RDONLY);
    bs = BitStreamOpenFD(fd, 0);

    uint8_t r1[2], r2[3], r3[4], r4[6];

    ReadBitSequence(bs, r1, 13);
    ReadBitSequence(bs, r2, 17);
    ReadBitSequence(bs, r3, 29);
    ReadBitSequence(bs, r4, 41);

    print_bytes("read S1 (13 bits)", r1, 2);
    print_bytes("read S2 (17 bits)", r2, 3);
    print_bytes("read S3 (29 bits)", r3, 4);
    print_bytes("read S4 (41 bits)", r4, 6);

    uint64_t x = ReadUInt64(bs, 60);
    printf("read uint64 (60 bits): %lX\n", x);

    BitStreamClose(bs);
    close(fd);

    fd = open("demo.bin", O_RDONLY);
    bs = BitStreamOpenFD(fd, 0);

    uint8_t a[4], b[5], c[6];

    ReadBitSequence(bs, a, 21);
    ReadBitSequence(bs, b, 37);
    ReadBitSequence(bs, c, 45);

    print_bytes("read first 21 bits", a, 4);
    print_bytes("read next 37 bits", b, 5);
    print_bytes("read next 45 bits", c, 6);

    BitStreamClose(bs);
    close(fd);

    printf("demo end\n");
    return 0;
}
