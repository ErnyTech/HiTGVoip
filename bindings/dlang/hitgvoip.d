module hitgvoip;

import c.hitgvoip;
import std.file;
import std.outbuffer;
import std.string;
import core.stdc.string;
import std.json;
import std.conv;
import std.exception;
public import c.hitgvoip: CallProtocols;
public import c.hitgvoip: AUDIO_STATE;
public import c.hitgvoip: CALL_STATE;
public import c.hitgvoip: PROXY_PROTOCOL;
public import c.hitgvoip: NET_TYPE;
public import c.hitgvoip: DATA_SAVING;
public import c.hitgvoip: CallDiscardReason = call_discard_reason_t;
public import c.hitgvoip: CallStats = call_stats_t;
public import c.hitgvoip: VoipConfig = config_t;
public import c.hitgvoip: VoipEndpoint = endpoints_t;
public import c.hitgvoip: VoipProxy = proxy_t;

class HiTGVoip {
	private bool isCreator;
	private long otherID;
	private long callID;
	private CALL_STATE callState;
	private CallProtocols callProtocols;
	private DiscardCallback discardCallback;
	private AcceptCallback acceptCallback;
	private client_t client;
	
	
	this() {
	}
	
	this(bool isCreator, long otherID, long callID, CALL_STATE callState, CallProtocols callProtocols) {
		this.isCreator = isCreator;
		this.otherID = otherID;
		this.callID = callID;
		this.callState = callState;
		this.callProtocols = callProtocols;
	}
	
	this(bool isCreator, long otherID, long callID, CALL_STATE callState, CallProtocols callProtocols, DiscardCallback discardCallback, AcceptCallback acceptCallback) {
		this.isCreator = isCreator;
		this.otherID = otherID;
		this.callID = callID;
		this.callState = callState;
		this.callProtocols = callProtocols;
		this.discardCallback = discardCallback;
		this.acceptCallback = acceptCallback;
	}
	
	~this() {
		destroy_client(this.client);
	}

	HiTGVoip setIsCreator(bool isCreator) {
		this.isCreator = isCreator;
		return this;
	}

	HiTGVoip setOtherID(long otherID) {
		this.otherID = otherID;
		return this;
	}

	HiTGVoip setCallID(long callID) {
		this.callID = callID;
		return this;
	}

	HiTGVoip setCallState(CALL_STATE callState) {
		this.callState = callState;
		return this;
	}

	HiTGVoip setCallProtocols(CallProtocols callProtocols) {
		this.callProtocols = callProtocols;
		return this;
	}

	HiTGVoip setDiscardCallback(DiscardCallback discardCallback) {
		this.discardCallback = discardCallback;
		return this;
	}

	HiTGVoip setAcceptCallback(AcceptCallback acceptCallback) {
		this.acceptCallback = acceptCallback;
		return this;
	}
	
	void init() {
		call_params_t callParams = {this.isCreator, 
			this.otherID, 
			this.callID, 
			this.callState, 
			this.callProtocols, 
			this.discardCallback, 
			this.acceptCallback};
		this.client = create_client(callParams);
	}
	
	void discard(CallDiscardReason discardReason, int rating) {
		discard_call(this.client, discardReason, rating);
	}
	
	void accept() {
		accept_call(this.client);
	}
	
	void close() {
		destroy_client(this.client);
	}
	
	void start() {
		c.hitgvoip.start(this.client);
	}
	
	long whenCreated() {
		return when_created(this.client);
	}
	
	bool isCallCreator() {
		return to!bool(is_creator(this.client));
	}
	
	long getOtherID() {
		return get_other_ID(this.client);
	}
	
	CallProtocols getCallProtocol() {
		return get_protocol(this.client);
	}
	
	long getCallID() {
		return get_call_id(this.client);
	}
	
	HiTGVoip setOutputFile(string outputPath) {
		immutable(int) result = set_output_file(this.client, stringToCStr(outputPath));
		
		if(!result) {
			throw new FileException(outputPath.readText); 
		}

		return this;
	}
	
	HiTGVoip play(string inputPath) {
		immutable(int) result = c.hitgvoip.play(this.client, stringToCStr(inputPath));
		
		if(!result) {
			throw new FileException(inputPath.readText); 
		}

		return this;
	}
	
	HiTGVoip playOnHold(string[] inputPaths) {
		immutable(int) result = play_on_hold(this.client, stringsToArrStr(inputPaths), inputPaths.length);
		
		if(!result) {
			throw new FileException(join(inputPaths, ", ").readText); 
		}

		return this;
	}
	
	HiTGVoip muteMic() {
		set_mic_mute(this.client, 1);
		return this;
	}
	
	HiTGVoip unmuteMic() {
		set_mic_mute(this.client, 0);
		return this;
	}
	
	HiTGVoip debugCtl(int request, int param) {
		debug_ctl(this.client, request, param);
		return this;
	}
	
	HiTGVoip setBitrate(int bitrate) {
		set_bitrate(this.client, bitrate);
		return this;
	}
	
	string getDebugLog() {
		auto buf = new OutBuffer();
		char[8192] buffer;
		
		while((get_debug_log(this.client, buffer.ptr, 8192)) != 0) {
			buf.write(buffer);
			buffer[] = 0;
		}
		
		return buf.toString();
	}
	
	string getVersion() {
		auto cversion = get_version();
		return cversion[0..strlen(cversion)].idup;
	}
	
	int getSignalBarsCount() {
		return get_signal_bars_count(this.client);
	}
	
	long getPreferredRelayID() {
		return get_preferred_relay_ID(this.client);
	}
	
	int getLastError() {
		return get_last_error(this.client);
	}
	
	string getDebugString() {
		auto buf = new OutBuffer();
		char[8192] buffer;
		
		while((get_debug_string(this.client, buffer.ptr, 8192)) != 0) {
			buf.write(buffer);
			buffer[] = 0;
		}
		
		return buf.toString();
	}
	
	CallStats getCallStats() {
		return get_stats(this.client);
	} 
	
	uint getPeerCapabilities() {
		return get_peer_capabilities(this.client);
	}
	
	HiTGVoip requestCallUpgrade() {
		request_call_upgrade(this.client);
		return this;
	}
	
	HiTGVoip sendGroupCallKey(string key) {
		send_group_call_key(this.client, stringToCStr(key));
		return this;
	}
	
	int getState() {
		return get_state(this.client);
	}
	
	bool isPlaying() {
		return to!bool(is_playing(this.client));
	}
	
	HiTGVoip setServerConfig(string jsonServerConfig) {
		auto json = parseJSON(jsonServerConfig);
		auto length = json.object.values.length + json.object.keys.length;
		string[] config = new string[length];
		int index = 0;		
		
		foreach(elem; json.object.byKeyValue()) {
			string key = elem.key;
			string value;
			
			if(json[key].type == JSONType.string) {
				value = json[key].str;
			}
			
			if(json[key].type == JSONType.integer) {
				value = to!string(json[key].integer);
			}
			
			if(json[key].type == JSONType.float_) {
				value = to!string(json[key].floating);
			}
			
			if(json[key].type == JSONType.uinteger) {
				value = to!string(json[key].uinteger);
			}
			
			if(json[key].type == JSONType.true_ || json[key].type == JSONType.false_) {
				value = to!string(json[key].boolean);
			}
			
			config[index] = key;
			config[++index] = value; 
			index++;
		}
		
		set_server_config(stringsToArrStr(config), to!int(length));
		return this;
	}
	
	HiTGVoip setConfig(config_t config) {
		set_config(this.client, config);
		return this;
	}
	
	HiTGVoip setEncryptionKey(string key) {
		set_encryption_key(this.client, stringToCStr(key));
		return this;
	}
	
	HiTGVoip setNetworkType(NET_TYPE type) {
		set_network_type(this.client, type);
		return this;
	}
	
	HiTGVoip setProxy(proxy_t proxy) {
		set_proxy(this.client, proxy);
		return this;
	}

	private char* stringToCStr(string str) {
		auto cstr = str.toStringz;
		return cstr[0 .. str.length + 1].dup.ptr;
	}

	private char[]* stringsToArrStr(string[] strings) {
		char[][] carr = new char[][strings.length];

		foreach(i, elem; strings) {
			carr[i] = (toStringz(elem)[0..elem.length + 1]).dup;
		}

		return carr.ptr;
	}
}