local cr = require "ffi.cairo"
local overlay = require "ffi.overlay"

local settings = {
    requestRedraw = false,
    keyboards = overlay.piga_xkb_get_keyboards()
}

function settings:draw()
    cr.cairo_set_source_rgba(ctx, 1, 1, 1, 0.5)
    cr.cairo_select_font_face(ctx, "Inconsolata",
    cr.CAIRO_FONT_SLANT_NORMAL,
    cr.CAIRO_FONT_WEIGHT_BOLD);

    cr.cairo_set_font_size(ctx, HEIGHT / 2);

    cr.cairo_move_to(ctx, 0, HEIGHT / 2);
    cr.cairo_show_text(ctx, "Settings");  
end

function settings:shouldConsumeKeyEvent(e)
    if e.key == self.options.up or
	e.key == self.options.down or
	e.key == self.options.left or
	e.key == self.options.right
    then
	return true
    end
    return false
end

function settings:onKeyPress(e)
    return false
end
function settings:onKeyRelease(e)
    return false
end

function settings:needsRedraw()
    if self.requestRedraw then
	self.requestRedraw = false
	return true
    end
    return false
end

function settings:new(o, options)
    o = o or {}
    setmetatable(o, self)
    self.__index = self 

    self.options = options
    
    return o
end

return settings
