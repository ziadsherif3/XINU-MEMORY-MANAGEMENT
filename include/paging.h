/* paging.h */

/* Variables and constant definitions */

#define PAGEDIR         0x00400000      /* Physical address of the page directory */
#define PAGETAB1        0x00401000      /* Physical address of the first page table */
#define PAGETAB2        0x00402000      /* Physical address of the second page table */
#define PAGETAB3        0x00403000      /* Physical address of the third page table */
#define PAGETAB4        0x00404000      /* Physical address of the fourth page table */
#define FREEFRAME       0x00405000      /* Physical address of the first free frame */
#define FRAMEEND        0x00FFFFFF      /* Last address of the physical memory */

#define NPFRAMES        3072            /* Number of page frames */

extern byte pageframes[];               /* Array to indicate whether the page frame is present or not */

/* Helper functions */

status paginginit();
int32 getemptyframe();

/* Inline helper functions */

#define indtoadd(I)         (I * 4096 + 0x00400000)     /* Convert the index of a page frame to its physical address */
#define isbadfid(I)         ((I < 0) || (I >= 3072))    /* Check whether the frame index is in the range 0 -> 3071 */
#define isframeused(I)      (pageframes[I])             /* Check whether the frame is used */