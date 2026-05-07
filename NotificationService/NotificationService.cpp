#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
// Decorator pattern in notification
class INotification{
    public:
    virtual string getContent()=0;  
    virtual ~ INotification(){}
};

class SimpleNotification:public INotification{
    string text;
    public:
    SimpleNotification(string text){
        this->text=text;
    }
    string getContent() override{
        return text;
    }
};
class INotificationDecorator: public INotification{
    public:
    INotification* notification;
    INotificationDecorator(INotification* notification){
        this->notification=notification;
    }
    virtual ~INotificationDecorator(){
        delete notification;
    }
};

class TimstampDecorator:public INotificationDecorator{
    public:
    TimstampDecorator(INotification* notification):INotificationDecorator(notification){
    }
    string getContent() override{
        return notification->getContent()+ " 12/12/2001";
    }
};
class SignatureDecorator:public INotificationDecorator{
    string sign;
    public:
    SignatureDecorator(INotification* notification,const string &sign):INotificationDecorator(notification){
        this->sign=sign;
    }
    string getContent() override{
        return notification->getContent()+ " "+sign;
    }
};

class IObserver{
    public:
    virtual void update()=0;
};

class IObservable{
    public:
    virtual void addObserver(IObserver*)=0;
    virtual void removeObserver(IObserver*)=0;
    virtual void notifyObservers()=0;
};

class NotificationObservable: public IObservable{
    private:
    vector<IObserver*> observers;
    INotification * notification;
    public:
    NotificationObservable(){
        notification=NULL;
    }
    void addObserver(IObserver* ob) override{
        observers.push_back(ob);
    }
    void removeObserver(IObserver* ob) override{
        auto it = find(observers.begin(), observers.end(), ob);
        
        if(it != observers.end()){
            observers.erase(it);
        }    
    }
    void notifyObservers(){
        for(auto it:observers){
            it->update();
        }
    }
    INotification* getNotification(){
        return this->notification;
    }
    void setNotification(INotification* nt){
        if(notification!=NULL){
            delete notification;
        }
        notification=nt;
        notifyObservers();
    }
    string getNotificationContent(){
        return this->notification->getContent();
    }
};

class Logger: public IObserver{
    NotificationObservable* notificationObservable;
    public:
    Logger(NotificationObservable* no){
        notificationObservable=no;
    }
    void update() override{
        cout<<"Logging "<<notificationObservable->getNotificationContent()<<endl;
    }
};

//startegy Design pattern
class INotificationStrategy{
    public:
    virtual void sendNotification(string)=0;
};
class EmailNotification:public INotificationStrategy{
    private:
    string email;
    public:
    EmailNotification(string email){
        this->email=email;
    }
    void sendNotification(string content) override{
        cout<<"sending email notification to "<<email<<": "<<content<<endl;
    }
};
class SMSNotification:public INotificationStrategy{
    private:
    string number;
    public:
    SMSNotification(string number){
        this->number=number;
    }
    void sendNotification(string content) override{
        cout<<"sending sms notification to "<<number<<": "<<content<<endl;
    }
};
class NotificationEngine: public IObserver{
    NotificationObservable* notificationObservable;
    vector<INotificationStrategy*> notificationStrategies;
    public:
    NotificationEngine(NotificationObservable* no){
        notificationObservable=no;
    }
    void addNotificationStarategy(INotificationStrategy* ns){
        notificationStrategies.push_back(ns);
    }
    void update(){
        for(auto it: notificationStrategies){
            it->sendNotification(notificationObservable->getNotificationContent());
        }
    }
};

// notification service(singleton class)

class NotificationService{
    private:
    NotificationObservable* observable;
    static NotificationService* instance;
    vector<INotification*> notifications;
    NotificationService(){
        observable=new NotificationObservable();
    }
    public:
    static NotificationService* getInstance(){
        if(instance==NULL){
            instance=new NotificationService();
        }
        return instance;
    }
    NotificationObservable* getObservable(){
        return observable;
    }
    void sendNotification(INotification* notification){
        notifications.push_back(notification);
        observable->setNotification(notification);
    }
};
NotificationService* NotificationService::instance=NULL;

int main()
{
    NotificationService* notificationService=NotificationService::getInstance();
    
    NotificationObservable* notificationObservable=notificationService->getObservable();
    Logger *logger=new Logger(notificationObservable);
    
    NotificationEngine* notificationEngine= new NotificationEngine(notificationObservable);
    
    notificationEngine->addNotificationStarategy(new EmailNotification("abc@example.com"));
    notificationEngine->addNotificationStarategy(new SMSNotification("3222-12-223"));
    
    notificationObservable->addObserver(logger);
    notificationObservable->addObserver(notificationEngine);
    
    INotification* notification= new TimstampDecorator(new SignatureDecorator(new SimpleNotification("Order Delivered"),"tomato"));
    notificationService->sendNotification(notification);

    return 0;
}