/*
    Copyright (c) 2013 Emil Renner Berthing <nanomsg@esmil.dk>

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom
    the Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
    IN THE SOFTWARE.
*/

#include "../nn.h"

#include "../inproc.h"
#include "../ipc.h"
#include "../tcp.h"

#include "../pair.h"
#include "../pubsub.h"
#include "../reqrep.h"
#include "../pipeline.h"
#include "../survey.h"
#include "../bus.h"

struct nn_sockoptentry {
    int option;
    int type;
};

static const struct nn_sockoptentry nn_sockopt_sol_socket [] = {
    { NN_LINGER,            NN_OPTTYPE_INT },
    { NN_SNDBUF,            NN_OPTTYPE_INT },
    { NN_RCVBUF,            NN_OPTTYPE_INT },
    { NN_SNDTIMEO,          NN_OPTTYPE_INT },
    { NN_RCVTIMEO,          NN_OPTTYPE_INT },
    { NN_RECONNECT_IVL,     NN_OPTTYPE_INT },
    { NN_RECONNECT_IVL_MAX, NN_OPTTYPE_INT },
    { NN_SNDPRIO,           NN_OPTTYPE_INT },
    { NN_IPV4ONLY,          NN_OPTTYPE_INT },
};

static const struct nn_sockoptentry nn_sockopt_req [] = {
    { NN_REQ_RESEND_IVL,    NN_OPTTYPE_INT },
};

static const struct nn_sockoptentry nn_sockopt_sub [] = {
    { NN_SUB_SUBSCRIBE,     NN_OPTTYPE_STRING },
    { NN_SUB_UNSUBSCRIBE,   NN_OPTTYPE_STRING },
};

static const struct nn_sockoptentry nn_sockopt_surveyor [] = {
    { NN_SURVEYOR_DEADLINE, NN_OPTTYPE_INT },
};

static const struct nn_sockoptentry nn_sockopt_tcp [] = {
    { NN_TCP_NODELAY,       NN_OPTTYPE_INT },
};

#define NN_LEVELENTRY(val, opts) \
    { val, sizeof(opts)/sizeof(struct nn_sockoptentry), opts }
static const struct {
    int level;
    unsigned int length;
    const struct nn_sockoptentry *options;
} nn_levelopts [] = {
    NN_LEVELENTRY(NN_SOL_SOCKET, nn_sockopt_sol_socket),
    NN_LEVELENTRY(NN_REQ,        nn_sockopt_req),
    NN_LEVELENTRY(NN_SUB,        nn_sockopt_sub),
    NN_LEVELENTRY(NN_SURVEYOR,   nn_sockopt_surveyor),
    NN_LEVELENTRY(NN_TCP,        nn_sockopt_tcp),
};
#define NN_LEVELOPTS (sizeof(nn_levelopts)/sizeof(nn_levelopts[0]))

int nn_sockopttype (int level, int option)
{
    unsigned int i;
    unsigned int len;
    const struct nn_sockoptentry *opts;

    for (i = 0; i < NN_LEVELOPTS; i++) {
        if (level == nn_levelopts[i].level)
            break;
    }
    if (i == NN_LEVELOPTS)
        return NN_OPTTYPE_NOTFOUND;

    len = nn_levelopts[i].length;
    opts = nn_levelopts[i].options;
    for (i = 0; i < len; i++) {
        if (option == opts[i].option)
            return opts[i].type;
    }
    return NN_OPTTYPE_NOTFOUND;
}
