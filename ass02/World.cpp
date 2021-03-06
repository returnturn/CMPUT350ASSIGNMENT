#include <cmath>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include "World.h"
#include "Unit.h"

using namespace std;

/* helper that you need to implement
   
 - move unit by (current_speed * heading)

 - units only collide with the map border
 
 - when a unit hits a wall it stops at the collision point (current_speed=0)
   and collision_hook needs to be called with the previous speed and an array
   of bools encoding all the walls that were hit (the array indexes need to be
   consistent with enum CollDir). This array is used to implement bouncing by
   only changing the unit's heading, i.e. even when bouncing the unit stays at
   the collision location for the remainder of the simulation frame and only
   starts moving again in the following frame

 - it is essential that units are located inside the map at all times. There
   can be no overlap. Ensure this property by clipping coordinates after
   moving

*/

void World::move_unit(Unit &u)
{
  // ... implement
  //for checking collisions with four borders {L,R,U,D}
  bool collisions[4] = {false, false, false, false};
  double movingX, movingY, newPosX, newPosY;
  //moving distance
  movingX = u.current_speed * u.heading.x;
  movingY = u.current_speed * u.heading.y;
  //new position
  newPosX = u.pos.x + movingX;
  newPosY = u.pos.y + movingY;
  double previous_speed = u.current_speed;
  //left collision
  if(newPosX < u.radius){
    collisions[0] = true;
  }
  //right collision
  if(newPosX > width-(u.radius)){
    collisions[1] = true;
  }
  //up collision
  if(newPosY < u.radius){
    collisions[2] = true;
  }
  //down collision
  if(newPosY > height-(u.radius)){
    collisions[3] = true;
  }
  //collision happened
  if(collisions[0]||collisions[1]||collisions[2]||collisions[3]){
    u.current_speed = 0;
    u.collision_hook(previous_speed, collisions);
  }
  //no collision
  else{
    u.pos.x = newPosX;
    u.pos.y = newPosY;
  }
}


// returns policy name
const char *apol2str(AttackPolicy pol)
{
  switch (pol) {
    case ATTACK_WEAKEST:
      return "attack-weakest";
    case ATTACK_CLOSEST:
      return "attack-closest";
    case ATTACK_MOST_DANGEROUS:
      return "attack-most-dangerous";
  }
  return "?";
}


World::~World()
{
  // clean up
  
  // ... implement
  units.clear();
}


// deducts hit points and calls attack_hook

void World::attack(Unit &attacker, Unit &attacked)
{
  // ... implement
  //deduct hp
  attacked.hp -= attacker.damage;
  attack_hook(attacker, attacked);
}


// return a random position at which a circle of that radius fits

Vec2 World::rnd_pos(double radius) const
{
  double slack = radius * 2;

  Vec2 p((width  - 2*slack) * rnd01() + slack,
         (height - 2*slack) * rnd01() + slack);
  
  // assert circle in rectangle
  assert(p.x > radius && p.x < width - radius);
  assert(p.y > radius && p.y < height - radius);
  return p;
}


// return uniform random heading
// length of vector = 1

Vec2 World::rnd_heading() const
{
  // ... implement
  //x^2+y^2=1
  double v_x = rnd01();
  double v_y = sqrt(1-pow(v_x,2));
  if(rnd01()>=0.5){
    v_x = -v_x;
  }
  if(rnd01()>=0.5){
    v_y = -v_y;
  }
  Vec2 v(v_x,v_y);
  return v;
}

// mirror position with respect to map mid-point

Vec2 World::mirror(const Vec2 &pos) const
{
  // ... implement
  //reflect around mid point (width/2,height/2)
  double mirror_x = width/2 - (pos.x - width/2);
  double mirror_y = height/2 - (pos.y - height/2);
  Vec2 mv(mirror_x, mirror_y);
  return mv;
}


// return squared distance between two unit centers

double World::distance2(const Unit &u, const Unit &v)
{
  // ... implement
  //distance between two points
  double dis = sqrt(pow((u.pos.x-v.pos.x),2)+pow((u.pos.y-v.pos.y),2));
  return dis;
}

// return true iff u can attack v, i.e. distance of u's and v's centers is
// less than u's attack distance plus v's radius
bool World::can_attack(const Unit &u, const Unit &v)
{
  // ... implement
  //units within attact range
  return (distance2(u,v) < u.attack_radius+v.radius);
}

// populate a vector with enemy units that can be attacked by u;
// clears vector first
void World::enemies_within_attack_range(const Unit &u,
                                        vector<Unit*> &targets) const
{
  targets.clear();

  // ... implement

  // use push_back to add elements to targets
  for(unsigned int i=0; i<units.size(); i++){
    //check if different team
    if(u.team != units[i]->team){
      //check if in range
      if(can_attack(u,*units[i])){
        targets.push_back(units[i]);
      }
    }
  }
}

// return a random unit that can be attacked by u with minimal hp_old value,
// or 0 if none exists

Unit *World::random_weakest_target(Unit &u) const
{
  vector<Unit*> targets;
  
  // ... implement
  //give enemies in range
  enemies_within_attack_range(u, targets);
  if(targets.size() == 0){
    return 0;
  }
  vector<Unit*> weak_v;
  double low = 1000;
  for(unsigned int i=0; i<targets.size(); i++){
    //even weaker
    if(targets[i]->hp_old < low){
      low = targets[i]->hp_old;
      weak_v.clear();
      weak_v.push_back(targets[i]);
    }
    //same weak level
    else if(targets[i]->hp_old == low){
      weak_v.push_back(targets[i]);
    }
  }
  return weak_v[rnd_int(weak_v.size())];
}


// return a random unit that can be attacked by u with minimal distance to
// u, or 0 if none exists

Unit *World::random_closest_target(Unit &u) const
{
  vector<Unit*> targets;
  
  // ... implement
  //give enemies in range
  enemies_within_attack_range(u, targets);
  if(targets.size() == 0){
    return 0;
  }
  vector<Unit*> close_v;
  double minDis = 1000;
  for(unsigned int i=0; i<targets.size(); i++){
    //even closer
    if(distance2(u,*targets[i]) < minDis){
      minDis = distance2(u,*targets[i]);
      close_v.clear();
      close_v.push_back(targets[i]);
    }
    //same shortest distance
    else if(targets[i]->hp_old == minDis){
      close_v.push_back(targets[i]);
    }
  }
  return close_v[rnd_int(close_v.size())];
}


// return a random unit that can be attacked by u with maximal damage/hp_old
// ratio, or 0 if none exists

Unit *World::random_most_dangerous_target(Unit &u) const
{
  vector<Unit*> targets;

  // ... implement
  //give enemies in range
  enemies_within_attack_range(u, targets);
  if(targets.size() == 0){
    return 0;
  }
  vector<Unit*> dan_v;
  double maxDan = 0;
  for(unsigned int i=0; i<targets.size(); i++){
    //more dangerous ever
    if(((targets[i]->damage)/(targets[i]->hp_old)) > maxDan){
      maxDan = (targets[i]->damage)/(targets[i]->hp_old);
      dan_v.clear();
      dan_v.push_back(targets[i]);
    }
    //same level
    else if(((targets[i]->damage)/(targets[i]->hp_old)) == maxDan){
      dan_v.push_back(targets[i]);
    }
  }
  return dan_v[rnd_int(dan_v.size())];
}


// return score for red: 2 for win, 0 for loss, 1 for draw, -1 for game not
// over yet

int World::red_score() const
{
  if (units.empty()) {
    return 1;
  }

  // ... implement
  //any RED? any BLUE?
  bool R=false, B=false;
  for(unsigned int i=0; i<units.size(); i++){
    if(units[i]->team == BLUE){
      B=true;
    }
    else if(units[i]->team == RED){
      R=true;
    }
    //not finished
    if(R && B){
      return -1;
    }
  }
  //red only, win
  if(R){
    return 2;
  }
  //lose
  else{
    return 0;
  }
}

