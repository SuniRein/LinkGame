#ifndef GAME_LINK_GAME_MAP_GENERATOR_H_
#define GAME_LINK_GAME_MAP_GENERATOR_H_

#include "link_game.h"

namespace game {

class link_game::map_generator {
public:
  map_generator() = default;
  ~map_generator() = default;

  map_generator(const map_generator&) = delete;
  map_generator& operator=(const map_generator&) = delete;

  void operator() (link_game&) const;

private:
};

}

#endif