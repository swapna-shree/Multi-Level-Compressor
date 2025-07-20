@echo off
echo Building C++ executables...

REM Compile compressor_cli
echo Compiling compressor_cli...
g++ -std=c++17 -o compressor_cli.exe ../algorithms/compressor_cli.cpp ../algorithms/Compressor.cpp ../algorithms/BWT.cpp ../algorithms/MTF.cpp ../algorithms/RLE.cpp ../algorithms/Huffman.cpp

REM Compile decompressor_cli  
echo Compiling decompressor_cli...
g++ -std=c++17 -o decompressor_cli.exe ../algorithms/decompressor_cli.cpp ../algorithms/Compressor.cpp ../algorithms/BWT.cpp ../algorithms/MTF.cpp ../algorithms/RLE.cpp ../algorithms/Huffman.cpp

echo Build completed successfully! 