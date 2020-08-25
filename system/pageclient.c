/* pagingclient.c - connectiontest, get_bs, release_bs, read_bs, write_bs */

#include <xinu.h>

/*-------------------------------------------------------------------------
 *  connectiontest  -  Test the connection with the paging server
 *-------------------------------------------------------------------------
 */
status	connectiontest()
{
    int	i;			/* Index into buffer */
	int	retval;			/* Return value */
	char	msg[] = "Xinu testing connection with the paging server";   /* Message to send */
	char	inbuf[1500];		/* Buffer for incoming reply */
	int32	slot;			/* UDP slot to use */
	int32	msglen;			/* Length of outgoing message */
	uint32	remoteip;		/* Remote IP address to use */
	uint16	locport	= 52743;	/* Local port to use */
	int32	retries	= 3;		/* Number of retries */
	int32	delay	= 2000;		/* Reception delay in ms */

    /* Convert IP string into uint32 */

    if (dot2ip(PAGING_SERVER_IP, &remoteip) == SYSERR) {
		fprintf(stderr, "Invalid IP address argument\r\n");
		return SYSERR;
	}

    /* Register local UDP port */

	slot = udp_register(remoteip, PAGING_SERVER_PORT, locport);
	if (slot == SYSERR) {
		fprintf(stderr, "Could not reserve UDP port %d\n", locport);
		return SYSERR;
	}

    /* Retry sending outgoing datagram and getting response */

	msglen = strnlen(msg, 1200);
	for (i = 0; i < retries; i++) {
		retval = udp_send(slot, msg, msglen);
		if (retval == SYSERR) {
			fprintf(stderr, "Error sending UDP\n");
			return SYSERR;
		}

		retval = udp_recv(slot, inbuf, sizeof(inbuf), delay);
		if (retval == TIMEOUT) {
			fprintf(stderr, "Timeout...\n");
			continue;
		} else if (retval == SYSERR) {
			fprintf(stderr, "Error from udp_recv\n");
			udp_release(slot);
			return SYSERR;
		}
		break;
	}

    udp_release(slot);
	if (retval == TIMEOUT) {
		fprintf(stderr, "Retry limit exceeded\n");
		return SYSERR;
	}

    /* Response received - check contents */

    fprintf(stdout, "Received message: %s\n", inbuf);

    return OK;
}

/*----------------------------------------------------------------
 *  get_bs  -  Request from the paging server a new backing store
 *----------------------------------------------------------------
 */
int32 get_bs(bsd_t store, uint32 npages)
{
	int	i;			/* Index into buffer */
	int	retval;			/* Return value */
	uint32	data[3];   /* Data to send */
	uint32	inbuf[2];		/* Buffer for incoming reply */
	int32	slot;			/* UDP slot to use */
	uint32	remoteip;		/* Remote IP address to use */
	uint16	locport	= 52743;	/* Local port to use */
	int32	retries	= 3;		/* Number of retries */
	int32	delay	= 2000;		/* Reception delay in ms */

    /* Convert IP string into uint32 */

    if (dot2ip(PAGING_SERVER_IP, &remoteip) == SYSERR) {
		fprintf(stderr, "Invalid IP address argument\r\n");
		return SYSERR;
	}

	/* Register local UDP port */

	slot = udp_register(remoteip, PAGING_SERVER_PORT, locport);
	if (slot == SYSERR) {
		fprintf(stderr, "Could not reserve UDP port %d\n", locport);
		return SYSERR;
	}

	/* Retry sending outgoing datagram and getting response */

	data[0] = GETBS;
	data[1] = store;
	data[2] = npages;

	for (i = 0; i < retries; i++) {
		retval = udp_send(slot, (char *) data, sizeof(data));
		if (retval == SYSERR) {
			fprintf(stderr, "Error sending UDP\n");
			return SYSERR;
		}

		retval = udp_recv(slot, (char *) inbuf, sizeof(inbuf), delay);
		if (retval == TIMEOUT) {
			fprintf(stderr, "Timeout...\n");
			continue;
		} else if (retval == SYSERR) {
			fprintf(stderr, "Error from udp_recv\n");
			udp_release(slot);
			return SYSERR;
		}
		break;
	}

    udp_release(slot);
	if (retval == TIMEOUT) {
		fprintf(stderr, "Retry limit exceeded\n");
		return SYSERR;
	}

	/* Parse incoming packet */

	if (inbuf[0] == PAGESERVERERR) {
		return SYSERR;
	}

	return inbuf[1];

}

/*-------------------------------------------------------------------------------------------
 *  release_bs  -  Request from the paging server to release the backing store with ID store
 *-------------------------------------------------------------------------------------------
 */
int32 release_bs(bsd_t store)
{
	return OK;
}

/*-------------------------------------------------------------
 *  read_bs  -  Copy the pagenumth page from the backing store
 *-------------------------------------------------------------
 */
syscall read_bs(char *dst, bsd_t store, int32 pagenum)
{
	return OK;
}

/*----------------------------------------------------------------------
 *  write_bs  -  Copy a page to the pagenumth page of the backing store
 *----------------------------------------------------------------------
 */
syscall write_bs(char *src, bsd_t store, int32 pagenum)
{
	return OK;
}