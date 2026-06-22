#pragma once

#include "PowerUp.h"

class SlowPowerUp : public PowerUp {
public:
    SlowPowerUp();
    void spawn(int randomValue) override;
};