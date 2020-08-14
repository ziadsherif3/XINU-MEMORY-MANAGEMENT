/* paginginit.c - paginginit */

#include <xinu.h>

byte pageframes[NPFRAMES];

/*-------------------------------------------------------------------------
 *  paginginit  -  Initialize variables and data structures used in paging
 *-------------------------------------------------------------------------
 */
status	paginginit()
{
    int32* pte = (int32*) PAGETAB1;     /* Pointer to a 4 byte page table entry */
    int32* pde = (int32*) PAGEDIR;      /* Pointer to a 4 byte page directory entry */
    int32 i;                            /* Iterate through # entries */
    int32 pageadd;                      /* Address of a page in the physical memory */

    for (i = 0; i < 5; i++) { /* Mark the first 5 page frames as used */
        pageframes[i] = 1;
    }

    /* Fill the 4 page tables */

    for (i = 0; i < NPAGES; i++) {
        pageadd = i * NPAGES;
        pageadd |= 0x00000001;
        *pte = pageadd;
        pte += 1;
    }

    /* Fill the page directory */

    for (i = 0; i < 4; i++) {
        pageadd = i * NPAGES + PAGETAB1;
        pageadd |= 0x00000001;
        *pde = pageadd;
        pde += 1;
    }

    /* Enable paging */

    asm("movl %CR3, %eax;"
        "movl $0x00400000, %eax;"
        "movl %eax, %CR3;"
        "movl %CR4, %eax;"
        "andl $0xFFFFFFDF, %eax;"
        "movl %eax, %CR4;"
        "movl %CR0, %eax;"
        "orl $0x80000001, %eax;"
        "movl %eax, %CR0;");

    return OK;
}