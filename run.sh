#!/bin/sh

g++ -o snake main.cc game.cc snake.cc stage.cc menu.cc gameplay.cc --std=c++17 -lncurses
./snake
