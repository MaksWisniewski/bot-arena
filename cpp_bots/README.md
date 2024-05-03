# How to use bots written in C++

## Install dependencies
Bots use [`json`](https://github.com/nlohmann/json) library for JSON parsing.
You can (probably) install it with your package manager:

- dnf (Fedora): `sudo dnf install json-devel`
- apt (Debian): `sudo apt install nlohmann-json3-dev`
- if you use other distro or package manager please find out how to install it and write it down here :)

## Compile bot
To compile bot `bot_name` use command: `make bot_name`. You can run `make all` to compile all bots.
If you add new bot, please add proper target to `Makefile`.

## Run bot
Compiled bots should by default be created in `bots` directory, if not you should move them there. Then the bots can be run in the same way as python bots.
