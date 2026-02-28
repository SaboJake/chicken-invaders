#pragma once
#include <functional>

class RepeatedAction {
public:
    RepeatedAction(int n, float t, std::function<void()> action)
        : times(n), interval(t), action(action), elapsedTime(0.0f) {
    }

    void Update(float deltaTime) {
        if (times <= 0) return;

        elapsedTime += deltaTime;
        if (elapsedTime >= interval) {
            elapsedTime -= interval;
            action();
            times--;
        }
    }

    bool IsFinished() const {
        return times <= 0;
    }

    void Reset(int n, float t) {
        times = n;
        interval = t;
        elapsedTime = 0.0f;
	}

    int times;
private:
    float interval;
    float elapsedTime;
    std::function<void()> action;
};