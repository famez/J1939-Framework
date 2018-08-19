/* cmakeconfig.h.in */

#ifndef __CONFIG_H__
#define __CONFIG_H__

/* Note: You cannot use earlier #defines in later #cmakedefines (cmake 2.6.2). */

/* Name of package */
#define PACKAGE "wireshark"

#define VERSION_EXTRA ""

/* Version number of package */
#define VERSION "2.6.1"
#define VERSION_MAJOR 2
#define VERSION_MINOR 6
#define VERSION_MICRO 1

#define VERSION_RELEASE "2.6"
#define VERSION_FLAVOR ""

/* FIXME: Move the path stuff to the CMakeInstallDirs.cmake file */
/* Directory for data */
#define DATAFILE_DIR "/usr/local/share/wireshark"

/* Build wsutil with SIMD optimization */
#define HAVE_SSE4_2 1

/* Directory where extcap hooks reside */
#define EXTCAP_DIR "/usr/local/lib/wireshark/extcap"

/* Define to 1 if we want to enable plugins */
#define HAVE_PLUGINS 1

/*  Define to 1 if we check hf conflict */
/* #undef ENABLE_CHECK_FILTER */

/* Link Wireshark libraries statically */
/* #undef ENABLE_STATIC */

/* Enable AirPcap */
/* #undef HAVE_AIRPCAP */

/* Define to 1 if you have the <alloca.h> header file. */
/* #undef HAVE_ALLOCA_H */

/* Define to 1 if you have the <arpa/inet.h> header file. */
#define HAVE_ARPA_INET_H 1

/* Define to 1 if you have the `bpf_image' function. */
#define HAVE_BPF_IMAGE 1

/* Define to use c-ares library */
/* #undef HAVE_C_ARES */

/* Define to 1 if you have the `dladdr' function. */
/* #undef HAVE_DLADDR */

/* Define to 1 if you have the <fcntl.h> header file. */
#define HAVE_FCNTL_H 1

/* Define to use the MaxMind DB library */
/* #undef HAVE_MAXMINDDB */

/* Define to 1 if you have the <ifaddrs.h> header file. */
#define HAVE_IFADDRS_H 1

/* Define to 1 if you have the `getexecname' function. */
/* #undef HAVE_GETEXECNAME */

/* Define to 1 if you have the `getifaddrs' function. */
#define HAVE_GETIFADDRS 1

/* Define if LIBSSH support is enabled */
/* #undef HAVE_LIBSSH */

/* Define if LIBSSH has ssh_userauth_agent() function */
/* #undef HAVE_SSH_USERAUTH_AGENT */

/* Define if you have the 'floorl' function. */
#define HAVE_FLOORL 1

/* Define if you have the 'lrint' function. */
#define HAVE_LRINT 1

/* Define to 1 if you have the getopt_long function. */
#define HAVE_GETOPT_LONG 1

/* Define to 1 if you have the <getopt.h> header file. */
#define HAVE_GETOPT_H 1

/* Define to 1 if you have the <grp.h> header file. */
#define HAVE_GRP_H 1

/* Define to use heimdal kerberos */
/* #undef HAVE_HEIMDAL_KERBEROS */

/* Define to 1 if you have the `inflatePrime' function. */
#define HAVE_INFLATEPRIME 1

/* Define to 1 if you have the `issetugid' function. */
/* #undef HAVE_ISSETUGID */

/* Define to use kerberos */
/* #undef HAVE_KERBEROS */

/* Define to use nghttp2 */
/* #undef HAVE_NGHTTP2 */

/* Define to use the libcap library */
/* #undef HAVE_LIBCAP */

/* Define to use GnuTLS library */
/* #undef HAVE_LIBGNUTLS */

/* Enable libnl support */
/* #undef HAVE_LIBNL */

/* libnl version 1 */
/* #undef HAVE_LIBNL1 */

/* libnl version 2 */
/* #undef HAVE_LIBNL2 */

/* libnl version 3 */
/* #undef HAVE_LIBNL3 */

/* Define to use libpcap library */
#define HAVE_LIBPCAP 1

/* Define to 1 if you have the `smi' library (-lsmi). */
/* #undef HAVE_LIBSMI */

/* Define to use zlib library */
#define HAVE_ZLIB 1

/* Define to use lz4 library */
/* #undef HAVE_LZ4 */

/* Define to use snappy library */
/* #undef HAVE_SNAPPY */

/* Define to 1 if you have the <linux/sockios.h> header file. */
#define HAVE_LINUX_SOCKIOS_H 1

/* Define to 1 if you have the <linux/if_bonding.h> header file. */
#define HAVE_LINUX_IF_BONDING_H 1

/* Define to use Lua */
/* #undef HAVE_LUA */

/* Define to 1 if you have the <lua.h> header file. */
/* #undef HAVE_LUA_H */

/* Define to 1 if you have the <memory.h> header file. */
/* #undef HAVE_MEMORY_H */

/* Define to use MIT kerberos */
/* #undef HAVE_MIT_KERBEROS */

/* Define to 1 if you have the `mkstemps' function. */
#define HAVE_MKSTEMPS 1

/* Define to 1 if you have the `mmap' function. */
/* #undef HAVE_MMAP */

/* Define to 1 if you have the `mprotect' function. */
/* #undef HAVE_MPROTECT */

/* Define to 1 if you have the <netdb.h> header file. */
#define HAVE_NETDB_H 1

/* Define to 1 if you have the <netinet/in.h> header file. */
#define HAVE_NETINET_IN_H 1

/* nl80211.h is new enough */
/* #undef HAVE_NL80211 */

/* SET_CHANNEL is supported */
/* #undef HAVE_NL80211_CMD_SET_CHANNEL */

/* SPLIT_WIPHY_DUMP is supported */
/* #undef HAVE_NL80211_SPLIT_WIPHY_DUMP */

/* VHT_CAPABILITY is supported */
/* #undef HAVE_NL80211_VHT_CAPABILITY */

/* Define to 1 if you have macOS frameworks */
/* #undef HAVE_MACOS_FRAMEWORKS */

/* Define to 1 if you have the macOS CFPropertyListCreateWithStream function */
/* #undef HAVE_CFPROPERTYLISTCREATEWITHSTREAM */

/* Define if pcap_breakloop is known */
#define HAVE_PCAP_BREAKLOOP 1

/* Define to 1 if you have the `pcap_create' function. */
#define HAVE_PCAP_CREATE 1

/* Define to 1 if the capture buffer size can be set. */
#define CAN_SET_CAPTURE_BUFFER_SIZE 1

/* Define to 1 if you have the `pcap_datalink_name_to_val' function. */
#define HAVE_PCAP_DATALINK_NAME_TO_VAL 1

/* Define to 1 if you have the `pcap_datalink_val_to_description' function. */
#define HAVE_PCAP_DATALINK_VAL_TO_DESCRIPTION 1

/* Define to 1 if you have the `pcap_datalink_val_to_name' function. */
#define HAVE_PCAP_DATALINK_VAL_TO_NAME 1

/* Define to 1 if you have the `pcap_findalldevs' function and a pcap.h that
   declares pcap_if_t. */
#define HAVE_PCAP_FINDALLDEVS 1

/* Define to 1 if you have the `pcap_freecode' function. */
#define HAVE_PCAP_FREECODE 1

/* Define to 1 if you have the `pcap_free_datalinks' function. */
#define HAVE_PCAP_FREE_DATALINKS 1

/* Define to 1 if you have the `pcap_get_selectable_fd' function. */
#define HAVE_PCAP_GET_SELECTABLE_FD 1

/* Define to 1 if you have the `pcap_lib_version' function. */
#define HAVE_PCAP_LIB_VERSION 1

/* Define to 1 if you have the `pcap_list_datalinks' function. */
#define HAVE_PCAP_LIST_DATALINKS 1

/* Define to 1 if you have the `pcap_open' function. */
/* #undef HAVE_PCAP_OPEN */

/* Define to 1 if you have the `pcap_open_dead' function. */
#define HAVE_PCAP_OPEN_DEAD 1

/* Define to 1 if you have libpcap/WinPcap remote capturing support. */
/* #undef HAVE_PCAP_REMOTE */

/* Define to 1 if you have the `pcap_set_datalink' function. */
#define HAVE_PCAP_SET_DATALINK 1

/* Define to 1 if you have the `pcap_setsampling' function. */
/* #undef HAVE_PCAP_SETSAMPLING */

/* Define to 1 if you have the `pcap_set_tstamp_precision' function. */
#define HAVE_PCAP_SET_TSTAMP_PRECISION 1

/* Define to 1 if you have the `pcap_set_tstamp_type' function. */
#define HAVE_PCAP_SET_TSTAMP_TYPE 1

/* Define to 1 if you have the <pwd.h> header file. */
#define HAVE_PWD_H 1

/* Define to 1 if you have the optreset variable */
/* #undef HAVE_OPTRESET */

/* Define if sa_len field exists in struct sockaddr */
/* #undef HAVE_STRUCT_SOCKADDR_SA_LEN */

/* Define to 1 if you want to playing SBC by standalone BlueZ SBC library */
/* #undef HAVE_SBC */

/* Define to 1 if you have the SpanDSP library. */
/* #undef HAVE_SPANDSP */

/* Define to 1 if you have the bcg729 library. */
/* #undef HAVE_BCG729 */

/* Define to 1 if you have the lixbml2 library. */
/* #undef HAVE_LIBXML2 */

/* Define to 1 if you have the `setresgid' function. */
#define HAVE_SETRESGID 1

/* Define to 1 if you have the `setresuid' function. */
#define HAVE_SETRESUID 1

/* Define to 1 if you have the WinSparkle library */
/* #undef HAVE_SOFTWARE_UPDATE */

/* Define if you have the 'strptime' function. */
#define HAVE_STRPTIME 1

/* Define to 1 if `st_birthtime' is a member of `struct stat'. */
/* #undef HAVE_STRUCT_STAT_ST_BIRTHTIME */

/* Define if st_flags field exists in struct stat */
/* #undef HAVE_STRUCT_STAT_ST_FLAGS */

/* Define to 1 if `__st_birthtime' is a member of `struct stat'. */
/* #undef HAVE_STRUCT_STAT___ST_BIRTHTIME */

/* Define to 1 if you have the <sys/ioctl.h> header file. */
#define HAVE_SYS_IOCTL_H 1

/* Define to 1 if you have the <sys/socket.h> header file. */
#define HAVE_SYS_SOCKET_H 1

/* Define to 1 if you have the <sys/sockio.h> header file. */
/* #undef HAVE_SYS_SOCKIO_H */

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/time.h> header file. */
#define HAVE_SYS_TIME_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <sys/utsname.h> header file. */
#define HAVE_SYS_UTSNAME_H 1

/* Define to 1 if you have the <sys/wait.h> header file. */
#define HAVE_SYS_WAIT_H 1

/* Define if tm_zone field exists in struct tm */
#define HAVE_STRUCT_TM_TM_ZONE 1

/* Define if tzname array exists */
#define HAVE_TZNAME 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Name of package */
/* #undef PACKAGE */

/* Define to the address where bug reports for this package should be sent. */
/* #undef PACKAGE_BUGREPORT */

/* Define to the full name of this package. */
/* #undef PACKAGE_NAME */

/* Define to the full name and version of this package. */
/* #undef PACKAGE_STRING */

/* Define to the one symbol short name of this package. */
/* #undef PACKAGE_TARNAME */

/* Define to the version of this package. */
/* #undef PACKAGE_VERSION */

/* Support for pcapng */
#define PCAP_NG_DEFAULT 1

/* Define if we are using version of of the Portaudio library API */
/* #undef PORTAUDIO_API_1 */

/* Define if we have QtMultimedia */
#define QT_MULTIMEDIA_LIB 1

/* Define if we have QtMacExtras */
/* #undef QT_MACEXTRAS_LIB */

/* Define if we have QtWinExtras */
/* #undef QT_WINEXTRAS_LIB */

/* Build androiddump with libpcap instead of wireshark stuff */
/* #undef ANDROIDDUMP_USE_LIBPCAP */

/* Large file support */
/* #undef _LARGEFILE_SOURCE */
/* #undef _LARGEFILE64_SOURCE */
/* #undef _LARGE_FILES */
/* #undef _FILE_OFFSET_BITS */

/* Define to 1 if `lex' declares `yytext' as a `char *' by default, not a
   `char[]'. */
/* Note: not use in the code */
/* #undef YYTEXT_POINTER */

#if defined(_WIN32)
   /*
    * Make sure everyone is using the same API and that it's sufficient
    * for our needs.
    * This should match the following:
    * - The <compatibility><application> section in image\wireshark.exe.manifest.in
    * - The GetWindowsVersion parts of packaging\nsis\wireshark.nsi
    * - The VersionNT parts of packaging\wix\Prerequisites.wxi
    */
#  if defined(NTDDI_VERSION)
#    error NTDDI_VERSION already defined.
#  endif
#  define NTDDI_VERSION NTDDI_WIN7
#  if defined(_WIN32_WINNT)
#    error _WIN32_WINNT already defined.
#  endif
#  define _WIN32_WINNT _WIN32_WINNT_WIN7

   /* WpdPack/INclude/pcap/pcap.h checks for "#if defined(WIN32)" */
#  ifndef WIN32
#    define WIN32	1
#  endif

#  if !defined(QT_VERSION) || !defined(_SSIZE_T_DEFINED)
   typedef int ssize_t;
#  endif

   /*
    * Flex (v 2.5.35) uses this symbol to "exclude" unistd.h
    */
#  define YY_NO_UNISTD_H

#  define strncasecmp strnicmp
#  define popen       _popen
#  define pclose      _pclose

#  ifndef __STDC__
#    define __STDC__ 0
#  endif
   /* Use Unicode in Windows runtime functions. */
#  define UNICODE 1
#  define _UNICODE 1

#  define NEED_STRPTIME_H 1
#endif

#if defined(__APPLE__)
   /* This is to trigger the integration of objective-c
    * code builds for removing unnecessary menu entries
    * in 10.12 > and Qt 5.3 >
    */
#  define CMAKE_BUILD 1
#endif

#endif /* __CONFIG_H__ */
