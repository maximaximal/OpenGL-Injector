local ffi = require( "ffi" )

-- Normal bindings
local overlay = require "ffi.overlay"

-- Kbd table
local kbd = {}

function kbd.keyboardIterator(keyboards)
    local current = 0

    return function()
	if current == 0 then
	    -- The first element.
	    current = keyboards.keyboards
	else
	    -- Elements after the current one.
	    current = current.next
	end

	-- Only if there is a keyboard - the iterator is over in any other case.
	if current ~= nil then
	    return current
	end
    end
end

return kbd
