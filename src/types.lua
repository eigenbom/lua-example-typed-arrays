---@meta

---@module 'carray'
local carray

---@class carray.u8: number[]
---@class carray.i8: number[]
---@class carray.u16: number[]
---@class carray.i16: number[]
---@class carray.u32: number[]
---@class carray.i32: number[]
---@class carray.f32: number[]
---@class carray.f64: number[]

---@param length? integer
---@return carray.u8
function carray.new_u8(length) end

---@param length? integer
---@return carray.i8
function carray.new_i8(length) end

---@param length? integer
---@return carray.u16
function carray.new_u16(length) end

---@param length? integer
---@return carray.i16
function carray.new_i16(length) end

---@param length? integer
---@return carray.u32
function carray.new_u32(length) end

---@param length? integer
---@return carray.i32
function carray.new_i32(length) end

---@param length? integer
---@return carray.f32
function carray.new_f32(length) end

---@param length? integer
---@return carray.f64
function carray.new_f64(length) end

