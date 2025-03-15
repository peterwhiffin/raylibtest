@echo off
mkdir ..\..\raylibGameBuild
pushd ..\..\raylibGameBuild
cl gdi32.lib msvcrt.lib raylib.lib winmm.lib user32.lib shell32.lib ..\raylibtest\code\VoxelTest.c -IX:\Raylib\raylib-5.5_win64_msvc16\include /link /libpath:X:\Raylib\raylib-5.5_win64_msvc16\lib /NODEFAULTLIB:libcmt
popd
