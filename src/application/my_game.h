#pragma once
#include "engine/core/application.h"

class MyGame : public Application {
public:
    bool onStart()  override;
    void onUpdate() override;
    void onRender() override;
};