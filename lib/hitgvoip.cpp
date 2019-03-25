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

#include <hitgvoip.h>
#include <voipcontroller.h>
#include <libtgvoip/VoIPServerConfig.h>
#include <string>

VoIP* get_client(client_t client) {
    return reinterpret_cast<VoIP*>(client.client_id);
}

extern "C" {
client_t create_client(call_params_t call_params) {
    auto client_id = reinterpret_cast<uintptr_t>(new VoIP(call_params.creator,
                                                               call_params.other_ID,
                                                               call_params.call_id,
                                                               call_params.call_state,
                                                               call_params.call_protocols));
    return {client_id, call_params.discard_callback, call_params.accept_callback};
}

void destroy_client(client_t client) {
    delete get_client(client);
}

int discard_call(client_t client, call_discard_reason_t call_discard_reason, int32_t reason) {
    if (get_client(client)->callState == CALL_STATE_ENDED) {
        return false;
    }

    client.discard_callback(get_client(client)->call_id, call_discard_reason, reason);
    get_client(client)->deinitVoIPController();
    return true;
}

int accept_call(client_t client) {
    if (get_client(client)->callState != CALL_STATE_INCOMING) {
        return false;
    }

    get_client(client)->callState = CALL_STATE_ACCEPTED;

    if(!client.accept_callback(get_client(client)->call_id)) {
        client.discard_callback(get_client(client)->call_id, CallDiscardReasonDisconnected, 5);
        get_client(client)->deinitVoIPController();
        return false;
    }

    return true;
}

void close_call(client_t client) {
    get_client(client)->deinitVoIPController();
}

void __wakeup(client_t client) {
    get_client(client)->callState = CALL_STATE_ENDED;
}

int start(client_t client) {
    if (get_client(client)->state == STATE_WAIT_INIT_ACK) {
        client.discard_callback(get_client(client)->call_id, CallDiscardReasonDisconnected, 5);
        get_client(client)->deinitVoIPController();
        return false;
    }

    get_client(client)->inst->Start();
    get_client(client)->inst->Connect();
    get_client(client)->created = reinterpret_cast<int64_t>(time(nullptr));
    get_client(client)->callState = CALL_STATE_READY;
    return true;
}

int64_t when_created(client_t client) {
    return get_client(client)->created;
}

int is_creator(client_t client) {
    return get_client(client)->creator;
}

long get_other_ID(client_t client) {
    return get_client(client)->other_ID;
}

CallProtocols get_protocol(client_t client) {
    return get_client(client)->call_protocols;
}

long get_call_id(client_t client) {
    return get_client(client)->call_id;
}

CALL_STATE get_call_state(client_t client) {
    return get_client(client)->callState;
}

int set_output_file(client_t client, char* output_file_path) {
    MutexGuard m(get_client(client)->output_mutex);

    if (get_client(client)->output_file != nullptr) {
        fclose(get_client(client)->output_file);
        get_client(client)->output_file = nullptr;
    }

    get_client(client)->output_file = fopen(output_file_path, "wb");

    return get_client(client)->output_file != nullptr;

}

int play(client_t client, char* input_file_path) {
    FILE *tmp = fopen(input_file_path, "rb");

    if (tmp == nullptr) {
        return false;
    }

    MutexGuard m(get_client(client)->input_mutex);
    get_client(client)->input_files.push(tmp);
    return true;
}

int play_on_hold(client_t client, char** input_files_path, size_t length) {
    FILE *tmp = nullptr;

    MutexGuard m(get_client(client)->input_mutex);
    while (!get_client(client)->hold_files.empty()) {
        fclose(get_client(client)->hold_files.front());
        get_client(client)->hold_files.pop();
    }

    for (size_t i = 0; i < length; i++) {
        tmp = fopen(input_files_path[i], "rb");
        if (tmp == nullptr) {
            return false;
        }
        get_client(client)->hold_files.push(tmp);
    }
    return true;
}

void set_mic_mute(client_t client, int mute) {
    get_client(client)->inst->SetMicMute(static_cast<bool>(mute));
}

void debug_ctl(client_t client, int request, int param) {
    get_client(client)->inst->DebugCtl(request, param);
}

void set_bitrate(client_t client, int bitrate) {
    debug_ctl(client, 1, bitrate);
}

size_t get_debug_log(client_t client, char* debug_data, size_t size) {
    string encoded = get_client(client)->inst->GetDebugLog();

    if (encoded.empty()) {
        return 0;
    }

    strncpy(debug_data, encoded.c_str(), size);
    return encoded.size();
}

const char* get_version() {
    return VoIPController::GetVersion();
}

int get_signal_bars_count(client_t client) {
    return get_client(client)->inst->GetSignalBarsCount();
}

int64_t get_preferred_relay_ID(client_t client) {
    return get_client(client)->inst->GetPreferredRelayID();
}

int get_last_error(client_t client) {
    return get_client(client)->inst->GetLastError();
}

size_t get_debug_string(client_t client, char* debug_string, size_t size) {
    string data = get_client(client)->inst->GetDebugString();

    if (data.empty()) {
        return 0;
    }

    strncpy(debug_string, data.c_str(), size);
    return data.size();
}

call_stats_t get_stats(client_t client) {
    VoIPController::TrafficStats _stats {};
    get_client(client)->inst->GetStats(&_stats);
    return {_stats.bytesSentWifi, _stats.bytesSentMobile, _stats.bytesRecvdWifi, _stats.bytesRecvdMobile};
}

uint32_t get_peer_capabilities(client_t client) {
    return get_client(client)->inst->GetPeerCapabilities();
}

void request_call_upgrade(client_t client) {
    get_client(client)->inst->RequestCallUpgrade();
}

void send_group_call_key(client_t client, unsigned char* key) {
    auto *key_data = new unsigned char[256];
    memcpy(key_data, key, 256);
    get_client(client)->inst->SendGroupCallKey(key);
    delete[] key_data;
}

int get_state(client_t client) {
    return get_client(client)->state;
}

int is_playing(client_t client) {
    return get_client(client)->playing;
}

int is_destroyed(client_t client) {
    return get_client(client)->destroyed;
}

void set_server_config(const char **values, int count) {
    ServerConfig::GetSharedInstance()->Update(values, count);
}

void set_config(client_t client, config_t config) {
    VoIPController::Config config_obj;
    config_obj.recvTimeout = config.recv_timeout;
    config_obj.initTimeout = config.init_timeout;
    config_obj.dataSaving = config.data_saving;
    config_obj.enableAEC = config.enableAEC;
    config_obj.enableNS = config.enableNS;
    config_obj.enableAGC = config.enableAGC;
    config_obj.enableCallUpgrade = config.enableCallUpgrade;

    if(config.log_file_path != nullptr) {
        config_obj.logFilePath = config.log_file_path;
    }

    if(config.status_dump_path != nullptr) {
        config_obj.statsDumpFilePath = config.status_dump_path;
    }

    get_client(client)->inst->SetConfig(config_obj);
}

void set_encryption_key(client_t client, char* key) {
    auto *key_data = new char[256];
    memcpy(key_data, key, 256);
    get_client(client)->inst->SetEncryptionKey(key, get_client(client)->creator);
    delete[] key_data;
}

void set_endpoints(client_t client, endpoints_t endpoints) {
    vector<Endpoint> endpoints_obj;
    IPv4Address ipv4(std::string(endpoints.ipv4));
    IPv6Address ipv6("::0");
    auto *peer_tag = new unsigned char[16];

    if(endpoints.ipv6[0] != '\0') {
        ipv6 = IPv6Address(std::string(endpoints.ipv6));
    }

    if(endpoints.peer_tag[0] != '\0') {
        memcpy(peer_tag, endpoints.peer_tag, 16);
    }

    endpoints_obj.emplace_back(endpoints.id, endpoints.port, ipv4, ipv6, Endpoint::TYPE_UDP_RELAY, peer_tag);
    endpoints_obj.emplace_back(endpoints.id, endpoints.port, ipv4, ipv6, Endpoint::TYPE_TCP_RELAY, peer_tag);
    delete[] peer_tag;
    get_client(client)->inst->SetRemoteEndpoints(endpoints_obj, get_protocol(client).udp_p2p_, get_protocol(client).max_layer_);
}

void set_network_type(client_t client, NET_TYPE type) {
    get_client(client)->inst->SetNetworkType(type);
}

void set_proxy(client_t client, proxy_t proxy) {
    get_client(client)->inst->SetProxy(proxy.protocol, std::string(proxy.address), proxy.port, std::string(proxy.username), std::string(proxy.password));
}

}
