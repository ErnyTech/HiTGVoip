module c.hitgvoip;
import std.stdint;

extern (C) {
	alias DiscardCallback = int function(long, call_discard_reason_t call, int32_t);
	alias AcceptCallback = void function(long);
	
	struct CallProtocols {
		bool udp_p2p_;
		bool udp_reflector_;
		int32_t min_layer_;
		int32_t max_layer_;
	};
	
	enum AUDIO_STATE {
		AUDIO_STATE_NONE = -1,
		AUDIO_STATE_CREATED = 0,
		AUDIO_STATE_CONFIGURED = 1,
		AUDIO_STATE_RUNNING = 2
	};
	
	enum CALL_STATE {
		CALL_STATE_NONE = -1,
		CALL_STATE_REQUESTED = 0,
		CALL_STATE_INCOMING = 1,
		CALL_STATE_ACCEPTED = 2,
		CALL_STATE_CONFIRMED = 3,
		CALL_STATE_READY = 4,
		CALL_STATE_ENDED = 5
	};
	
	enum PROXY_PROTOCOL {
		PROXY_NONE = 0,
		PROXY_SOCKS5 = 1
	};
	
	enum NET_TYPE {
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
	};
	
	enum DATA_SAVING {
		DATA_SAVING_NEVER = 0,
		DATA_SAVING_MOBILE,
		DATA_SAVING_ALWAYS
	};
	
	enum call_discard_reason_t {
		CallDiscardReasonDeclined,
		CallDiscardReasonDisconnected,
		CallDiscardReasonEmpty,
		CallDiscardReasonHungUp,
		CallDiscardReasonMissed
	};
	
	struct call_stats_t {
		uint64_t bytes_sent_wifi;
		uint64_t bytes_sent_mobile;
		uint64_t bytes_recvd_wifi;
		uint64_t bytes_recvd_mobile;
	};
	
	struct config_t {
		double recv_timeout;
		double init_timeout;
		DATA_SAVING data_saving;
		bool enableAEC;
		bool enableNS;
		bool enableAGC;
		bool enableCallUpgrade;
		const char *log_file_path;
		const char *status_dump_path;
	}
	
	struct  endpoints_t {
		int64_t id;
		char* ipv4;
		char* ipv6;
		int32_t port;
		char* peer_tag;
	};
	
	struct proxy_t {
		PROXY_PROTOCOL protocol;
		char* address;
		uint16_t port;
		char* username;
		char* password;
	};
	
	struct client_t {
		uintptr_t client_id;
		DiscardCallback discard_callback;
		AcceptCallback accept_callback;
    };
	
	struct call_params_t {
		bool creator;
		long other_ID;
		long call_id;
		CALL_STATE call_state;
		CallProtocols call_protocols;
		DiscardCallback discard_callback;
		AcceptCallback accept_callback;
	};
	
	client_t create_client(call_params_t call_params);
	void destroy_client(client_t client);
	int discard_call(client_t client, call_discard_reason_t call_discard_reason, int32_t reason);
	int accept_call(client_t client);
	void close_call(client_t client);
	int start(client_t client);
	int64_t when_created(client_t client);
	int is_creator(client_t client);
	long get_other_ID(client_t client);
	CallProtocols get_protocol(client_t client);
	long get_call_id(client_t client);
	CALL_STATE get_call_state(client_t client);
	int set_output_file(client_t client, char* output_file_path);
	int play(client_t client, char* input_file_path);
	int play_on_hold(client_t client, char[]* input_files_path, size_t length);
	void set_mic_mute(client_t client, int mute);
	void debug_ctl(client_t client, int request, int param);
	void set_bitrate(client_t client, int bitrate);
	size_t get_debug_log(client_t client, char* debug_data, size_t size);
	const(char)* get_version();
	int get_signal_bars_count(client_t client);
	int64_t get_preferred_relay_ID(client_t client);
	int get_last_error(client_t client);
	size_t get_debug_string(client_t client, char* debug_string, size_t size);
	call_stats_t get_stats(client_t client);
	uint32_t get_peer_capabilities(client_t client);
	void request_call_upgrade(client_t client);
	void send_group_call_key(client_t client, char* key);
	int get_state(client_t client);
	int is_playing(client_t client);
	int is_destroyed(client_t client);
	void set_server_config(char[]* values, int count);
	void set_config(client_t client, config_t config);
	void set_encryption_key(client_t client, char* key);
	void set_endpoints(client_t client, endpoints_t endpoints);
	void set_network_type(client_t client, NET_TYPE type);
	void set_proxy(client_t client, proxy_t proxy);
}