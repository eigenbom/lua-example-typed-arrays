package = "carray"
version = "dev-1"
source = {
   url = "."
}
description = {
   summary = "lua typed arrays",
   license = "MIT"
}
dependencies = {
   "lua >= 5.1"
}
build = {
   type = "builtin",
   modules = {
      carray = "src/carray.c",
      types = "src/types.lua",
   }
}
