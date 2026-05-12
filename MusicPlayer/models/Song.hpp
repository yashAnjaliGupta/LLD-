#pragma once
#include<iostream>

using namespace std;

class Song{
    private:
    string title;
    string artist;
    string filepath;
    public:
    Song(string t,string a,string f){
        title=t;
        artist=a;
        filepath=f;
    }
    string getTitle(){
        return title;
    }
    string getArtist(){
        return artist;
    }
    string getFilePath(){
        return filepath;
    }
};