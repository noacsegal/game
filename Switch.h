#pragma once
#include "point.h"

class Switch
{
public:
    static constexpr char OPEN = '/';
    static constexpr char CLOSED = '\\'; // the char: \

private:
    point place;
    bool active;

public:
    Switch(point p, bool start) : place(p), active(start) {}

    void draw() {
        place.draw(active ? OPEN : CLOSED);
    }

    bool isOn() {
        return active;
    }

    void toggle() {
        active = !active;
        place.changeChar(active ? OPEN : CLOSED);
    }

    void setState(bool state) {
        active = state;
    }

    point getPlace() {
        return place;
    }
};