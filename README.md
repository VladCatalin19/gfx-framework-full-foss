# GFX Framework Full Foss

The original framework does not have all components open source. Camera
and Transform come precompiled in dynamically linked libraries. For this
project, both classes have been reversed engineered and rewritten using
[Ghidra](https://github.com/NationalSecurityAgency/ghidra) and
[VSCodium](https://github.com/VSCodium/vscodium).

Note that this code should be used only for reference. It can be used to
build the actual framework. Simply put the cpp's in _gfx-framework/src/components_,
rerun the cmake command if you already built the project and you're done.
No need to change CMakeLists.txt. It will prioritise the classes in
the project over the classes from the precompiled libraries.
