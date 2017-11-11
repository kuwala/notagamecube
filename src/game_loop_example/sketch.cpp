/*
  A simple game loop with FPS control and no interpolation.
*/

#include <Arduino.h>
#include <climits>

#define __ASSERT_USE_STDERR

// uncomment to disable assert()
// #define NDEBUG
#include <cassert>

// prints diagnostic info given by an assert and aborts the program
void __assert(char const* __func, char const* __file, int __lineno, char const* __sexp)
{
  Serial.println(__func);
  Serial.println(__file);
  Serial.println(__lineno, DEC);
  Serial.println(__sexp);
  Serial.flush();

  abort();
}

namespace game {
  typedef unsigned long time_t;
};

// displays the current game state
void display() {}

// processes user input
void process_input() {}

// updates the game state
void update()
{
  process_input();
}

// treat this function like main() in order to avoid globals
// TODO: figure out whether serialEventRun() needs to be called periodically
void loop()
{
  using game::time_t;

  ////////////////////////////////////////////////
  // Game loop configuration.
  // Controls game speed and FPS.
  ////////////////////////////////////////////////

  // controls the game speed; lower is faster
  time_t const ms_per_update = 20;
  // minimum times per second that display() should be called
  // once fps dips below this value, display() will be called before any more update() calls
  // this value can be defined relative to how many update() calls have occurred since the last display(), e.g.:
  // "1000 / (ms_per_update * 5)" to call display() at least once every 5 update() calls
  size_t const min_fps = 0;
  // maximum times per second that display() is called
  // set to 0 for no fps cap
  size_t const max_fps = 0;

  // sanity
  assert(min_fps <= max_fps || max_fps == 0);
  // limit precision to milliseconds
  assert(min_fps <= 1000);
  assert(max_fps <= 1000);

  ////////////////////////////////////////////////
  // Initialization and game loop.
  ////////////////////////////////////////////////

  Serial.begin(115200);

  // limit the frequency of printed warnings
  time_t const ms_per_warning = 1000;
  time_t last_warning = 0;

  // maximum and minimum milliseconds elapsed between calls to to display()
  time_t const max_ms_per_display = (min_fps == 0) ? ULONG_MAX : (1000 / min_fps);
  time_t const min_ms_per_display = (max_fps == 0) ? 0 : (1000 / max_fps);
  // the time (in total elapsed milliseconds) of the next allowed update() call
  time_t next_update = 0;
  // the time (in total elapsed milliseconds) of the most recent display() call
  time_t previous_display = 0;

  // main game loop
  for (;;) {
    // update game state
    bool below_min_fps = false;
    while (millis() >= next_update && !below_min_fps) {
      update();
      next_update += ms_per_update;
      below_min_fps = (millis() - previous_display) >= max_ms_per_display;
    }

    // check fps
    if (below_min_fps && millis() >= (last_warning + ms_per_warning)) {
      Serial.println("WARNING: Dropped below minimum FPS. Either something is taking a long time or the minimum FPS is set too high.");
      last_warning = millis();
    }

    // display game state
    if (millis() >= (previous_display + min_ms_per_display)) {
      display();
      previous_display = millis();
    }
  }
}

// runs exactly once after the board is powered up or reset
// intentionally left empty; do all initialization in loop()
void setup() {}
