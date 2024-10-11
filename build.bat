@echo off
if exist server.exe ( del server.exe )
g++ -o server.exe server.cpp -lws2_32
if exist client.exe ( del client.exe)
g++ -o client.exe client.cpp -lws2_32