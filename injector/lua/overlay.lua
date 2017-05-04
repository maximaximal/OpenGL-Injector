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
    cr.cairo_move_to(ctx, WIDTH / 2, 0)
    cr.cairo_line_to(ctx, 0, HEIGHT / 2)
    cr.cairo_stroke(ctx)

    cr.cairo_set_source_rgba(ctx, 1, 1, 1, 0.5)
    cr.cairo_select_font_face(ctx, "Inconsolata",
    cr.CAIRO_FONT_SLANT_NORMAL,
    cr.CAIRO_FONT_WEIGHT_BOLD);

    cr.cairo_set_font_size(ctx, HEIGHT / 2);

    cr.cairo_move_to(ctx, 0, HEIGHT / 2);
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

    --cr.cairo_rectangle (ctx, 4, 0, 0.5, 0.5);
    --cr.cairo_set_source_rgb (ctx, 5, 0, 1);
    --cr.cairo_fill (ctx);
    --cr.cairo_move_to(ctx, 0, 0)
    --cr.cairo_line_to(ctx, WIDTH, 0)
    --cr.cairo_stroke(ctx)

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
