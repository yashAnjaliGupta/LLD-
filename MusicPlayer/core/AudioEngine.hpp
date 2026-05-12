
#include<iostream>
#include "../models/Song.hpp"
#include "../device/IAudioOutputDevice.hpp"
using namespace std;

class AudioEngine{
 private:
 Song * currSong;
 bool songIsPaused;
 public:
    AudioEngine(){
        currSong=NULL;
        songIsPaused=false;
    }
    string getCurrentSongTitle() const{
        if(currSong){
            return currSong->getTitle();
        }
        return "";
    }
    bool isPaused() const{
        return songIsPaused;
    }
    void play(IAudioOutputDevice* aod, Song* song){
        if(song==NULL){
            throw runtime_error("Cannot play a NULL song");
        }
        if(songIsPaused && song==currSong){
            songIsPaused=false;
            cout<<"Resuming song: "<<song->getTitle()<<endl;
            aod->playAudio(song);
            return;
        }
        currSong = song;
        songIsPaused = false;
        cout << "Playing song: " << song->getTitle() << "\n";
        aod->playAudio(song);
    }
    void pause(){
        if(currSong==NULL){
            throw runtime_error("No song is currently playing to pause");
        }
        if(songIsPaused){
            throw runtime_error("Song is already paused");
        }
        songIsPaused=true;
        cout<<"Pausing Song: "<<currSong->getTitle()<<endl;
    }
};