/* pagingclient.c - connectiontest */

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