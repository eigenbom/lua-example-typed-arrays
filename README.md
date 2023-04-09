# Lua Typed Arrays

This repository demonstrates how to implement resizable typed arrays for Lua. This code exists for testing and education purposes only and shouldn't be used in production. It is a light wrapper around [stb_ds.h](https://nothings.org/stb_ds/) with some additional Lua for type annotations, etc.

## Usage

```lua
local carray = require("carray")

-- Create a new array of unsigned 8-bit integers with size 4
local arr = carray.new_u8(4)
assert(#arr == 4)
print(type(arr)) --> "carray<u8>: 0x013964632950"
arr[1] = 4 -- assign the first element
arr[10] = 42 -- assign the tenth element and automatically resize
```

See [spec/carray_spec.lua](spec/carray_spec.lua) for more examples.

## Installation

You can use [luarocks](https://luarocks.org) build system, e.g., download this repo and then `luarocks make`.

Alternatively manually compile [carray.c](src/carray.c) and [stb_ds.h](src/stb_ds.h). Optionally include [types.lua](src/types.lua) for type annotations.