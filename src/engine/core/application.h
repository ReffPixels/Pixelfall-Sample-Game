// Virtual class that an application can override to gain access to the basic engine functions.

#pragma once
#include <memory>

class Application {
public:
    static std::unique_ptr<Application> create();
    virtual ~Application() = default;
    virtual bool onStart() { return true; }
    virtual void onUpdate() {}
    virtual void onRender() {}
};