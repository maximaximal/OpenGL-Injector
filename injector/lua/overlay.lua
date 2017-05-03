-- Set the correct path.
package.path = package.path .. ";" .. PATH .. "/lua/?.lua"

local cr = require "ffi.cairo"

function needsRedraw()
    return true
end

function draw()
    cr.cairo_set_source_rgba(ctx, 1, 1, 1, 1)
    cr.cairo_set_line_width(ctx, 0.1)
    cr.cairo_move_to(ctx, 0, 0)
    cr.cairo_line_to(ctx, 1, 1)
    
    cr.cairo_stroke(ctx)
end
