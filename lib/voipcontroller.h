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

#ifndef PHPLIBTGVOIP_H
#define PHPLIBTGVOIP_H
#ifndef TGVOIP_USE_CALLBACK_AUDIO_IO
#define TGVOIP_USE_CALLBACK_AUDIO_IO
#endif

#include <common.h>
#include "libtgvoip/VoIPController.h"
#include <stdio.h>
#include "libtgvoip/threading.h"
#include <queue>

using namespace tgvoip;
using namespace tgvoip::audio;

namespace tgvoip
{
namespace audio
{
class AudioInputModule;
class AudioOutputModule;
}
}
class VoIP
{
public:
    VoIP(bool creator, int32_t other_ID, int32_t call_id, CALL_STATE callState, CallProtocols call_protocols);
    void initVoIPController();
    void deinitVoIPController();
    void __wakeup();
    void recvAudioFrame(int16_t* data, size_t size);
    void sendAudioFrame(int16_t* data, size_t size);
    int unsetOutputFile();

    int state = STATE_CREATED;
    bool playing = false;
    bool destroyed = false;
    int32_t other_ID;
    int32_t call_id;
    bool creator;
    CALL_STATE callState = CALL_STATE_NONE;
    CallProtocols call_protocols;
    VoIPController *inst;
    int64_t created;
    Mutex input_mutex;
    FILE *output_file;
    Mutex output_mutex;
    std::queue<FILE *> input_files;
    std::queue<FILE *> hold_files;

private:
    size_t read_input;
    size_t read_output;
};

#endif
