#pragma once

class Application {
public:
    virtual ~Application() = default;
    virtual bool onStart()  { return true; }
    virtual void onUpdate() {}
    virtual void onRender() {}
};