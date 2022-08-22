#pragma once

#ifdef GLFWAPI
#include "glKeys.h"
#else
#include "glKeys.h" // DX
#endif

namespace Input {

    using EKeyAction = String;
    static THashMap<EKeyAction, EAPIKeyCode> KeyCode;

    inline static void InitKeys()
    {
        KeyCode["Escape"]      = EAPIKeyCode::ESCAPE;
        KeyCode["Enter"]       = EAPIKeyCode::ENTER;
        KeyCode["Tab"]         = EAPIKeyCode::TAB;
        KeyCode["Backspace"]   = EAPIKeyCode::BACKSPACE;
        KeyCode["Insert"]      = EAPIKeyCode::INSERT;
        KeyCode["Delete"]      = EAPIKeyCode::DELETE_;
        KeyCode["Right"]       = EAPIKeyCode::RIGHT;
        KeyCode["Left"]        = EAPIKeyCode::LEFT;
        KeyCode["Down"]        = EAPIKeyCode::DOWN;
        KeyCode["Up"]          = EAPIKeyCode::UP;
        KeyCode["PageUp"]      = EAPIKeyCode::PAGE_UP;
        KeyCode["PageDown"]    = EAPIKeyCode::PAGE_DOWN;
        KeyCode["Home"]        = EAPIKeyCode::HOME;
        KeyCode["End"]         = EAPIKeyCode::END;
        KeyCode["CapsLock"]    = EAPIKeyCode::CAPS_LOCK;
        KeyCode["ScrollLock"]  = EAPIKeyCode::SCROLL_LOCK;
        KeyCode["NumLock"]     = EAPIKeyCode::NUM_LOCK;
        KeyCode["PrintScreen"] = EAPIKeyCode::PRINT_SCREEN;
        KeyCode["Pause"]       = EAPIKeyCode::PAUSE;

        KeyCode["Space"] = EAPIKeyCode::SPACE;
        KeyCode["\'"]    = EAPIKeyCode::APOSTROPHE;
        KeyCode[","]     = EAPIKeyCode::COMMA;
        KeyCode["-"]     = EAPIKeyCode::MINUS;
        KeyCode["."]     = EAPIKeyCode::PERIOD;
        KeyCode["/"]     = EAPIKeyCode::SLASH;
        KeyCode["0"]     = EAPIKeyCode::NUM0;
        KeyCode["1"]     = EAPIKeyCode::NUM1;
        KeyCode["2"]     = EAPIKeyCode::NUM2;
        KeyCode["3"]     = EAPIKeyCode::NUM3;
        KeyCode["4"]     = EAPIKeyCode::NUM4;
        KeyCode["5"]     = EAPIKeyCode::NUM5;
        KeyCode["6"]     = EAPIKeyCode::NUM6;
        KeyCode["7"]     = EAPIKeyCode::NUM7;
        KeyCode["8"]     = EAPIKeyCode::NUM8;
        KeyCode["9"]     = EAPIKeyCode::NUM9;
        KeyCode[";"]     = EAPIKeyCode::SEMICOLON;
        KeyCode["Equal"] = EAPIKeyCode::EQUAL;
        KeyCode["("]     = EAPIKeyCode::LEFT_BRACKET;
        KeyCode["\\"]    = EAPIKeyCode::BACKSLASH;
        KeyCode[")"]     = EAPIKeyCode::RIGHT_BRACKET;

        KeyCode["F1"]  = EAPIKeyCode::F1;
        KeyCode["F2"]  = EAPIKeyCode::F2;
        KeyCode["F3"]  = EAPIKeyCode::F3;
        KeyCode["F4"]  = EAPIKeyCode::F4;
        KeyCode["F5"]  = EAPIKeyCode::F5;
        KeyCode["F6"]  = EAPIKeyCode::F6;
        KeyCode["F7"]  = EAPIKeyCode::F7;
        KeyCode["F8"]  = EAPIKeyCode::F8;
        KeyCode["F9"]  = EAPIKeyCode::F9;
        KeyCode["F10"] = EAPIKeyCode::F10;
        KeyCode["F11"] = EAPIKeyCode::F11;
        KeyCode["F12"] = EAPIKeyCode::F12;

        KeyCode["A"] = EAPIKeyCode::A;
        KeyCode["B"] = EAPIKeyCode::B;
        KeyCode["C"] = EAPIKeyCode::C;
        KeyCode["D"] = EAPIKeyCode::D;
        KeyCode["E"] = EAPIKeyCode::E;
        KeyCode["F"] = EAPIKeyCode::F;
        KeyCode["G"] = EAPIKeyCode::G;
        KeyCode["H"] = EAPIKeyCode::H;
        KeyCode["I"] = EAPIKeyCode::I;
        KeyCode["J"] = EAPIKeyCode::J;
        KeyCode["K"] = EAPIKeyCode::K;
        KeyCode["L"] = EAPIKeyCode::L;
        KeyCode["M"] = EAPIKeyCode::M;
        KeyCode["N"] = EAPIKeyCode::N;
        KeyCode["O"] = EAPIKeyCode::O;
        KeyCode["P"] = EAPIKeyCode::P;
        KeyCode["Q"] = EAPIKeyCode::Q;
        KeyCode["R"] = EAPIKeyCode::R;
        KeyCode["S"] = EAPIKeyCode::S;
        KeyCode["T"] = EAPIKeyCode::T;
        KeyCode["U"] = EAPIKeyCode::U;
        KeyCode["V"] = EAPIKeyCode::V;
        KeyCode["W"] = EAPIKeyCode::W;
        KeyCode["X"] = EAPIKeyCode::X;
        KeyCode["Y"] = EAPIKeyCode::Y;
        KeyCode["Z"] = EAPIKeyCode::Z;

        KeyCode["MB1"] = EAPIKeyCode::MOUSE_BUTTON_1;
        KeyCode["MB2"] = EAPIKeyCode::MOUSE_BUTTON_2;
        KeyCode["MB3"] = EAPIKeyCode::MOUSE_BUTTON_3;
        KeyCode["MB4"] = EAPIKeyCode::MOUSE_BUTTON_4;
        KeyCode["MB5"] = EAPIKeyCode::MOUSE_BUTTON_5;
        KeyCode["MB6"] = EAPIKeyCode::MOUSE_BUTTON_6;
        KeyCode["MB7"] = EAPIKeyCode::MOUSE_BUTTON_7;
        KeyCode["MB8"] = EAPIKeyCode::MOUSE_BUTTON_8;
        KeyCode["MLast"] = EAPIKeyCode::MOUSE_LAST;
        KeyCode["LMB"]  = EAPIKeyCode::MOUSE_LEFT;
        KeyCode["RMB"]  = EAPIKeyCode::MOUSE_RIGHT;
        KeyCode["MMB"]  = EAPIKeyCode::MOUSE_MIDDLE;
    }

    using EKeyCode = EAPIKeyCode;

    static THashMap<EAPIKeyCode, EKeyAction> APIKeyCode;
    
    static void InitAPIKeys()
    {
        for(auto& p : KeyCode)
        {
            APIKeyCode[p.second] = p.first;
        }
    }

    

} // namespace Input