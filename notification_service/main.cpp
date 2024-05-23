#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Observer Interface
class NotificationListener {
public:
    virtual ~NotificationListener() = default;
    virtual void update(const string& message) = 0;
};

// Concrete Observer: Email Notification
class EmailNotification : public NotificationListener {
public:
    void update(const string& message) override {
        cout << "Email Notification: " << message << endl;
    }
};

// Concrete Observer: SMS Notification
class SMSNotification : public NotificationListener {
public:
    void update(const string& message) override {
        cout << "SMS Notification: " << message << endl;
    }
};

// Observable: Notification Service
class NotificationService {
    vector<NotificationListener*> listeners;
public:
    ~NotificationService() {
        for (auto listener : listeners) {
            delete listener;
        }
    }

    void addListener(NotificationListener* listener) {
        listeners.push_back(listener);
    }

    void removeListener(NotificationListener* listener) {
        listeners.erase(remove(listeners.begin(), listeners.end(), listener), listeners.end());
        delete listener;
    }

    void notify(const string& message) {
        for (const auto& listener : listeners) {
            listener->update(message);
        }
    }
};

int main() {
    NotificationService service;

    NotificationListener* emailListener = new EmailNotification();
    NotificationListener* smsListener = new SMSNotification();

    service.addListener(emailListener);
    service.addListener(smsListener);

    service.notify("This is a test notification");

    service.removeListener(emailListener);

    service.notify("This is another test notification");

    // Cleanup remaining listeners manually since NotificationService destructor handles only deletion
    service.removeListener(smsListener);

    return 0;
}
