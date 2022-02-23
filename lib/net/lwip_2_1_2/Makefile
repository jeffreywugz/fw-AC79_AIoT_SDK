ifeq ($(CONFIG_NO_SDRAM_ENABLE),y)

AR_OUT = $(AR_DIR)/lwip_2_1_2_sfc.a

CC_DEFINE +=\
  -DLIWP_USE_SMALL_MEMORY

else

AR_OUT = $(AR_DIR)/lwip_2_1_2.a

endif


CC_DEFINE +=\
  -D_POSIX_SOURCE \
  -D_SYS_SELECT_H \


ifeq ($(ARCH),pi32v2)
CC_ARGS += \
	-ffunction-sections \
	-Wno-unused-value \
	-Os\

endif


includes = \
-I$(ROOT)/include_lib/system \
-I$(ROOT)/include_lib/system/generic \
-I$(ROOT)/include_lib/system/device \
-I$(ROOT)/include_lib/driver/cpu/$(CPU) \
-I$(ROOT)/include_lib \
-I$(ROOT)/include_lib/utils \
-I$(ROOT)/include_lib/driver \
-I$(ROOT)/include_lib/net \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/src/include \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/port \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/src/include/netif \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/port/arch \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/port \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/app/ctp \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/app/cdp \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/app/dhcp_srv \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/app/dtp \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/app/sock_api \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/app/udtp \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/src/include/lwip \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/app \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/app/mssdp \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/app/ddns \
-I$(ROOT)/include_lib/net/lte_module \


objs = \
./src/core/altcp.o\
./src/core/altcp_alloc.o\
./src/core/altcp_tcp.o\
./src/core/dns.o\
./src/core/stats.o\
./src/core/timeouts.o\
./src/core/memp.o\
./src/core/def.o\
./src/core/ipv4/icmp.o\
./src/core/ipv4/ip4_frag.o\
./src/core/ipv4/ip4.o\
./src/core/ipv4/dhcp.o\
./src/core/ipv4/etharp.o\
./src/core/ipv4/ip4_addr.o\
./src/core/ipv4/igmp.o\
./src/core/ipv4/autoip.o\
./src/core/sys.o\
./src/core/tcp_out.o\
./src/core/init.o\
./src/core/inet_chksum.o\
./src/core/mem.o\
./src/core/udp.o\
./src/core/netif.o\
./src/core/ipv6/nd6.o\
./src/core/ipv6/dhcp6.o\
./src/core/ipv6/icmp6.o\
./src/core/ipv6/ip6_addr.o\
./src/core/ipv6/ip6_frag.o\
./src/core/ipv6/inet6.o\
./src/core/ipv6/mld6.o\
./src/core/ipv6/ip6.o\
./src/core/ipv6/ethip6.o\
./src/core/tcp_in.o\
./src/core/raw.o\
./src/core/pbuf.o\
./src/core/tcp.o\
./src/core/ip.o\
./src/api/netdb.o\
./src/api/netifapi.o\
./src/api/tcpip.o\
./src/api/sockets.o\
./src/api/netbuf.o\
./src/api/err.o\
./src/api/api_lib.o\
./src/api/api_msg.o\
./src/api/if_api.o\
./src/netif/bridgeif.o\
./src/netif/bridgeif_fdb.o\
./src/netif/ethernet.o\
./src/netif/slipif.o\
./src/netif/zepif.o\
./src/netif/lowpan6.o\
./src/netif/lowpan6_ble.o\
./src/netif/lowpan6_common.o\
./src/netif/ppp/eap.o\
./src/netif/ppp/pppol2tp.o\
./src/netif/ppp/chap_ms.o\
./src/netif/ppp/ecp.o\
./src/netif/ppp/ipcp.o\
./src/netif/ppp/ccp.o\
./src/netif/ppp/chap-md5.o\
./src/netif/ppp/ppp.o\
./src/netif/ppp/demand.o\
./src/netif/ppp/fsm.o\
./src/netif/ppp/magic.o\
./src/netif/ppp/pppcrypt.o\
./src/netif/ppp/mppe.o\
./src/netif/ppp/vj.o\
./src/netif/ppp/chap-new.o\
./src/netif/ppp/auth.o\
./src/netif/ppp/upap.o\
./src/netif/ppp/pppoe.o\
./src/netif/ppp/pppapi.o\
./src/netif/ppp/eui64.o\
./src/netif/ppp/polarssl/md5.o\
./src/netif/ppp/polarssl/sha1.o\
./src/netif/ppp/polarssl/md4.o\
./src/netif/ppp/polarssl/des.o\
./src/netif/ppp/polarssl/arc4.o\
./src/netif/ppp/pppos.o\
./src/netif/ppp/ipv6cp.o\
./src/netif/ppp/utils.o\
./src/netif/ppp/lcp.o\
./src/netif/ppp/multilink.o\
../$(LWIP_DIR)/port/LwIP.o\
../$(LWIP_DIR)/port/sys_arch.o\
../$(LWIP_DIR)/port/wireless_ethernetif.o\
../$(LWIP_DIR)/port/lte_ethernetif.o\
../$(LWIP_DIR)/apps/sock_api/sock_api.o\
../$(LWIP_DIR)/apps/dhcp_srv/dhcp_srv.o\
../$(LWIP_DIR)/apps/ctp/ctp_server.o\
../$(LWIP_DIR)/apps/ctp/ctp_clinent.o\
../$(LWIP_DIR)/apps/cdp/cdp_server.o\
../$(LWIP_DIR)/apps/mssdp/mssdp.o\
../$(LWIP_DIR)/apps/ntp/ntp.o\
../$(LWIP_DIR)/apps/ping/ping.o\




obj_ls = \


obj_bs = \

objs_ver = version.o

GEN_LIB = y

include $(MAKE_RULE)
