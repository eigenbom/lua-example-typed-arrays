local carray = require 'carray'

if jit then
	jit.off()
end

-- print memory usage in MB
local function print_memory_usage(label)
	collectgarbage('count')
	local mem = collectgarbage('count')
	print(string.format('Memory usage (%s): %.2f MB', label, mem / 1024))
end

local function size_alloc_test()
	local N = 1000000

	print_memory_usage("start")

	_G._table = {[1] = 0, [N] = 99}
	for i=1,N do
		_G._table[i] = 0
	end
	print_memory_usage("after new table")

	_G._u8_array = carray.new_u8(N)
	_G._u8_array[N] = 42
	print_memory_usage("after new_u8")

	_G._u32_array = carray.new_u32(N)
	_G._u32_array[N] = 42
	print_memory_usage("after new_u32")

	_G._f64_array = carray.new_f64(N)
	_G._f64_array[N] = 42
	print_memory_usage("after new_f64")

	_G._f64_array[N+1] = 42
	print_memory_usage("after growing f64")

	_G._table2 = {[1] = 0, [N] = 99}
	for i=1,N do
		_G._table2[i] = 0
	end
	print_memory_usage("after another new table")

end

local function leak_test()
	print_memory_usage("before")
	do
		local N = 1000000
		local arr = carray.new_u8(N)
		-- print(next(getmetatable(arr)))
		print_memory_usage("after")
	end

	collectgarbage("collect")
	collectgarbage("collect")
	print_memory_usage("after gc")
end

size_alloc_test()
-- leak_test()


