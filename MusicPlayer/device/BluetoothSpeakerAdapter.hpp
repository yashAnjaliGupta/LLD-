#include<iostream>
#include "./IAudioOutputDevice.hpp"
#include "../models/Song.hpp"
#include "../external/BluetoothSpeakerAPI.hpp"

using namespace std;

class BluetoothSpeakerAdapter:public IAudioOutputDevice{
    private:
    BluetoothSpeakerAPI* bluetoothapi;
    public:
    BluetoothSpeakerAdapter(BluetoothSpeakerAPI* api){
        bluetoothapi=api;
    }
    void playAudio(Song* song) override{
        bluetoothapi->playSoundviaBluetooth(song->getTitle());
    }
};