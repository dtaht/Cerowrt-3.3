--- linux-3.3.3/include/linux/netfilter/Kbuild.orig	2012-04-26 00:24:57.581217964 +0100
+++ linux-3.3.3/include/linux/netfilter/Kbuild	2012-04-26 00:25:34.301606804 +0100
@@ -49,6 +49,7 @@ header-y += xt_hashlimit.h
 header-y += xt_helper.h
 header-y += xt_iprange.h
 header-y += xt_ipvs.h
+header-y += xt_layer7.h
 header-y += xt_length.h
 header-y += xt_limit.h
 header-y += xt_mac.h
