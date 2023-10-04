#include "natebot13_treat.h"

#include "mj/mj_game_list.h"

#include "bn_keypad.h"
#include "bn_math.h"

#include "bn_regular_bg_items_natebot13_treat_candy.h"
#include "bn_regular_bg_items_natebot13_treat_hand_close.h"
#include "bn_regular_bg_items_natebot13_treat_hand_open.h"

MJ_GAME_LIST_ADD(natebot13_treat::treat)

namespace natebot13_treat {

int mod(int i, int n) { return (i % n + n) % n; }

hand::hand(candy &candy)
    : _bg{bn::regular_bg_items::natebot13_treat_hand_open.create_bg(0, 0)},
      _candy{candy} {}

void hand::update() {
  closed = bn::keypad::a_held();
  if (bn::keypad::a_pressed()) {
    _bg = bn::regular_bg_items::natebot13_treat_hand_close.create_bg(
        _bg.position());
    if (_candy.get_rect().intersects(get_rect())) {
      _candy.grabbed = true;
    }
  }

  if (bn::keypad::a_released()) {
    _bg = bn::regular_bg_items::natebot13_treat_hand_open.create_bg(
        _bg.position());
    _candy.grabbed = false;
  }

  bn::fixed_point move;
  move.set_x(bn::keypad::right_held() - bn::keypad::left_held());
  move.set_y(bn::keypad::down_held() - bn::keypad::up_held());
  _bg.set_position(_bg.position() + move * speed);

  if (_candy.grabbed) {
    _candy.move(move * speed);
  }
}

bn::fixed_rect hand::get_rect() {
  bn::fixed_point offset = _bg.position() + bn::fixed_point{128, 128 + 64};
  bn::fixed_point p{mod(offset.x().integer(), 256),
                    mod(offset.y().integer(), 256)};
  return bn::fixed_rect(p, bn::size{16 + 24, 16});
}

candy::candy()
    : _bg{bn::regular_bg_items::natebot13_treat_candy.create_bg(0, 0)} {}

bn::fixed_rect candy::get_rect() {
  bn::fixed_point offset = _bg.position() + bn::fixed_point{128, 128 - 73};
  bn::fixed_point p{mod(offset.x().integer(), 256),
                    mod(offset.y().integer(), 256)};
  return bn::fixed_rect(p, bn::size{24, 16});
}

treat::treat([[maybe_unused]] int completed_games,
             [[maybe_unused]] const mj::game_data &data) {
  _total_frames = 10 * 60;
  bool take = data.random.get_fixed(1) > .5;
  if (take) {
    _bg.set_y(_bg.y() + 142);
  }
}

bn::fixed_rect treat::get_rect() {
  bn::fixed_point offset = _bg.position() + bn::fixed_point{128, 128 + 64};
  bn::fixed_point p{mod(offset.x().integer(), 256),
                    mod(offset.y().integer(), 256)};
  return bn::fixed_rect(p, bn::size{64, 64});
}

mj::game_result treat::play([[maybe_unused]] const mj::game_data &data) {
  mj::game_result result;
  _hand.update();

  if (!_candy.grabbed && get_rect().intersects(_candy.get_rect())) {
    _victory = true;
  } else {
    _victory = false;
  }
  return result;
}

void treat::fade_in([[maybe_unused]] const mj::game_data &data) {}

void treat::fade_out([[maybe_unused]] const mj::game_data &data) {}

} // namespace natebot13_treat