#pragma once

#include "bn_fixed.h"
#include "bn_sprite_items_natebot13_spook_ghost.h"
#include "bn_sprite_items_natebot13_spook_person.h"
#include "bn_sprite_ptr.h"
#include "bn_vector.h"
#include "bn_optional.h"

#include "mj/mj_game.h"

namespace natebot13_spook {

class person {
 public:
  person(bn::fixed angle);
  void spook();
  void notice();
  bool is_spooked() { return _spooked; }
  bn::fixed angle() { return _angle; }
  void update();

 private:
  bn::sprite_ptr _sprite = bn::sprite_items::natebot13_spook_person.create_sprite(0, 0);
  bn::fixed _angle;
  bn::fixed _distance = 50;
  bool _spooked = false;
};

class spook : public mj::game {
 public:
  spook(int completed_games, const mj::game_data& data);

  [[nodiscard]] bn::string<16> title() const final { return "Spook!"; }

  [[nodiscard]] int total_frames() const final { return _total_frames; }

  void fade_in(const mj::game_data& data) final;

  [[nodiscard]] mj::game_result play(const mj::game_data& data) final;

  [[nodiscard]] bool victory() const final { return _victory; }

  void fade_out(const mj::game_data& data) final;

 private:
  void move();
  bn::sprite_ptr _sprite = bn::sprite_items::natebot13_spook_ghost.create_sprite(0, 0);
  bn::optional<bn::sprite_ptr> _light;
  int _total_frames;
  int _show_result_frames = 60;
  bool _victory = false;
  bool _defeat = false;
  bn::fixed _angle;
  bn::fixed _speed = 0.005;
  bn::vector<bn::optional<person>, 4> _spookees;
  bn::vector<bn::sprite_ptr, 16> _info_sprites;
};

}  // namespace natebot13
