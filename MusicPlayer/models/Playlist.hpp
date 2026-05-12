#pragma once
#include<iostream>
#include<vector>
#include "./Song.hpp"
using namespace std;

class Playlist{
    private:
    string playlistName;
    vector<Song*> songs;
    public:
    Playlist(string n){
        playlistName=n;
    }
    string getPlaylistName(){
        return playlistName;
    }
    const vector<Song*> getSongs(){
        return songs;
    }
    int getSize(){
        return songs.size();
    }
    void addSongToPlaylist(Song *s){
        if(s==NULL){
            throw runtime_error("Cannot add null song to playlist");
        }else{
            songs.push_back(s);
        }
        
    }

};
