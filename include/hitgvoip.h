/*
Copyright 2016-2017 Daniil Gentili
(https://daniil.it)
Copyright 2019 Ernesto Castellotti <erny.castell@gmail.com>
This file is part of HiTGVoip, which is a fork of php-libtgvoip (more information on https://github.com/danog/libtgvoip).
HiTGVoip is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
HiTGVoip is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU Affero General Public License for more details.
You should have received a copy of the GNU General Public License along with php-libtgvoip.
If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef LIBTGVOIP_H
#define LIBTGVOIP_H
#include <common.h>

#ifdef __cplusplus
#include <voipcontroller.h>
VoIP* get_client(client_t client); //Exports the function that exposes the wrapped object only for CPP
extern "C" {
#endif
client_t create_client(call_params_t call_params);
void destroy_client(client_t client);
int discard_call(client_t client, call_discard_reason_t call_discard_reason, int32_t reason);
int accept_call(client_t client);
void close_call(client_t client);
void __wakeup(client_t client);
int startTheMagic(client_t client);
int64_t when_created(client_t client);
int is_creator(client_t client);
long get_other_ID(client_t client);
CallProtocols get_protocol(client_t client);
long get_call_id(client_t client);
CALL_STATE get_call_state(client_t client);
int set_output_file(client_t client, char* output_file_path);
int play(client_t client, char* input_file_path);
int play_on_hold(client_t client, char** input_files_path, size_t length);
void set_mic_mute(client_t client, int mute);
void debug_ctl(client_t client, int request, int param);
void set_bitrate(client_t client, int bitrate);
size_t get_debug_log(client_t client, char* debug_data, size_t size);
const char* get_version();
int get_signal_bars_count(client_t client);
int64_t get_preferred_relay_ID(client_t client);
int get_last_error(client_t client);
size_t get_debug_string(client_t client, char* debug_string, size_t size);
call_stats_t get_stats(client_t client);
uint32_t get_peer_capabilities(client_t client);
void request_call_upgrade(client_t client);
void send_group_call_key(client_t client, unsigned char* key);
int get_state(client_t client);
int is_playing(client_t client);
int is_destroyed(client_t client);
#ifdef __cplusplus
}
#endif
#endif // LIBTGVOIP_H
