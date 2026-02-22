set shell := ["cmd.exe", "/c"]
set quiet

default:
    just --summary --unsorted

init:
    cmake -G Ninja -S . -B .\out

clean:
    cmake --build .\out --target clean

build:
    ninja -C .\out
