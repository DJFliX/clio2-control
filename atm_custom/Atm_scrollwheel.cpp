#include "Atm_scrollwheel.hpp"

Atm_scrollwheel &Atm_scrollwheel::begin()
{
    // clang-format off
  const static state_t state_table[] PROGMEM = {
    /* Standard Mode: press/repeat */
    /*            ON_ENTER    ON_LOOP   ON_EXIT   EVT_AH  EVT_AL  EVT_BH  EVT_BL  EVT_CH  EVT_CL  ELSE */
    /* A    */       ENT_A,        -1,    EXT_A,      -1,  TRANS,  TRANS,  TRANS,  TRANS, TRANS,    -1,
    /* B    */       ENT_B,        -1,    EXT_B,   TRANS,  TRANS,     -1,  TRANS,  TRANS, TRANS,    -1,    
    /* C    */       ENT_C,        -1,    EXT_C,   TRANS,  TRANS,  TRANS,  TRANS,     -1, TRANS,    -1,
    /* TRANS*/          -1,        -1,       -1,       A,     -1,      B,     -1,      C,    -1,    -1
  };

    // clang-format on
    Machine::begin(state_table, ELSE);
    return *this;
}

int Atm_scrollwheel::event(int id)
{
    return 0;
}

void Atm_scrollwheel::action(int id)
{
    switch (id)
    {
        case EXT_A:
            last_known_state = A;
            break;
        case EXT_B:
            last_known_state = B;
            break;
        case EXT_C:
            last_known_state = C;
            break;
        case ENT_A:
            if(last_known_state == B) up.push(0, 1);
            if(last_known_state == C) down.push(0, 0);
            break;
        case ENT_B:
            if(last_known_state == C) up.push(0, 1);
            if(last_known_state == A) down.push(0, 0);
            break;
        case ENT_C:
            if(last_known_state == B) down.push(0, 0);
            if(last_known_state == A) up.push(0, 1);
            break;
    }
}

Atm_scrollwheel &Atm_scrollwheel::onUp(atm_cb_push_t callback, int idx /* = 0 */)
{
  up.set(callback, idx);
  return *this;
}

Atm_scrollwheel &Atm_scrollwheel::onDown(atm_cb_push_t callback, int idx /* = 0 */)
{
  down.set(callback, idx);
  return *this;
}

Atm_scrollwheel &Atm_scrollwheel::trace(Stream &stream)
{
  setTrace(&stream, atm_serial_debug::trace,
    "SCROLL\0EVT_AH\0EVT_AL\0EVT_BH\0EVT_BL\0EVT_CH\0EVT_CL\0ELSE\0A\0B\0C\0TRANS");
  return *this;
}