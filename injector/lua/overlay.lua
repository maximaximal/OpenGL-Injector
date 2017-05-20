-- Set the correct path.
package.path = package.path .. ";" .. PATH .. "/lua/?.lua"

local cr = require "ffi.cairo"
local overlay = require "ffi.overlay"
local Keyboard = require "keyboard"
local ffi = require "ffi"

-- Specify the default options first, but try to load program specific options.
local optionsScript = PATH .. "/lua/options.lua"
if OPTIONS_SCRIPT ~= "" then
    print("Using program specific options file " .. OPTIONS_SCRIPT)
    optionsScript = OPTIONS_SCRIPT
end
local options = dofile(optionsScript)

print("Using options \"" .. options.name .. "\"")

-- Global overlay objects
local keyboard = Keyboard.init();

-- Global overlay state
local keyboardOpen = false
local requestRedraw = true

-- Conversion functions.
local toKeyEv = ffi.typeof("piga_key_event_t *")

local x = 0
local y = 0

function onKeyPress(e)
    e = toKeyEv(e)

    if e.key == options.settingsKey then
    end
    if e.key == options.keyboardKey then
	keyboardOpen = not keyboardOpen
    end
    if e.key == options.up then
	y = y - 10
    end
    if e.key == options.down then
	y = y + 10
    end
    if e.key == options.left then
	x = x - 10
    end
    if e.key == options.right then
	x = x + 10
    end
end
function onKeyRelease(e)
    e = toKeyEv(e)

    if e.key == options.settingsKey then
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
    if e.key == options.settingsKey then
	return true
    end
    if e.key == options.keyboardKey then
	return true
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
    return requestRedraw
end

function draw()
    -- Test the overlay functions.
    layouts = overlay.piga_xkb_get_available_layouts()

    for i = 1,tonumber(layouts.size) - 1,1 do
	--print("Layout " .. ffi.string(layouts.layouts[i]))
    end
    
    overlay.piga_xkb_free_layouts(layouts)

    if keyboardOpen then
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

    end

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

  --  cr.cairo_rectangle (ctx, 4, 0, 0.5, 0.5);
  --  cr.cairo_set_source_rgb (ctx, 5, 0, 1);
   -- cr.cairo_fill (ctx);
   -- cr.cairo_move_to(ctx, 0, 0)
    --cr.cairo_line_to(ctx, 0, HEIGHT)
    --cr.cairo_stroke(ctx)

   --circle...
     cr.cairo_set_line_width(ctx, 9)  
     cr.cairo_set_source_rgb(ctx, 0.69, 0.19, 0)
     cr.cairo_set_line_width(ctx, 9)  
     cr.cairo_set_source_rgb(ctx, 0.69, 0.19, 0)
     cr.cairo_move_to(ctx, WIDTH / 2, WIDTH / 2)
     cr.cairo_arc(ctx, 0, 0, 50, 0, 2 * 3.14)
     cr.cairo_move_to(ctx, WIDTH, 0)
     cr.cairo_line_to(ctx, 0, HEIGHT)
     cr.cairo_stroke_preserve(ctx);

    -- ...another circle...
    linpat = cr.cairo_pattern_create_linear (0, 0, 1, 1)
    cr.cairo_pattern_add_color_stop_rgb (linpat, 0, 0, 0.3, 0.8)
    cr.cairo_pattern_add_color_stop_rgb (linpat, 1, 0, 0.8, 0.3)

    radpat = cr.cairo_pattern_create_radial (0.5, 0.5, 0.25, 0.5, 0.5, 0.75)
    cr.cairo_pattern_add_color_stop_rgba (radpat, 0, 0, 0, 0, 1)
    cr.cairo_pattern_add_color_stop_rgba (radpat, 0.5, 0, 0, 0, 0)

    cr.cairo_set_source (ctx, linpat);
    cr.cairo_mask (ctx, radpat);

    

    -- cr.cairo_set_source_rgb(ctx, 0.1, 0.1, 0.1);  



end
