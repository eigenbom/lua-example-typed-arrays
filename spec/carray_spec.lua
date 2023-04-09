---@module 'types'

local carray = require("carray")

describe("carray functionality", function()
	it("creates userdata", function()
		assert.are.equal(type(carray.new_u8(16)), "userdata")
		assert.are.equal(type(carray.new_i8(0)), "userdata")
		assert.are.equal(type(carray.new_f32(128)), "userdata")
		assert.are.equal(type(carray.new_f64(64)), "userdata")
	end)

	it("assigns unique metatable per array type", function()
		assert.are.equal(getmetatable(carray.new_u8(16)), getmetatable(carray.new_u8(0)))
		assert.are.not_equal(getmetatable(carray.new_u8(16)), getmetatable(carray.new_f32(0)))
	end)

	it("can set elements", function()
		local arr = carray.new_u8(4)
		arr[1] = 4
		arr[10] = 42
		assert.has_error(function() arr[0] = 4 end, "bad argument #2 to '__newindex' (index < 1)")

		local farr = carray.new_f32(4)
		farr[1] = 4.1
		farr[10] = 42
		assert.has_error(function() farr[0] = 4 end, "bad argument #2 to '__newindex' (index < 1)")
	end)

	it("can get elements", function()

		local arr = carray.new_u8(4)
		arr[1] = 4
		arr[10] = 42
		assert.are.equal(arr[1], 4)
		assert.are.equal(arr[10], 42)
		assert.has_error(function() print(arr[0]) end, "bad argument #2 to '__index' (index < 1)")

		local farr = carray.new_f32(4)
		farr[1] = 4.1
	end )

	it("can get length", function()
		local arr = carray.new_u8(4)
		arr[1] = 4
		assert.are.equal(#arr, 4)
		arr[10] = 42
		assert.are.equal(#arr, 10)
	end )

	it("has tostring", function()
		assert.are.equal(tostring(carray.new_u8(4)):gmatch("(.*):")(), "carray<u8>")
		assert.are.equal(tostring(carray.new_f32(4)):gmatch("(.*):")(), "carray<f32>")
		assert.are.equal(tostring(carray.new_f64(4)):gmatch("(.*):")(), "carray<f64>")
	end)
end)

describe("carray is array-like", function()
	it("acts like the array part of a table", function()
		local arr = carray.new_u8(0)
		for i=1,10 do
			arr[i] = i
		end
		local contents = {}
		for i=1, #arr do
			contents[i] = arr[i]
		end
		assert.are.equal(table.concat(contents, ","), "1,2,3,4,5,6,7,8,9,10")
	end)
end )