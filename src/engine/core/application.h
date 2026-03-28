// Virtual class that an application can override to gain access to the basic engine functions.

#pragma once

class Application {
public:
    virtual ~Application() = default;
    virtual bool onStart() { return true; }
    virtual void onUpdate() {}
    virtual void onRender() {}
};