-- Default options. This file gets loaded for every program that doesn't define custom options.
local x = require "ffi.x11"

options = {
    name = "Default Options",
    settingsKey = x.XK_Tab,
    keyboardKey = x.XK_F1,

    up = x.XK_Up,
    down = x.XK_Down,
    left = x.XK_Left,
    right = x.XK_Right,
}

return options
