#include "natebot13_treat.h"

namespace natebot13_treat {

treat::treat(int completed_games, const mj::game_data& data) {
  _total_frames = 5 * 60;
  bool take = data.random.get_fixed(1) > .5;
}

}