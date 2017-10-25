local keyboard = {
    requestRedraw = false,
    x = 0,
    y = 0
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

    cr.cairo_move_to(ctx, self.x, self.y);
    cr.cairo_show_text(ctx, "KB2");  
end

function keyboard:needsRedraw()
    if self.requestRedraw then
	self.requestRedraw = false
	return true
    end
    return false
end

function keyboard:shouldConsumeKeyEvent(e)
    if e.key == self.options.up or
	e.key == self.options.down or
	e.key == self.options.left or
	e.key == self.options.right
    then
	return true
    end
    return false
end
function keyboard:onKeyPress(e)
    if e.key == self.options.up then
	self.y = self.y - 10
	self.requestRedraw = true
    end
    if e.key == self.options.down then
	self.y = self.y + 10
	self.requestRedraw = true
    end
    if e.key == self.options.left then
	self.x = self.x - 10
	self.requestRedraw = true
    end
    if e.key == self.options.right then
	self.x = self.x + 10
	self.requestRedraw = true
    end
end
function keyboard:onKeyRelease(e)
end

function keyboard:new(o, options)
    o = o or {}
    setmetatable(o, self)
    self.__index = self
    
    self.options = options
    self.x = 0
    self.y = 0
    
    return o 
end

return keyboard
