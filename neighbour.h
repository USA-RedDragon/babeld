/*
Copyright (c) 2007, 2008 by Juliusz Chroboczek

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

struct hello_history {
    unsigned short reach;
    unsigned short interval;    /* in centiseconds */
    int seqno;
    struct timeval time;
};

struct neighbour {
    struct neighbour *next;
    /* This is -1 when unknown, so don't make it unsigned */
    unsigned char address[16];
    struct hello_history hello;
    struct hello_history uhello; /* for Unicast Hellos */
    unsigned short txcost;
    unsigned short hello_seqno; /* for Unicast Hellos */
    struct timeval ihu_time;
    unsigned short ihu_interval;   /* in centiseconds */
    /* Used for RTT estimation. */
    /* Absolute time (modulo 2^32) at which the Hello was sent,
       according to remote clock. */
    unsigned int hello_send_us;
    struct timeval hello_rtt_receive_time;
    struct timeval echo_receive_time;
    unsigned int rtt;
    struct timeval rtt_time;
    unsigned char pc_u[4], pc_m[4];
    int index_len; /* This is -1 when index is undefined */
    unsigned char index[32];
    struct interface *ifp;
    struct buffered buf;
};

extern struct neighbour *neighs;

#define FOR_ALL_NEIGHBOURS(_neigh) \
    for(_neigh = neighs; _neigh; _neigh = _neigh->next)

struct neighbour *find_neighbour(const unsigned char *address,
                                 struct interface *ifp);
int update_neighbour(struct neighbour *neigh, struct hello_history *hist,
                     int unicast, int hello, int hello_interval);
unsigned check_neighbours(void);
unsigned neighbour_txcost(struct neighbour *neigh);
unsigned neighbour_rxcost(struct neighbour *neigh);
unsigned neighbour_rttcost(struct neighbour *neigh);
unsigned neighbour_cost(struct neighbour *neigh);
int valid_rtt(struct neighbour *neigh);
