#ifndef HEALTH_COMPONENT_H
#define HEALTH_COMPONENT_H

struct HealthComponent {
  int maxHealth;
  int currentHealth;

  HealthComponent(int maxHealth = 100) : maxHealth(maxHealth) {
    currentHealth = maxHealth;
  }
};

#endif
