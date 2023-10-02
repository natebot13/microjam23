#include "natebot13_spook.h"

#include "bn_keypad.h"
#include "bn_sprite_first_attributes.h"

#include "bn_math.h"
#include "mj/mj_game_list.h"

#include "bn_sprite_items_natebot13_spook_light.h"

MJ_GAME_LIST_ADD(natebot13::spook)

namespace natebot13 {

bn::fixed normalize_angle(bn::fixed angle) {
  return angle = angle > 1 ? angle - 1 : angle < 0 ? 1 - angle : angle;
}

bool angle_in_range(bn::fixed angle, bn::fixed lower, bn::fixed upper) {
  angle = normalize_angle(angle);
  lower = normalize_angle(lower);
  upper = normalize_angle(upper);
  if (lower > upper) {
    return angle >= lower || angle <= upper;
  }
  return angle > lower && angle < upper;
}

spook::spook(int completed_games, const mj::game_data& data) {
  constexpr int frames_diff = maximum_frames - minimum_frames;
  constexpr int maximum_speed_completed_games = 30;

  completed_games = bn::min(completed_games, maximum_speed_completed_games);

  int frames_reduction = (frames_diff * completed_games) / maximum_speed_completed_games;
  _total_frames = maximum_frames - frames_reduction;
  _total_frames -= data.random.get_int(60);
  _total_frames = bn::clamp(_total_frames, minimum_frames, maximum_frames);
  _angle = data.random.get_fixed(1);

  int level = bn::min(completed_games / 10 + 1, 4);
  _speed += 0.001 * (level - 1);
  for (int i = 0; i < level; i += 1) {
    _spookees.push_back(bn::optional<person>(data.random.get_fixed(1)));
  }
}

void spook::fade_in([[maybe_unused]] const mj::game_data& data) { move(); }

mj::game_result spook::play(const mj::game_data& data) {
  mj::game_result result;
  result.exit = data.pending_frames == 0;

  move();
  
  bool all_spooked = true;
  person* in_range_spookee = nullptr;
  for (auto& maybe_spookee : _spookees) {
    if (maybe_spookee.has_value()) {
      auto spookee = maybe_spookee.get();
      all_spooked &= spookee->is_spooked();
      bool in_range = angle_in_range(spookee->angle(), _angle - 0.05, _angle + 0.05);
      if (in_range) {
        if (!spookee->is_spooked()) {
          in_range_spookee = spookee;
        }
      } else if (!_defeat) _sprite.set_scale(1);
      spookee->update();
    }
  }

  if (!_victory && !_defeat) {
    if (bn::keypad::a_pressed()) {
      if (in_range_spookee) {
        _sprite.set_scale(1.2);
        in_range_spookee->spook();
        data.small_text_generator.generate(_sprite.position(), "BOO!", _info_sprites);
      } else {
        for (auto& maybe_spookee : _spookees) {
          if (maybe_spookee.has_value()) {
            auto spookee = maybe_spookee.get();
            if (!spookee->is_spooked()) spookee->notice();
          }
        }
        _sprite.set_bg_priority(2);
        _light = bn::sprite_items::natebot13_spook_light.create_sprite(_sprite.position());
        _light.get()->set_scale(2);
        _defeat = true;
      }
    }


    if (all_spooked && !_defeat && !_victory) {
      result.remove_title = true;
      _victory = true;
    }
  } else {
    if (_show_result_frames) {
      _show_result_frames--;
      if (_defeat) {
        _sprite.set_scale(bn::max(bn::fixed(_show_result_frames) / 60, bn::fixed(0.001)));
      }
    } else {
      result.exit = true;
    }
  }

  return result;
}

void spook::move() {
  if (!_defeat) {
    _angle += _speed;
    if (_angle > 1) {
      _angle = 0;
    }

    _sprite.set_horizontal_flip(_angle < .5);

    _sprite.set_position(bn::cos(_angle) * 40, bn::sin(_angle) * 40);
  }
}

void spook::fade_out([[maybe_unused]] const mj::game_data& data) { move(); }

person::person(bn::fixed angle) {
  _sprite.set_bg_priority(2);
  _angle = angle;
  _sprite.set_position(bn::cos(_angle) * 50, bn::sin(_angle) * 50);
}

void person::update() {
  if (_spooked) {
    _sprite.set_position(bn::cos(_angle) * _distance, bn::sin(_angle) * _distance);
    _distance += 0.5;
  }

}

void person::spook() {
  _sprite = bn::sprite_items::natebot13_spook_person.create_sprite(_sprite.position(), 1);
  _sprite.set_bg_priority(2);
  _sprite.set_horizontal_flip(_angle > .25 && _angle < .75);
  _spooked = true;
}

void person::notice() {
  _sprite = bn::sprite_items::natebot13_spook_person.create_sprite(_sprite.position(), 2);
  _sprite.set_bg_priority(2);
  _sprite.set_horizontal_flip(_angle < .25 || _angle > .75);
}

}  // namespace natebot13
