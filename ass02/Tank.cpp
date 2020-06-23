#include "Tank.h"

// ... implement
//constructor
Tank::Tank(Team team, const Vec2 &pos, AttackPolicy pol, bool _bounce)
  : Unit(team, pos, 20, 80, 15, 4, 100){
    policy = pol;
    bounce = _bounce;
}
//act by policy
void Tank::act(World &w){
  if(policy == ATTACK_WEAKEST && w.random_weakest_target(*this)!=NULL){
    w.attack(*this, *w.random_weakest_target(*this));
  }
  else if(policy == ATTACK_CLOSEST && w.random_closest_target(*this)!=NULL){
    w.attack(*this, *w.random_closest_target(*this));
  }
  else if(policy == ATTACK_MOST_DANGEROUS && w.random_most_dangerous_target(*this)!=NULL){
    w.attack(*this, *w.random_most_dangerous_target(*this));
  }
}
//change heading when collision
void Tank::collision_hook(double prev_speed, bool collisions[4]){
  current_speed = prev_speed;
  if(collisions[0]||collisions[1]){
    heading.x=-heading.x;
  }
  if(collisions[2]||collisions[3]){
    heading.y=-heading.y;
  }
}
