#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <limits.h>



typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;


/*This is use to get the flag of the CPSR register*/
#define NFLAG_MASK (u32) 1 << 31
#define ZFLAG_MASK (u32) 1 << 30
#define CFLAG_MASK (u32) 1 << 29
#define VFLAG_MASK (u32) 1 << 28

#define SET_NFLAG(x) (u32) (x << 31 & NFLAG_MASK)
#define SET_ZFLAG(x) (u32) (x << 30 & ZFLAG_MASK)
#define SET_CFLAG(x) (u32) (x << 29 & CFLAG_MASK)
#define SET_VFLAG(x) (u32) (x << 28 & VFLAG_MASK)

/*This is the masks for data processing instruction */
#define COND_MASK            (u32)         0xFu<<28
#define IMMED_OP_MASK        (u32)         0x1u<<24
#define OP_CODE_MASK         (u32)         0xFu<<20
#define S_MASK               (u32)         0x1u<<19
#define FIRST_OP_REG_MASK    (u32)         0xFu<<15
#define DIS_REG_MASK         (u32)         0xFu<<11

#define SET_COND(x)              (u32)          (x << 28 & COND_MASK)
#define SET_IMMED_OP_MASK        (u32)          (x << 24 & IMMED_OP_MASK)
#define SET_OP_CODE(x)           (u32)          (x << 20 & OP_COND_MASK)
#define SET_S(x)                 (u32)          (x << 19 & S_MASK)
#define SET_FIRST_OP_REG_MASK(x) (u32)          (x << 15 & FIRST_OP_REG_MASK)
#define SET_DIS_REG_MASK(x)      (u32)          (x <<11 & DIS_REG_MASK)
#define SET_OPRAND2(x)           (u32)          (x)


void printBit(uint32_t x){
    int i;
    uint32_t mask = 1 << 7;
    for(i = 0;i < 8;++i){
        printf("%i", (x & mask) != 0);
        x<<=1;
    }
    printf("\n");
}

void loadbinaryFile(char* address){
    unsigned char *combuffer;
    u32 size;
    FILE *ifp;

    ifp = fopen(address, "rb");

    fseek(ifp, 0, SEEK_END);
    size = ftell(ifp);
    fseek(ifp, 0, SEEK_SET);
    combuffer = (unsigned char *) malloc(size);
    combuffer = malloc(size);

    if(ifp == NULL){
        fprintf(stderr,"Unable to read file!");
        exit(EXIT_FAILURE);
    }

    fread(combuffer, 1,size,ifp);

    for(int i = 0; i < size; i++) {
        printf("%x ",combuffer[i]);
        printBit(combuffer[i]);
        printf("\n");
    }

    fclose(ifp);
    free(combuffer);
}




int main(int argc,  char **argv) {
    char address[400];
    strcpy(address, "/homes/hxm16/arm_1617_testsuite/test_cases/add01");
    loadbinaryFile(address);

  return EXIT_SUCCESS;
}









