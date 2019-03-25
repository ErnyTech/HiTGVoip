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

#include <voipcontroller.h>
#include <libtgvoip/VoIPServerConfig.h>
#include <libtgvoip/threading.h>
#include <libtgvoip/logging.h>

using namespace tgvoip;
using namespace tgvoip::audio;
using namespace std;

VoIP::VoIP(bool creator, long other_ID, long call_id, CALL_STATE callState, CallProtocols call_protocols) {
    this->creator = creator;
    this->other_ID = other_ID;
    this->call_id = call_id;
    this->callState = callState;
    this->call_protocols = call_protocols;
    this->state = STATE_CREATED;
    initVoIPController();
}

void VoIP::initVoIPController() {
    inst = new VoIPController();

    this->output_file = nullptr;

    inst->implData = static_cast<void*>(this);
    VoIPController::Callbacks callbacks {};
    callbacks.connectionStateChanged = [](VoIPController *controller, int state) {
        static_cast<VoIP*>(controller->implData)->state = state;

        if (state == STATE_FAILED) {
            (static_cast<VoIP*>(controller->implData))->deinitVoIPController();
        }
    };

    callbacks.signalBarCountChanged = nullptr;
    callbacks.groupCallKeySent = nullptr;
    callbacks.groupCallKeyReceived = nullptr;
    callbacks.upgradeToGroupCallRequested = nullptr;
    inst->SetCallbacks(callbacks);
    inst->SetAudioDataCallbacks([this](int16_t *buffer, size_t size) {
        this->sendAudioFrame(buffer, size);
        }, [this](int16_t *buffer, size_t size) {
        this->recvAudioFrame(buffer, size);
    });

}

void VoIP::deinitVoIPController() {
    if (this->callState != CALL_STATE_ENDED) {
        this->callState = CALL_STATE_ENDED;

        if (inst) {
            inst->Stop();
            delete inst;
            inst=nullptr;
        }

        while (!this->hold_files.empty()) {
            fclose(this->hold_files.front());
            this->hold_files.pop();
        }

        while (!this->input_files.empty()) {
            fclose(this->input_files.front());
            this->input_files.pop();
        }

        unsetOutputFile();
        this->destroyed = true;
    }
}

void VoIP::recvAudioFrame(int16_t *data, size_t size) {
    MutexGuard m(this->output_mutex);

    if (this->output_file != nullptr) {
        if (fwrite(data, sizeof(int16_t), size, this->output_file) != size) {
            LOGE("COULD NOT WRITE DATA TO FILE");
        }
    }
}
void VoIP::sendAudioFrame(int16_t *data, size_t size) {
    MutexGuard m(this->input_mutex);

    if (!this->input_files.empty()) {
        if ((this->read_input = fread(data, sizeof(int16_t), size, this->input_files.front())) != size) {
            fclose(this->input_files.front());
            this->input_files.pop();
            memset(data + (this->read_input % size), 0, size - (this->read_input % size));
        }
        this->playing = true;
    }
    else {
        this->playing = false;
        if (!this->hold_files.empty()) {
            if ((this->read_input = fread(data, sizeof(int16_t), size, this->hold_files.front())) != size) {
                fseek(this->hold_files.front(), 0, SEEK_SET);
                this->hold_files.push(this->hold_files.front());
                this->hold_files.pop();
                memset(data + (this->read_input % size), 0, size - (this->read_input % size));
            }
        }
    }
}

int VoIP::unsetOutputFile() {
    if (this->output_file == nullptr) {
        return false;
    }

    MutexGuard m(this->output_mutex);
    fflush(this->output_file);
    fclose(this->output_file);
    this->output_file = nullptr;
    return true;
}
