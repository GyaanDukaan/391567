#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

class Observer {
public:
    virtual void update(const std::string& message) = 0;
};

class Subject {
private:
    std::vector<Observer*> observers;
    std::string state;
public:
    void attach(Observer* observer) {
        observers.push_back(observer);
    }

    void detach(Observer* observer) {
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }

    void setState(const std::string& newState) {
        state = newState;
        notify();
    }

    void notify() {
        for (auto& observer : observers) {
            observer->update(state);
        }
    }
};

// A mock observer for testing
class MockObserver : public Observer {
public:
    std::string receivedMessage;
    
    void update(const std::string& message) override {
        receivedMessage = message;
    }
};

#include <gtest/gtest.h>

TEST(ObserverPatternTest, SingleObserverReceivesNotification) {
    Subject subject;
    MockObserver observer;

    subject.attach(&observer);
    subject.setState("New State");

    EXPECT_EQ(observer.receivedMessage, "New State");
}

TEST(ObserverPatternTest, MultipleObserversReceiveNotification) {
    Subject subject;
    MockObserver observer1, observer2;

    subject.attach(&observer1);
    subject.attach(&observer2);
    subject.setState("Another State");

    EXPECT_EQ(observer1.receivedMessage, "Another State");
    EXPECT_EQ(observer2.receivedMessage, "Another State");
}

TEST(ObserverPatternTest, DetachObserver) {
    Subject subject;
    MockObserver observer;

    subject.attach(&observer);
    subject.detach(&observer);
    subject.setState("State After Detach");

    EXPECT_EQ(observer.receivedMessage, ""); // Should not receive any updates
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}