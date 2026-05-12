#include<iostream>
#include "./IAudioOutputDevice.hpp"
#include "../models/Song.hpp"
#include "../external/WiredSpeakerAPI.hpp"

using namespace std;

class WiredSpeakerAdapter:public IAudioOutputDevice{
    private:
    WiredSpeakerAPI* wiredSpeakerapi;
    public:
    WiredSpeakerAdapter(WiredSpeakerAPI* api){
        wiredSpeakerapi=api;
    }
    void playAudio(Song* song) override{
        wiredSpeakerapi->playSoundviaWiredSpeaker(song->getTitle());
    }
};