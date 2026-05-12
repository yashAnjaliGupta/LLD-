#include <iostream>
#include "../device/IAudioOutputDevice.hpp"
#include "../enums/DeviceType.hpp"
#include "../factories/DeviceFactory.hpp"
using namespace std;

class DeviceManager{
private:
    static DeviceManager* instance;
    IAudioOutputDevice * currentOutputDevice;
    DeviceManager(){
        currentOutputDevice=NULL;
    }
public:
    static DeviceManager *getInstance(){
        if(instance==NULL){
            instance=new DeviceManager();
        }
        return instance;
    }
    void connect(DeviceType deviceType){
        if(currentOutputDevice){
            delete currentOutputDevice;
        }
        currentOutputDevice = DeviceFactory::createDevice(deviceType);

        switch(deviceType){
            case DeviceType::BLUETOOTH:
                cout<<"Bluetooth device connected"<<endl;
            break;
            case DeviceType::HEADPHONES:
                cout<<"Headphones device connected"<<endl;
            break;
            case DeviceType::WIRED:
                cout<<"Wired device connected"<<endl;
        }
    }
    IAudioOutputDevice* getOutputDevice(){
        if(!currentOutputDevice){
            throw runtime_error("No output device is connected.");
        }
        return currentOutputDevice;
    }
    bool hasOutputDevice(){
        return currentOutputDevice!=NULL;
    }
};

DeviceManager* DeviceManager::instance=NULL;