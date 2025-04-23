#
# This is the makefile building your odl files
#
-include ../makefile.inc

# Set default values if CONFIG_SAH_AMX_INTERNET_CHECK_RUN_AS_USER or CONFIG_SAH_AMX_INTERNET_CHECK_RUN_AS_GROUP is empty
ifeq ($(CONFIG_SAH_AMX_INTERNET_CHECK_RUN_AS_USER)),)
    CONFIG_SAH_AMX_INTERNET_CHECK_RUN_AS_USER := "root"
endif

ifeq ($(strip $(CONFIG_SAH_AMX_INTERNET_CHECK_RUN_AS_GROUP)),)
    CONFIG_SAH_AMX_INTERNET_CHECK_RUN_AS_GROUP := "root"
endif

all:
	m4 -D USER_ID=$(CONFIG_SAH_AMX_INTERNET_CHECK_RUN_AS_USER) -D GROUP_ID=$(CONFIG_SAH_AMX_INTERNET_CHECK_RUN_AS_GROUP) internet_check_caps.odl.m4 > internet_check_caps.odl	#m4 -D USER_ID=$(CONFIG_SAH_SERVICES_AMX-HOSTS-MANAGER_RUN_AS_USER) -D GROUP_ID=$(CONFIG_SAH_SERVICES_AMX-HOSTS-MANAGER_RUN_AS_GROUP) hosts-manager_caps.odl.m4 > hosts-manager_caps.odl
clean:
	rm -rf internet_check_caps.odl

#
# All these targets must be run regardless of the presence of a file
# named after them.
#
.PHONY: all clean
