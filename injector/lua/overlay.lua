-- Set the correct path.
package.path = package.path .. ";" .. PATH .. "/lua/?.lua"

local cr = require "ffi.cairo"
local overlay = require "ffi.overlay"
local kbd = require "ffi.kbd"
local ffi = require "ffi"

local Keyboard = require "keyboard"
local Settings = require "settings"

-- Specify the default options first, but try to load program specific options.
local optionsScript = PATH .. "/lua/options.lua"
if OPTIONS_SCRIPT ~= "" then
    print("Using program specific options file " .. OPTIONS_SCRIPT)
    optionsScript = OPTIONS_SCRIPT
end
local options = dofile(optionsScript)

print("Using options \"" .. options.name .. "\"")

-- Global overlay objects
local keyboard = Keyboard:new({}, options)
local settings = Settings:new({}, options)

-- Global overlay state
local requestRedraw = true

-- Conversion functions.
local toKeyEv = ffi.typeof("piga_key_event_t *")

local drawSettings = false
local drawKeyboard = false

function onKeyPress(e)
    e = toKeyEv(e)

    if e.key == options.settingsKey then
	-- Activate/Deactivate Settings screen.
	drawSettings = not drawSettings
	requestRedraw = true
    end
    if e.key == options.keyboardKey then
	drawKeyboard = not drawKeyboard
	requestRedraw = true
    end

    -- Handle global objects.
    if drawSettings then
	settings:onKeyPress(e)
    end
    if drawKeyboard then
	keyboard:onKeyPress(e)
    end
end
function onKeyRelease(e)
    e = toKeyEv(e)

    -- Handle global objects.
    if drawSettings then
	settings:onKeyRelease(e)
    end
    if drawKeyboard then
	keyboard:onKeyRelease(e)
    end
end
function onMotionNotify(e)
end
function onButtonPress(e)
end
function onWindowEvent(e)
end

function shouldConsumeKeyEvent(e)
    e = toKeyEv(e)

    -- Global key events.
    if e.key == options.settingsKey then
	return true
    end
    if e.key == options.keyboardKey then
	return true
    end

    if drawSettings then
	if settings:shouldConsumeKeyEvent(e) then
	    return true
	end
    end
    if drawKeyboard then
	if keyboard:shouldConsumeKeyEvent(e) then
	    return true
	end
    end
    
    return false
end
function shouldConsumeMotionEvent(e)
    return false
end
function shouldConsumeButtonEvent(e)
    return false
end
function shouldConsumeWindowEvent(e)
    return false
end

function needsRedraw()
    if settings:needsRedraw() or keyboard:needsRedraw() then
	return true
    end
    if requestRedraw then
	requestRedraw = false
	return true
    end
    return false
end

function init()

end

function draw()
    -- Test the overlay functions.

    --for keyboard in kbd.keyboardIterator(keyboards) do
	-- Iterate over all keyboards.
	-- print("Keyboard: " .. ffi.string(keyboard.name) .. " in \"" .. ffi.string(keyboard.filename))
    --end

    if drawSettings then
	settings:draw()
    end
    
    if drawKeyboard then
	keyboard:draw()
    end
    
    --[[
    cr.cairo_set_line_width(ctx, 20)
    cr.cairo_set_source_rgba(ctx, 1, 0, 0, 0.5)
    cr.cairo_move_to(ctx, 0, 0)
    cr.cairo_line_to(ctx, WIDTH, HEIGHT)
    cr.cairo_stroke(ctx)

    cr.cairo_set_line_width(ctx, 5)
    cr.cairo_set_source_rgba(ctx, 1, 1, 0, 0.5)
    cr.cairo_move_to(ctx, WIDTH / 2, 0)
    cr.cairo_line_to(ctx, 0, HEIGHT / 2)
    cr.cairo_stroke(ctx)

    cr.cairo_set_source_rgba(ctx, 1, 1, 1, 0.5)
    cr.cairo_select_font_face(ctx, "Inconsolata",
    cr.CAIRO_FONT_SLANT_NORMAL,
    cr.CAIRO_FONT_WEIGHT_BOLD);

    cr.cairo_set_font_size(ctx, HEIGHT / 2);

    cr.cairo_move_to(ctx, x, y);
    cr.cairo_show_text(ctx, "Test");  

    cr.cairo_rectangle (ctx, 4, 0, 0.5, 0.5);
    cr.cairo_set_source_rgb (ctx, 5, 0, 1);
    cr.cairo_fill (ctx);
    cr.cairo_move_to(ctx, WIDTH, HEIGHT)
    cr.cairo_line_to(ctx, WIDTH, 0)
    cr.cairo_stroke(ctx)

    cr.cairo_rectangle (ctx, 4, 0, 0.5, 0.5);
    cr.cairo_set_source_rgb (ctx, 5, 0, 1);
    cr.cairo_fill (ctx);
    cr.cairo_move_to(ctx, WIDTH, HEIGHT)
    cr.cairo_line_to(ctx, 0, HEIGHT)
    cr.cairo_stroke(ctx)

    cr.cairo_rectangle (ctx, 10, 100, 20, 20);
    cr.cairo_set_source_rgb (ctx, 5, 0, 1);
    cr.cairo_fill (ctx);
    cr.cairo_move_to(ctx, 0, 0)
    cr.cairo_line_to(ctx, WIDTH, 0)
    cr.cairo_stroke(ctx)
    ]]--

  --  cr.cairo_rectangle (ctx, 4, 0, 0.5, 0.5);
  --  cr.cairo_set_source_rgb (ctx, 5, 0, 1);
   -- cr.cairo_fill (ctx);
   -- cr.cairo_move_to(ctx, 0, 0)
    --cr.cairo_line_to(ctx, 0, HEIGHT)
    --cr.cairo_stroke(ctx)
    -- cr.cairo_set_source_rgb(ctx, 0.1, 0.1, 0.1);  



end
