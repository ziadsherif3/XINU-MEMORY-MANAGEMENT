/* paging.h */

/* Variables, constants definitions, and typdefs */

typedef	uint32	bsd_t;                      /* Backing store descriptor type typedef */

#define PAGEDIR             0x00400000      /* Physical address of the page directory */
#define PAGETAB1            0x00401000      /* Physical address of the first page table */
#define PAGETAB2            0x00402000      /* Physical address of the second page table */
#define PAGETAB3            0x00403000      /* Physical address of the third page table */
#define PAGETAB4            0x00404000      /* Physical address of the fourth page table */
#define FREEFRAME           0x00405000      /* Physical address of the first free frame */
#define FRAMEEND            0x00FFFFFF      /* Last address of the physical memory */

#define NPFRAMES            3072            /* Number of page frames */
#define NPAGES              4096            /* Number of pages in the physical memory */

#define PAGING_SERVER_IP    "192.168.1.42"  /* IP address of the remote paging server */
#define PAGING_SERVER_PORT  49188           /* Port number of the remote paging server */

#define GETBS               0               /* Packet type of get_bs */
#define RELEASEBS           1               /* Packet type of get_bs */
#define READBS              2               /* Packet type of get_bs */
#define WRITEBS             3               /* Packet type of get_bs */

#define PAGESERVERERR       0               /* Paging server error */

extern byte pageframes[];                   /* Array to indicate whether the page frame is present or not */

/* Helper functions */

extern status paginginit();
extern int32 getemptyframe();
extern status connectiontest();
extern int32 get_bs(bsd_t, uint32);
extern int32 release_bs(bsd_t);
extern syscall read_bs(char *, bsd_t, int32);
extern syscall write_bs(char *, bsd_t, int32);

/* Inline helper functions */

#define indtoadd(I)         (I * 4096 + 0x00400000)     /* Convert the index of a page frame to its physical address */
#define addtoind(A)         ((A - 0x00400000) / 4096)   /* Convert the physical address of a page frame to its index */
#define isbadfid(I)         ((I < 0) || (I >= 3072))    /* Check whether the frame index is in the range 0 -> 3071 */
#define isframeused(I)      (pageframes[I])             /* Check whether the frame is used */

/* Definition of the inverted page table entry */

struct invpagent {
    pid32 procid;       /* ID of the process owning the page frame */
    int32 vpagenum;     /* Virtual page number in the backing store */
};

extern byte pageframes[];                   /* Array to indicate whether the page frame is present or not */
extern struct invpagent invpagetab[];       /* Inverted page table */