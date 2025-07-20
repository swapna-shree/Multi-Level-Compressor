#!/bin/bash

# Build script for Render deployment
echo "Building C++ executables..."

# Navigate to algorithms directory
cd algorithms

# Compile compressor_cli
echo "Compiling compressor_cli..."
g++ -std=c++17 -o compressor_cli compressor_cli.cpp Compressor.cpp BWT.cpp MTF.cpp RLE.cpp Huffman.cpp

# Compile decompressor_cli  
echo "Compiling decompressor_cli..."
g++ -std=c++17 -o decompressor_cli decompressor_cli.cpp Compressor.cpp BWT.cpp MTF.cpp RLE.cpp Huffman.cpp

# Copy executables to backend directory
echo "Copying executables to backend..."
cp compressor_cli ../backend/
cp decompressor_cli ../backend/

echo "Build completed successfully!" 