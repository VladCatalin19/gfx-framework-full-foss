# GFX Framework Full Foss

The original framework does not have all components open source. Camera
and Transform come precompiled in dynamically linked libraries. For this
project, both classes have been reversed engineered and rewritten using
[Ghidra](https://github.com/NationalSecurityAgency/ghidra) and
[VSCodium](https://github.com/VSCodium/vscodium).

The purpose of this code is to be used as reference, but it can also be used to
build the framework. Put the cpp's in _gfx-framework/src/components_,
remove GFXC_API from Camera and Transform declarations, (re)run cmake and build.
