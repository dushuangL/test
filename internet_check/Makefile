include $(TOPDIR)/rules.mk

PKG_NAME:=internet_check
PKG_VERSION:=v1.0
SHORT_DESCRIPTION:=Implementation of Device


PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-v1.0


COMPONENT:=internet_check

PKG_RELEASE:=1
PKG_BUILD_DEPENDS += libamxb

define SAHInit/Install
	install -d ${PKG_INSTALL_DIR}/etc/rc.d/
	ln -sfr ${PKG_INSTALL_DIR}/etc/init.d/$(COMPONENT) ${PKG_INSTALL_DIR}/etc/rc.d/S$(CONFIG_SAH_AMX_VLANBIND_ORDER)$(COMPONENT)
	ln -sfr ${PKG_INSTALL_DIR}/etc/init.d/$(COMPONENT) ${PKG_INSTALL_DIR}/etc/rc.d/K$(CONFIG_SAH_AMX_VLANBIND_ORDER)$(COMPONENT)
endef

include $(INCLUDE_DIR)/package.mk
include $(TOPDIR)/package/tinno/tinno.mk

define Package/$(PKG_NAME)
  #CATEGORY:=prpl Foundation
  #SUBMENU:=TR-181 Managers
  SECTION := tinno
  CATEGORY := $(TINNO_SOFTWARE_CATEGORY)
  SUBMENU := $(TINNO_APPLICATIONS)
  TITLE:=$(SHORT_DESCRIPTION)
#  URL:=https://gitlab.com/prpl-foundation/components/core/plugins/wan-manager
  DEPENDS += +libamxc
  DEPENDS += +libamxp
  DEPENDS += +libamxd
  DEPENDS += +libamxb
  DEPENDS += +libamxm
  DEPENDS += +libamxo
  DEPENDS += +libamxj
  DEPENDS += +amxrt
  DEPENDS += +libsahtrace
  DEPENDS += +libnetmodel
  DEPENDS += +mod-dmext
  DEPENDS += +libipat
  DEPENDS += +rpcd
  MENU:=1
endef

define Package/$(PKG_NAME)/description
	TR-181 compatible plugin for internet_check configuration
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./$(PKG_NAME)/* $(PKG_BUILD_DIR)/
endef


define Build/Compile
	$(call Build/Compile/Default, STAGINGDIR=$(STAGING_DIR) CONFIGDIR=$(STAGING_DIR) PKG_CONFIG_PATH=$(STAGING_DIR)/usr/lib/pkgconfig LIBDIR=/usr/lib INSTALL_LIB_DIR=/lib INSTALL_BIN_DIR=/bin RAW_VERSION=$(PKG_VERSION) HARDCO_HAL_DIR=$(STAGING_DIR)/usr/include CONFIG_SAH_AMX_VLANBIND_ORDER=$(CONFIG_SAH_AMX_VLANBIND_ORDER) CONFIG_SAH_AMX_VLANBIND_RUN_AS_USER=$(CONFIG_SAH_AMX_VLANBIND_RUN_AS_USER) CONFIG_SAH_AMX_VLANBIND_RUN_AS_GROUP=$(CONFIG_SAH_AMX_VLANBIND_RUN_AS_GROUP))
endef

define Build/Install
	$(call Build/Install/Default, install INSTALL=install D=$(PKG_INSTALL_DIR) DEST=$(PKG_INSTALL_DIR) STAGINGDIR=$(STAGING_DIR) CONFIGDIR=$(STAGING_DIR) PV=$(PKG_VERSION) PKG_CONFIG_LIBDIR=$(STAGING_DIR)/usr/lib/pkgconfig LIBDIR=/usr/lib INSTALL_LIB_DIR=/lib INSTALL_BIN_DIR=/bin RAW_VERSION=$(PKG_VERSION) HARDCO_HAL_DIR=$(STAGING_DIR)/usr/include CONFIG_SAH_AMX_VLANBIND_ORDER=$(CONFIG_SAH_AMX_VLANBIND_ORDER) CONFIG_SAH_AMX_VLANBIND_RUN_AS_USER=$(CONFIG_SAH_AMX_VLANBIND_RUN_AS_USER) CONFIG_SAH_AMX_VLANBIND_RUN_AS_GROUP=$(CONFIG_SAH_AMX_VLANBIND_RUN_AS_GROUP))

	$(call SAHInit/Install)
endef

define Build/InstallDev
	$(call Build/Install/Default, install INSTALL=install D=$(STAGING_DIR) DEST=$(STAGING_DIR) STAGINGDIR=$(STAGING_DIR) CONFIGDIR=$(STAGING_DIR) PV=$(PKG_VERSION) PKG_CONFIG_LIBDIR=$(STAGING_DIR)/usr/lib/pkgconfig LIBDIR=/usr/lib INSTALL_LIB_DIR=/lib INSTALL_BIN_DIR=/bin RAW_VERSION=$(PKG_VERSION) HARDCO_HAL_DIR=$(STAGING_DIR)/usr/include CONFIG_SAH_AMX_VLANBIND_ORDER=$(CONFIG_SAH_AMX_VLANBIND_ORDER) CONFIG_SAH_AMX_VLANBIND_RUN_AS_USER=$(CONFIG_SAH_AMX_VLANBIND_RUN_AS_USER) CONFIG_SAH_AMX_VLANBIND_RUN_AS_GROUP=$(CONFIG_SAH_AMX_VLANBIND_RUN_AS_GROUP))
endef

define Package/$(PKG_NAME)/install
	$(CP) $(PKG_INSTALL_DIR)/* $(1)/
	if [ -d ./files ]; then \
		$(CP) ./files/* $(1)/; \
	fi
	find $(1) -name *.a -exec rm {} +;
	find $(1) -name *.h -exec rm {} +;
	find $(1) -name *.pc -exec rm {} +;
endef

define Package/$(PKG_NAME)/config
	source "$(SOURCE)/Config.in"
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
