#
# Copyright (C) 2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=chrpath
PKG_VERSION:=0.13

PKG_SOURCE:=$(PKG_NAME)_$(PKG_VERSION).orig.tar.gz
PKG_SOURCE_URL:=http://ftp.debian.org/debian/pool/main/c/chrpath
PKG_MD5SUM:=b73072a8fbba277558c50364b65bb407

include $(INCLUDE_DIR)/host-build.mk

$(eval $(call HostBuild))
