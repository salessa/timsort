#include <stdio.h>
#include <stdint.h>

uint32_t getCpuCacheSize(void)
{
    uint32_t eax = 0x80000006;     /* L2 cahce feature */
    uint32_t ecx = 0;

    uint8_t  sCacheLineSize   = 0;
    uint16_t sCacheSizeInK    = 0;
    uint8_t  sL2Associativity = 0;

    /*
     * Because PIC(Position Independent Code) of i386 uses
     * ebx to store GOT(Global Offset Table),
     * you should preserve the content of EBX.
     *
     * Note that gcc on some platforms (e.g. gcc for Xeon)
     * use static object format instead of PIC, 
     * if not provided with explicit -fPIC.
     *
     * Used MOVL instead of PUSH/POP to preserve EBX, because x86-64 
     * assembly instruction is somewhat different than ia32 assembly.
     * PUSH and POP caused an assembler error when compiled with -m64.
     */
    asm("movl %%ebx, %%edi;"
        "cpuid;"
        "movl %%edi, %%ebx;"
        : "=c"(ecx)
        : "a"(eax)
        : "%edi");  /* Tell gcc that EDI has been changed inside asm block. */

    // (void)printf("ecx = 0x%08x\n", ecx);

    /*
     * cpuid instuction (IA32 Software Developer's Manual Vol.2A p.3-205 ~ p.3-206)
     * Opcode : EAX
     * If EAX is 0x80000006 when cpuid is called, 
     * EAX, EBX, EDX is reserved, and information is returned in ECX
     *
     * ECX 
     *      Bits 7-0: Cache Line size in bytes 
     *      Bits 15-12: L2 Associativity field * 
     *      Bits 31-16: Cache size in 1K units
     * 
     * L2 associativity field encodings: 
     *      00H - Disabled 
     *      01H - Direct mapped 
     *      02H - 2-way 
     *      04H - 4-way 
     *      06H - 8-way 
     *      08H - 16-way 
     *      0FH - Fully associative
     */
    sCacheLineSize   = ecx & 0xFF;
    sCacheSizeInK    = ecx >> 16;
    sL2Associativity = (ecx >> 12) & 0x0F;

    // (void)printf("L2 cache line size = %hhuBytes (0x%02x)\n", sCacheLineSize, sCacheLineSize);
    // (void)printf("L2 cache size = %huKBytes (0x%04x)\n", sCacheSizeInK, sCacheSizeInK);
    // (void)printf("L2 Associativity = 0x%02x\n", sL2Associativity);

    return sCacheSizeInK;
}

int32_t main(void)
{
    (void)printf("L2 cache size = %huKBytes\n", getCpuCacheSize());

    return 0;
}
