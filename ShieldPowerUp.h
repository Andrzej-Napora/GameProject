#pragma once

#include "PowerUp.h"

class ShieldPowerUp : public PowerUp {
public:
    ShieldPowerUp();
    void spawn(int randomValue) override;
};