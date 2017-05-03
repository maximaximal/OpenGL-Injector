-- Set the correct path.
package.path = package.path .. ";" .. PATH .. "/lua/?.lua"

local cr = require "ffi.cairo"

function needsRedraw()
    return true
end

function draw()
    cr.cairo_set_line_width(ctx, 20)
    cr.cairo_set_source_rgba(ctx, 1, 0, 0, 0.5)
    cr.cairo_move_to(ctx, 0, 0)
    cr.cairo_line_to(ctx, WIDTH, HEIGHT)
    cr.cairo_stroke(ctx)

    cr.cairo_set_line_width(ctx, 5)
    cr.cairo_set_source_rgba(ctx, 1, 1, 0, 0.5)
    cr.cairo_move_to(ctx, WIDTH, 0)
    cr.cairo_line_to(ctx, 0, HEIGHT)
    cr.cairo_stroke(ctx)
end
