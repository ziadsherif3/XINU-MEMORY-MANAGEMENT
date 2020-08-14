/* getemptyframe.c - getemptyframe */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  getemptyframe  -  Get a free unused frame from the upper 3072 frames
 *------------------------------------------------------------------------
 */
int32	getemptyframe()
{
    static int32 nextframe = 0;     /* Next frame index to try */
    int32 frame;                    /* Index of the frame to be returned */
    int32 i;                        /* Iterate through # entries */
    intmask	mask;			        /* Saved interrupt mask		*/

    mask = disable();

    for (i = 0; i < NPFRAMES; i++) {
        frame = nextframe++;
        if (nextframe >= NPFRAMES) 
            nextframe = 0;
        if (!isframeused(frame)) {
            pageframes[frame] = 1;
            restore(mask);
            return indtoadd(frame);
        }
    }
    
    restore(mask);
    return SYSERR;
}