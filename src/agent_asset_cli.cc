/*  =========================================================================
    agent_rt_cli - Command line interface for agent-rt

    Copyright (C) 2014 - 2015 Eaton

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
    =========================================================================
*/

/*
@header
    agent_asset_cli - Command line interface for agent-asset
@discuss
@end
*/

#include "agent_asset_classes.h"
#include <time.h>

const char *endpoint = "ipc://@/malamute";
zpoller_t *poller = NULL;

static void
s_republish (mlm_client_t *client, int argn, int argc, char** argv)
{
    argn ++;
    zmsg_t *msg = zmsg_new ();
    if (argc == argn)
        zmsg_addstr (msg, "$all");
    else
        for (int i = argn; i != argc; i++)
            zmsg_addstr (msg, argv [i]);

    mlm_client_sendto (client, "asset-agent", "REPUBLISH", NULL, 1000, &msg);
}

int main (int argc, char *argv [])
{

    mlm_client_t *client = mlm_client_new ();
    assert (client);

    int rv = mlm_client_connect (client, endpoint, 1000, "CLI");
    if ( rv == -1 ) {
        zsys_error ("agent-rt-cli:\tCannot connect to malamute on '%s'", endpoint);
        mlm_client_destroy (&client);
        return -1;
    }

    bool verbose = false;
    for (int argn = 1; argn < argc; argn++) {
        if (    streq (argv [argn], "--help")
             || streq (argv [argn], "-h"))
        {
            puts ("agent-asset-cli [options]");
            puts ("agent-asset-cli republish");
            break;
        }
        else
        if (    streq (argv [argn], "--verbose")
             || streq (argv [argn], "-v"))
        {
            verbose = true;
        }
        else
        if (    streq (argv [argn], "republish")
             || streq (argv [argn], "repub"))
        {
            s_republish (client, argn, argc, argv);
            break;
        }
    }

    if (verbose)
        zsys_info ("agent-asset-cli - Command line interface for agent-rt");

    zclock_sleep (200);
    mlm_client_destroy (&client);
    return 0;
}