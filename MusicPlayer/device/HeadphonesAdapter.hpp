#include<iostream>
#include "./IAudioOutputDevice.hpp"
#include "../models/Song.hpp"
#include "../external/HeadPhonesAPI.hpp"

using namespace std;

class HeadphonesAdapter:public IAudioOutputDevice{
    private:
    HeadPhonesAPI* headPhonesapi;
    public:
    HeadphonesAdapter(HeadPhonesAPI* api){
        headPhonesapi=api;
    }
    void playAudio(Song* song) override{
        headPhonesapi->playSoundviaHeadPhones(song->getTitle());
    }
};