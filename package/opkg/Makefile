#
# Copyright (C) 2006-2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk
include $(INCLUDE_DIR)/version.mk

PKG_NAME:=opkg
PKG_REV:=618
PKG_VERSION:=$(PKG_REV)
PKG_RELEASE:=3

PKG_SOURCE_PROTO:=svn
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_SUBDIR:=opkg-$(PKG_VERSION)
PKG_SOURCE_URL:=http://opkg.googlecode.com/svn/trunk/
PKG_SOURCE:=$(PKG_SOURCE_SUBDIR).tar.gz

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(BUILD_VARIANT)/$(PKG_NAME)-$(PKG_VERSION)
PKG_FIXUP:=autoreconf
PKG_REMOVE_FILES = autogen.sh aclocal.m4

PKG_BUILD_PARALLEL:=1
HOST_BUILD_PARALLEL:=1
PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/host-build.mk

define Package/opkg/Default
  SECTION:=base
  CATEGORY:=Base system
  TITLE:=opkg package management system
  MAINTAINER:=Jo-Philipp Wich <xm@subsignal.org>
  URL:=http://wiki.openmoko.org/wiki/Opkg
endef

define Package/opkg/Default/description
  Lightweight package management system
  opkg is the opkg Package Management System, for handling
  installation and removal of packages on a system. It can
  recursively follow dependencies and download all packages
  necessary to install a particular package.

  opkg knows how to install both .ipk and .deb packages.
endef

define Package/opkg
  $(call Package/opkg/Default)
  TITLE+= (without signing support)
  VARIANT:=unsigned
endef

define Package/opkg/description
  $(call Package/opkg/Default/description)
endef

define Package/opkg/conffiles
/etc/opkg.conf
endef

define Package/opkg-curl
  $(call Package/opkg/Default)
  TITLE+= (with cURL authentication support)
  DEPENDS+= +libcurl @BROKEN
  VARIANT:=curl
endef

define Package/opkg-curl/description
  $(call Package/opkg/Default/description)
endef

Package/opkg-curl/conffiles = $(Package/opkg/conffiles)

define Package/opkg-gpg
  $(call Package/opkg/Default)
  TITLE+= (with GPG signing support)
  DEPENDS+= +libgpgme
  VARIANT:=gpg
endef

define Package/opkg-gpg/description
  $(call Package/opkg/Default/description)

  This package allows the Package index to be verified with GPG.
endef

define Package/opkg-gpg/conffiles
$(call Package/opkg/conffiles)
/etc/opkg/secring.gpg
/etc/opkg/trustdb.gpg
/etc/opkg/trusted.gpg
endef

define Package/opkg-smime
  $(call Package/opkg/Default)
  TITLE+= (with S/MIME signing support)
  DEPENDS+= +libopenssl @USE_SMIME
  VARIANT:=smime
endef

define Package/opkg-smime/description
  $(call Package/opkg/Default/description)

  This package allows the Package index to be verified with S/MIME.
endef

Package/opkg-smime/conffiles = $(Package/opkg/conffiles)

TARGET_CFLAGS += $(if $(CONFIG_GCC_VERSION_4_3)$(CONFIG_GCC_VERSION_4_4),-Wno-array-bounds)
TARGET_CFLAGS += -ffunction-sections -fdata-sections
EXTRA_CFLAGS += $(TARGET_CPPFLAGS)

CONFIGURE_ARGS += \
	--with-opkgetcdir=/etc \
	--with-opkglockfile=/var/lock/opkg.lock

ifeq ($(BUILD_VARIANT),unsigned)
	CONFIGURE_ARGS += \
		--disable-curl \
		--disable-gpg
endif

ifeq ($(BUILD_VARIANT),curl)
	CONFIGURE_ARGS += \
		--disable-gpg \
		--enable-curl \
		--enable-ssl-curl
endif

ifeq ($(BUILD_VARIANT),gpg)
	CONFIGURE_ARGS += \
		--disable-curl \
		--disable-openssl \
		--enable-gpg
endif

ifeq ($(BUILD_VARIANT),smime)
	CONFIGURE_ARGS += \
		--disable-curl \
		--disable-gpg \
		--enable-openssl
endif

MAKE_FLAGS = \
		CC="$(TARGET_CC)" \
		DESTDIR="$(PKG_INSTALL_DIR)" \
		HOST_CPU="$(PKGARCH)" \
		LDFLAGS="-Wl,--gc-sections" 

define Package/opkg/Default/install
	$(INSTALL_DIR) $(1)/usr/lib/opkg
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_DATA) ./files/opkg$(2).conf $(1)/etc/opkg.conf
	$(VERSION_SED) $(1)/etc/opkg.conf
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/opkg-cl $(1)/bin/opkg
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libopkg.so.* $(1)/usr/lib/
endef

Package/opkg/install = $(call Package/opkg/Default/install,$(1),)

Package/opkg-curl/install = $(call Package/opkg/Default/install,$(1),-curl)

define Package/opkg-gpg/install
	$(call Package/opkg/Default/install,$(1),-gpg)
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/opkg-key $(1)/bin/
	$(INSTALL_DIR) $(1)/etc/opkg
#	gpg --export $(call qstrip,$(CONFIG_GPG_KEY)) | \
#		$(STAGING_DIR_HOST)/bin/opkg-key -o $(1) add -
endef

define Package/opkg-smime/install
	$(call Package/opkg/Default/install,$(1),-smime)
	$(SED) \
		's,@SMIME_CA@,$(shell basename $(CONFIG_SMIME_CA)),' \
		$(1)/etc/opkg.conf
	$(INSTALL_DIR) $(1)/etc/ssl/certs/
	$(INSTALL_CONF) \
		$(call qstrip,$(CONFIG_SMIME_CA)) \
		$(call qstrip,$(CONFIG_SMIME_INTERMEDIATE)) \
		$(1)/etc/ssl/certs/
	c_rehash $(1)/etc/ssl/certs/
endef

define Build/InstallDev
	mkdir -p $(1)/usr/include
	$(CP) $(PKG_INSTALL_DIR)/usr/include/libopkg $(1)/usr/include/
endef


HOST_CONFIGURE_ARGS+= \
	--disable-curl \
	--disable-gpg \
	--with-opkgetcdir=/etc \
	--with-opkglockfile=/tmp/opkg.lock

define Host/Compile
	$(MAKE) $(HOST_JOBS) -C $(HOST_BUILD_DIR) CC="$(HOSTCC)" all
endef

define Host/Install
	$(INSTALL_BIN) $(HOST_BUILD_DIR)/src/opkg-cl $(STAGING_DIR_HOST)/bin/opkg
	$(INSTALL_BIN) $(HOST_BUILD_DIR)/utils/opkg-key $(STAGING_DIR_HOST)/bin/
endef

$(eval $(call BuildPackage,opkg))
$(eval $(call BuildPackage,opkg-curl))
$(eval $(call BuildPackage,opkg-gpg))
$(eval $(call BuildPackage,opkg-smime))
$(eval $(call HostBuild))
