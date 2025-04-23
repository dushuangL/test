/****************************************************************************
**
** SPDX-License-Identifier: BSD-2-Clause-Patent
**
** SPDX-FileCopyrightText: Copyright (c) 2022 SoftAtHome
**
** Redistribution and use in source and binary forms, with or
** without modification, are permitted provided that the following
** conditions are met:
**
** 1. Redistributions of source code must retain the above copyright
** notice, this list of conditions and the following disclaimer.
**
** 2. Redistributions in binary form must reproduce the above
** copyright notice, this list of conditions and the following
** disclaimer in the documentation and/or other materials provided
** with the distribution.
**
** Subject to the terms and conditions of this license, each
** copyright holder and contributor hereby grants to those receiving
** rights under this license a perpetual, worldwide, non-exclusive,
** no-charge, royalty-free, irrevocable (except for failure to
** satisfy the conditions of this license) patent license to make,
** have made, use, offer to sell, sell, import, and otherwise
** transfer this software, where such license applies only to those
** patent claims, already acquired or hereafter acquired, licensable
** by such copyright holder or contributor that are necessarily
** infringed by:
**
** (a) their Contribution(s) (the licensed copyrights of copyright
** holders and non-copyrightable additions of contributors, in
** source or binary form) alone; or
**
** (b) combination of their Contribution(s) with the work of
** authorship to which such Contribution(s) was added by such
** copyright holder or contributor, if, at the time the Contribution
** is added, such addition causes such combination to be necessarily
** infringed. The patent license shall not apply to any other
** combinations which include the Contribution.
**
** Except as expressly stated above, no rights or licenses from any
** copyright holder or contributor is granted under this license,
** whether expressly, by implication, estoppel or otherwise.
**
** DISCLAIMER
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
** CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
** INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
** MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
** DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR
** CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
** USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
** AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
** LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
** ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
** POSSIBILITY OF SUCH DAMAGE.
**
****************************************************************************/
#include <string.h>
#include <debug/sahtrace_macros.h>
#include <netmodel/client.h>
#include <amxc/amxc_macros.h>
#include <amxm/amxm.h>
#include "internet_check.h"
#include "dm_internet_check.h"

#define ME "internet-check"

typedef struct {
    amxd_dm_t* dm;
    amxo_parser_t* parser;
    amxm_shared_object_t* so;
} app_t;

static app_t app;

amxd_dm_t* get_dm(void) {
    return app.dm;
}

amxo_parser_t* get_parser(void) {
    return app.parser;
}

amxc_var_t* get_config(void) {
    amxo_parser_t* parser = get_parser();
    return parser != NULL ? &(parser->config) : NULL;
}

static bool load_module(const char* dir, const char* module, const char* controller) {
    const char* path = NULL;
    amxc_string_t path_str;
    int rv = -1;

    amxc_string_init(&path_str, 0);

    when_str_empty(module, exit);
    when_str_empty(dir, exit);

    amxc_string_setf(&path_str, "%s/%s.so", dir, module);
    path = amxc_string_get(&path_str, 0);

    SAH_TRACEZ_NOTICE(ME, "Load module[%s] for %s", path, controller);

    rv = amxm_so_open(&app.so, controller, path);
    if(rv != 0) {
        SAH_TRACEZ_ERROR(ME, "Failed to load module[%s]", path);
        goto exit;
    }

exit:
    amxc_string_clean(&path_str);
    return rv == 0;
}

static bool load_vlan_bind_module(amxd_object_t* obj) {
    const char* module = amxc_var_constcast(cstring_t, amxd_object_get_param_value(obj, "VlanBindController"));
    const char* dir = GETP_CHAR(get_config(), "modules.vlan-bind-directory");

    return load_module(dir, module, "vlan-bind");
}

static bool load_modules(amxd_object_t* obj) {
    bool rv = true;

    rv &= load_vlan_bind_module(obj);

    return rv;
}

static void unload_modules(void) {
    app.so = NULL;
    amxm_close_all();
}

static int init(amxd_dm_t* dm, amxo_parser_t* parser) {
    int rv = -1;

    memset(&app, 0, sizeof(app));
    app.dm = dm;
    app.parser = parser;
    when_false_trace(netmodel_initialize(), exit, ERROR, "Failed to initialize libnetmodel!");
    rv = 0;
exit:
    return rv;
}

static int cleanup(void) {
    //destroy_upnp_pmp_firewall_rules();
    unload_modules();
    netmodel_cleanup();
    app.dm = NULL;
    app.parser = NULL;
    return 0;
}

void _app_start(UNUSED const char* const sig_name,
                UNUSED const amxc_var_t* const data,
                UNUSED void* const priv) {
    SAH_TRACEZ_NOTICE(ME, "app-start");
    load_modules(amxd_dm_findf(get_dm(), "Internet"));
    init_vlan_bind_module();
    //dm_vlan_bind_changed();
}

int _main(int reason, amxd_dm_t* dm, amxo_parser_t* parser) {
    int retval = 0;

    switch(reason) {
    case AMXO_START:
        SAH_TRACEZ_NOTICE(ME, "entrypoint - start");
        retval = init(dm, parser);
        break;

    case AMXO_STOP:
        SAH_TRACEZ_NOTICE(ME, "entrypoint - stop");
        retval = cleanup();
        break;
    }

    return retval;
}
