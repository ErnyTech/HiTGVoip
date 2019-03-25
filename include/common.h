#ifndef COMMON_H
/*
Copyright 2019 Ernesto Castellotti <erny.castell@gmail.com>
This file is part of HiTGVoip, which is a fork of php-libtgvoip (more information on https://github.com/danog/libtgvoip).
HiTGVoip is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
HiTGVoip is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU Affero General Public License for more details.
You should have received a copy of the GNU General Public License along with php-libtgvoip.
If not, see <http://www.gnu.org/licenses/>.
*/

#define COMMON_H
#ifdef __cplusplus
#include <cstdint>
#include <cstdlib>
#include <cstdbool>
#else
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif
typedef struct {
    bool udp_p2p_;
    bool udp_reflector_;
    int32_t min_layer_;
    int32_t max_layer_;
} CallProtocols;

typedef enum {
    AUDIO_STATE_NONE = -1,
    AUDIO_STATE_CREATED = 0,
    AUDIO_STATE_CONFIGURED = 1,
    AUDIO_STATE_RUNNING = 2
} AUDIO_STATE;

typedef enum {
    CALL_STATE_NONE = -1,
    CALL_STATE_REQUESTED = 0,
    CALL_STATE_INCOMING = 1,
    CALL_STATE_ACCEPTED = 2,
    CALL_STATE_CONFIRMED = 3,
    CALL_STATE_READY = 4,
    CALL_STATE_ENDED = 5
} CALL_STATE;

typedef enum {
    PROXY_NONE = 0,
    PROXY_SOCKS5,
} PROXY_PROTOCOL;

typedef enum {
    NET_TYPE_UNKNOWN=0,
    NET_TYPE_GPRS,
    NET_TYPE_EDGE,
    NET_TYPE_3G,
    NET_TYPE_HSPA,
    NET_TYPE_LTE,
    NET_TYPE_WIFI,
    NET_TYPE_ETHERNET,
    NET_TYPE_OTHER_HIGH_SPEED,
    NET_TYPE_OTHER_LOW_SPEED,
    NET_TYPE_DIALUP,
    NET_TYPE_OTHER_MOBILE
} NET_TYPE;

typedef enum {
    DATA_SAVING_NEVER = 0,
    DATA_SAVING_MOBILE,
    DATA_SAVING_ALWAYS
} DATA_SAVING;

typedef enum {
    CallDiscardReasonDeclined,
    CallDiscardReasonDisconnected,
    CallDiscardReasonEmpty,
    CallDiscardReasonHungUp,
    CallDiscardReasonMissed
} call_discard_reason_t;

typedef struct {
    uint64_t bytes_sent_wifi;
    uint64_t bytes_sent_mobile;
    uint64_t bytes_recvd_wifi;
    uint64_t bytes_recvd_mobile;
} call_stats_t;

typedef struct {
    double recv_timeout;
    double init_timeout;
    DATA_SAVING data_saving;
    bool enableAEC;
    bool enableNS;
    bool enableAGC;
    bool enableCallUpgrade;
    const char *log_file_path;
    const char *status_dump_path;
} config_t;

typedef struct {
    int64_t id;
    char* ipv4;
    char* ipv6;
    int32_t port;
    char* peer_tag;
} endpoints_t;

typedef struct {
    PROXY_PROTOCOL protocol;
    char* address;
    uint16_t port;
    char* username;
    char* password;
} proxy_t;

typedef struct {
    uintptr_t client_id;
    void (*discard_callback)(long call_id, call_discard_reason_t call_discard_reason, int32_t reason);
    int (*accept_callback)(long call_id);
} client_t;

typedef struct {
    bool creator;
    long other_ID;
    long call_id;
    CALL_STATE call_state;
    CallProtocols call_protocols;
    void (*discard_callback)(long call_id, call_discard_reason_t call_discard_reason, int32_t reason);
    int (*accept_callback)(long call_id);
} call_params_t;
#ifdef __cplusplus
}
#endif
#endif // COMMON_H
