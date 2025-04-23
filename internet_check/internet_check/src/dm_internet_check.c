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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "internet_check.h"
#include "dm_internet_check.h"

#include <amxm/amxm.h>
#include <amxc/amxc_macros.h>

#define ME "internet-check"

void _dm_vlan_bind_changed(UNUSED const char* const sig_name,
                  UNUSED const amxc_var_t* const data,
                  UNUSED void* const priv) {
    dm_vlan_bind_changed();
}

void _dm_vlan_bind_added(UNUSED const char* const sig_name,
                  UNUSED const amxc_var_t* const data,
                  UNUSED void* const priv) {

    amxd_object_t* bindobj = amxd_dm_signal_get_object(get_dm(), data);
    amxd_object_t* inst_obj = NULL;

    when_null_trace(bindobj, exit, ERROR, "Data model object from signal %s is NULL", sig_name);
    inst_obj = amxd_object_get_instance(bindobj, NULL, GET_UINT32(data, "index"));
    when_null_trace(inst_obj, exit, ERROR, "bind object is NULL");

	//TODO
	//Setting VLAN binding here works
	
exit:
	return;
}

// Debugging - print events - activate in odl
void _dm_vlan_bind_print_event(const char* const sig_name,
                  const amxc_var_t* const data,
                  UNUSED void* const priv) {
    printf("event received - %s\n", sig_name);
    if(data != NULL) {
        printf("Event data = \n");
        fflush(stdout);
        amxc_var_dump(data, STDOUT_FILENO);
    }
}

void init_vlan_bind_module(void) {
    amxc_var_t ret;

    amxc_var_init(&ret);
    if(0 != amxm_execute_function("vlan-bind", "vlan-bind", "init", get_config(), &ret)) {
        SAH_TRACEZ_WARNING(ME, "Module failed to execute function init");
    }
    amxc_var_clean(&ret);
}

void dm_vlan_bind_changed(void) {
    amxc_var_t ret;
    amxc_var_t args;


    amxc_var_init(&ret);
    amxc_var_init(&args);
    amxc_var_set_type(&args, AMXC_VAR_ID_HTABLE);

	//TODO
	//Setting VLAN binding here works

    amxc_var_clean(&args);
    amxc_var_clean(&ret);
}


