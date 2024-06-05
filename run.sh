#!/bin/sh

g++ -o snake main.cc game.cc snake.cc stage.cc --std=c++17 -lncurses
./snake
