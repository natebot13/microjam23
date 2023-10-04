#pragma once

#include "mj/mj_game.h"

#include "bn_sprite_ptr.h"
#include "bn_sprite_items_natebot13_spook_ghost.h"

namespace natebot13_treat {

class hand {
  bn::sprite_ptr _sprite = bn::sprite_items::natebot13_spook_ghost.create_sprite(0, 0);
};

class treat : mj::game {
 public:
  treat(int completed_games, const mj::game_data& data);

  [[nodiscard]] bn::string<16> title() const final { return "Spook!"; }

  [[nodiscard]] int total_frames() const final { return _total_frames; }

  void fade_in(const mj::game_data& data) final;

  [[nodiscard]] mj::game_result play(const mj::game_data& data) final;

  [[nodiscard]] bool victory() const final { return _victory; }

  void fade_out(const mj::game_data& data) final;

 private:
  int _total_frames;
  bool _victory = false;
};

}  // namespace natebot13_treat