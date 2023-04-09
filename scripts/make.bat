echo Running luarocks make in x64 Native Tools Command Prompt for VS 2019
@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
luarocks.bat make

