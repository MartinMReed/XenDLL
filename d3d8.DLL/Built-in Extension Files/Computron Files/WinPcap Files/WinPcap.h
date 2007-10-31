#ifndef WinPcap_H
#define WinPcap_H

#pragma once

#define WPCAP
#define HAVE_REMOTE
#include <pcap.h>

typedef int (WINAPI* f_pcap_createsrcstr)(char*, int, const char*, const char*, const char*, char*);
typedef pcap_t* (WINAPI* f_pcap_open)(const char*, int, int, int, pcap_rmtauth*, char*);
typedef int (WINAPI* f_pcap_datalink)(pcap_t*);
typedef pcap_send_queue* (WINAPI* f_pcap_sendqueue_alloc)(u_int);
typedef int (WINAPI* f_pcap_next_ex)(pcap_t*, pcap_pkthdr**, const u_char**);
typedef int (WINAPI* f_pcap_sendqueue_queue)(pcap_send_queue*, const pcap_pkthdr*, const u_char*);
typedef void (WINAPI* f_pcap_sendqueue_destroy)(pcap_send_queue*);
typedef u_int (WINAPI* f_pcap_sendqueue_transmit)(pcap_t*, pcap_send_queue*, int);
typedef char* (WINAPI* f_pcap_geterr)(pcap_t*);
typedef void (WINAPI* f_pcap_close)(pcap_t*);

HMODULE PacketMod;
HMODULE pthreadVCMod;
HMODULE WanPacketMod;
HMODULE wpcapMod;

f_pcap_createsrcstr _pcap_createsrcstr;
f_pcap_open _pcap_open;
f_pcap_datalink _pcap_datalink;
f_pcap_sendqueue_alloc _pcap_sendqueue_alloc;
f_pcap_next_ex _pcap_next_ex;
f_pcap_sendqueue_queue _pcap_sendqueue_queue;
f_pcap_sendqueue_destroy _pcap_sendqueue_destroy;
f_pcap_sendqueue_transmit _pcap_sendqueue_transmit;
f_pcap_geterr _pcap_geterr;
f_pcap_close _pcap_close;

#endif
