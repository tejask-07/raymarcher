#!/bin/bash
g++ *.cpp -o raymarcher -std=c++17 -I/opt/homebrew/include -L/opt/homebrew/lib -lraylib -framework OpenGL -lm
./raymarcher