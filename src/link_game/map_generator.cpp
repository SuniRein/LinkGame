#include "map_generator.h"

#include "kit/random.h"
#include "kit/random_pool.h"

#include <set>

#define array_size(array) (sizeof(array) / sizeof(array[0]))

namespace game {

namespace {
  gl::vec4 colors[] = {
    { 1.0f, 0.0f, 0.0f, 1.0f  },  // RED
    { 0.0f, 1.0f, 0.0f, 1.0f  },  // GREEN
    { 0.0f, 0.0f, 1.0f, 1.0f  },  // BLUE
    { 1.0f, 1.0f, 0.0f, 1.0f  },  // YELLOW
    { 1.0f, 0.0f, 1.0f, 1.0f  },  // PINK
    { 0.0f, 1.0f, 1.0f, 1.0f  },  // CYAN
  };
}

void link_game::map_generator::operator() (link_game& game) const {
  using SuniRein_kit::random;
  using SuniRein_kit::random_element;
  using SuniRein_kit::random_pool;

  auto count_x = game.get_count_x();
  auto count_y = game.get_count_y();

  static constexpr pos_t directions[] = {
    { 1,  0}, // RIGHT
    {-1,  0}, // LEFT
    { 0, -1}, // UP
    { 0,  1}, // DOWN
  };
  
  static std::set<pos_t> visited;
  static random_pool<pos_t> pool;

  visited.clear();
  pool.clear();

  pos_t origin = { random(0, count_x - 1), random(0, count_y - 1) };
  visited.insert(origin);
  pool.push(origin);

  auto add_nearby = [count_x, count_y] (pos_t now) {
    for(auto i = 0; i != array_size(directions); ++i) {
      auto new_pos = now + directions[i];
      if(new_pos.x >= 0 && new_pos.x < count_x && new_pos.y >= 0 && new_pos.y < count_y &&
         visited.find(new_pos) == visited.end()) {
        pool.push(new_pos);
        visited.insert(new_pos);
      }
    }
  };

  while(!pool.empty()) {
    auto first = pool();
    add_nearby(first.value());
    auto second = pool();
    add_nearby(second.value());

    auto color = random_element(colors, array_size(colors));
    game.m_map->set_color(first.value(), color);
    game.m_map->set_color(second.value(), color);
  }
}

}