local keyboard = {
    requestRedraw = false,
}

local cr = require "ffi.cairo"

local function generateKeys()
    local keys = {
	
    }
    return keys
end

function keyboard:draw()
    cr.cairo_set_source_rgba(ctx, 1, 1, 1, 0.5)
    cr.cairo_select_font_face(ctx, "Inconsolata",
    cr.CAIRO_FONT_SLANT_NORMAL,
    cr.CAIRO_FONT_WEIGHT_BOLD);

    cr.cairo_set_font_size(ctx, HEIGHT / 2);

    cr.cairo_move_to(ctx, 0, HEIGHT / 2);
    cr.cairo_show_text(ctx, "Keyboard");  
end

function keyboard:needsRedraw()
    if self.requestRedraw then
	self.requestRedraw = false
	return true
    end
    return false
end

function keyboard:shouldConsumeKeyEvent(e)
    return false
end
function keyboard:onKeyPress(e)
    return false
end
function keyboard:onKeyRelease(e)
    return false
end

function keyboard:new(o)
    o = o or {}
    setmetatable(o, self)
    self.__index = self
    return o 
end

return keyboard
