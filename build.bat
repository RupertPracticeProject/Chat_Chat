@echo off
if exist run.exe ( del run.exe )
g++ -o run.exe server.cpp -lws2_32
if exist run.exe ( run.exe )