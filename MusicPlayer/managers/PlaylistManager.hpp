#pragma once
#include<iostream>
#include<map>
#include "../models/Playlist.hpp"
using namespace std;

class PlaylistManager{
private:
    static PlaylistManager* instance;
    map<string,Playlist*> playlists;
    PlaylistManager(){}
public:
    static PlaylistManager* getInstance(){
        if(!instance){
            instance=new PlaylistManager();
        }
        return instance;
    }
    Playlist* getPlaylist(const string &name){
        if(!playlists.count(name)){
            throw runtime_error("Playlist \""+ name+"\" not found.");
        }
        return playlists[name];
    }
    void createPlaylist(const string &name){
        if(playlists.count(name)){
            throw runtime_error("playlist \""+name+"\" already exists.");
        }
        playlists[name]=new Playlist(name);
    }
    void addSongToPlaylist(const string &name,Song* song){
        if(!playlists.count(name)){
            throw runtime_error("Playlist \""+ name+"\" not found.");
        }
        playlists[name]->addSongToPlaylist(song);
    }
};
PlaylistManager *PlaylistManager::instance=NULL;