#pragma once

#include "mj/mj_game.h"

#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_items_natebot13_treat_bowl.h"
#include "bn_regular_bg_items_natebot13_treat_hand_open.h"
#include "bn_regular_bg_items_natebot13_treat_hand_close.h"

#include "bn_fixed_rect.h"
#include "bn_fixed.h"

namespace natebot13_treat {

class candy {
 public:
  candy();
  void move(bn::fixed_point amount) { _bg.set_position(_bg.position() + amount); }
  bn::fixed_rect get_rect();
  bool grabbed = false;
 private:
  bn::regular_bg_ptr _bg;
};

class hand {
 public:
  hand(candy& candy);
  bool closed = false;
  void update();
  bn::fixed_rect get_rect();
  bn::fixed speed = 3;
 private:
  bn::regular_bg_ptr _bg;
  candy& _candy;
};

class treat : public mj::game {
 public:
  treat(int completed_games, const mj::game_data& data);

  [[nodiscard]] bn::string<16> title() const final { return "Treat!"; }

  [[nodiscard]] int total_frames() const final { return _total_frames; }

  void fade_in(const mj::game_data& data) final;

  [[nodiscard]] mj::game_result play(const mj::game_data& data) final;

  [[nodiscard]] bool victory() const final { return _victory; }

  void fade_out(const mj::game_data& data) final;

  bn::fixed_rect get_rect();

 private:
  int _total_frames;
  bool _victory = false;
  bn::regular_bg_ptr _bg = bn::regular_bg_items::natebot13_treat_bowl.create_bg(0, 0);
  candy _candy;
  hand _hand{_candy};
};

}  // namespace natebot13_treat