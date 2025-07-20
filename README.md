# Multi-Level Text Compression Pipeline

A high-performance text compression system implementing a 4-stage compression pipeline: Burrows-Wheeler Transform (BWT) + Move-To-Front (MTF) + Run-Length Encoding (RLE) + Huffman Coding. Features a modern web interface with real-time compression statistics and file upload capabilities.

## Architecture

### Compression Pipeline
```
Input Text → BWT → MTF → RLE → Huffman → Compressed Binary
```

### Decompression Pipeline
```
Compressed Binary → Huffman Decode → RLE Decode → MTF Decode → BWT Decode → Original Text
```

## Tech Stack

### **Backend**
- **Runtime**: Node.js 18+
- **Framework**: Express.js 4.18+
- **File Upload**: Multer 1.4+
- **CORS**: Cross-Origin Resource Sharing enabled
- **Process Management**: Child process spawning for C++ executables

### **Frontend**
- **Framework**: React 18+ with Vite 4+
- **Styling**: Tailwind CSS 4+ with custom animations
- **HTTP Client**: Axios 1.4+
- **Build Tool**: Vite with PostCSS and Autoprefixer
- **UI Features**: Responsive design, dark theme, glassmorphism effects

### **C++ Core**
- **Standard**: C++11
- **Dependencies**: Standard library only (no external dependencies)
- **Compilation**: GCC/Clang compatible
- **Platform**: Cross-platform (Windows/Linux/macOS)

## Technical Implementation

### 1. Burrows-Wheeler Transform (BWT)
- **Purpose**: Reorders characters to group similar patterns together
- **Implementation**: `algorithms/BWT.cpp` / `algorithms/BWT.h`
- **Output**: Transformed string + primary index for reconstruction
- **Time Complexity**: O(n² log n) for suffix array construction
- **Space Complexity**: O(n²)
- **Algorithm**: Suffix array construction with lexicographic sorting

### 2. Move-To-Front (MTF)
- **Purpose**: Converts repeated characters to small integers
- **Implementation**: `algorithms/MTF.cpp` / `algorithms/MTF.h`
- **Algorithm**: Maintains 256-symbol table, moves accessed symbols to front
- **Output**: Integer sequence with high frequency of small values
- **Time Complexity**: O(n) per symbol access
- **Optimization**: Efficient symbol table management

### 3. Run-Length Encoding (RLE)
- **Purpose**: Compresses consecutive identical values
- **Implementation**: `algorithms/RLE.cpp` / `algorithms/RLE.h`
- **Format**: `(count, value)` pairs for runs > 1
- **Output**: Compressed run-length encoded sequence
- **Time Complexity**: O(n)
- **Efficiency**: Optimized for binary data compression

### 4. Huffman Coding
- **Purpose**: Variable-length encoding based on frequency
- **Implementation**: `algorithms/Huffman.cpp` / `algorithms/Huffman.h`
- **Algorithm**: Builds optimal prefix code tree using priority queue
- **Output**: Binary compressed data + Huffman tree
- **Time Complexity**: O(n log n) for tree construction
- **Memory**: Efficient tree representation

## File Structure

```
Huffman/
├── algorithms/                    
│   ├── Compressor.cpp            
│   ├── compressor_cli.cpp        
│   ├── decompressor_cli.cpp      
│   ├── BWT.cpp/BWT.h            
│   ├── MTF.cpp/MTF.h            
│   ├── RLE.cpp/RLE.h            
│   ├── Huffman.cpp/Huffman.h    
│   ├── test_*.cpp               
│   ├── *.exe                   
│   └── text_input.txt           
├── backend/                     
│   ├── server.js                
│   ├── compressor_cli.exe       
│   └── package.json             
├── frontend/                     
│   ├── src/
│   │   ├── components/
│   │   │   └── Compressor.jsx   
│   │   └── index.css            
│   ├── tailwind.config.js       
│   └── package.json             
├── README.md                     
└── .gitignore                   
```

## Web Application Features

### **User Interface**
- **Real-time Compression**: Instant compression with live statistics
- **File Upload**: Support for .txt, .json, .md files up to 10MB
- **Visual Feedback**: File upload status with file details
- **Compression History**: Track recent compressions with timestamps
- **Export Functionality**: Download compressed results as JSON
- **Responsive Design**: Works on desktop, tablet, and mobile

### **Compression Statistics**
- **Original Size**: Character count of input text
- **Compressed Size**: Length of binary output
- **Compression Ratio**: Percentage of space saved
- **Processing Time**: Real-time performance metrics
- **Primary Index**: BWT reconstruction parameter

## API Endpoints

### POST /compress
- **Input**: `{ "text": "string" }`
- **Output**: `{ "compressed": "binary", "primaryIndex": number, "timestamp": "ISO", "originalSize": number, "compressedSize": number }`
- **Process**: Runs `algorithms/compressor_cli.exe` with input text
- **Performance**: Real-time compression with detailed metrics

### POST /decompress
- **Input**: `{ "compressed": "binary", "primaryIndex": number }`
- **Output**: `{ "decompressed": "string", "timestamp": "ISO" }`
- **Process**: Runs `algorithms/decompressor_cli.exe` with compressed data
- **Verification**: Ensures perfect reconstruction of original text

### POST /upload
- **Input**: Multipart form data with file
- **Output**: Same as compress endpoint + filename
- **Process**: Reads file content and compresses
- **File Support**: UTF-8 encoded text files

## Development Setup

### C++ Core Compilation
```bash
cd algorithms

# Compile compression executable
g++ -o compressor_cli.exe compressor_cli.cpp

# Compile decompression executable  
g++ -o decompressor_cli.exe decompressor_cli.cpp

# Compile test files (optional)
g++ -o test_bwt.exe test_bwt.cpp
g++ -o test_mtf.exe test_mtf.cpp
g++ -o test_rle.exe test_rle.cpp
```

### Backend Development
```bash
cd backend
npm install
npm run dev  # Development mode
# or
npm start    # Production mode
```

### Frontend Development
```bash
cd frontend
npm install
npm run dev  # Development server (http://localhost:5173)
npm run build  # Production build
npm run preview  # Preview production build
```

## Performance Characteristics

### Compression Efficiency
- **Best case**: Repetitive text with long runs (70-80% compression)
- **Worst case**: Random data with high entropy (may expand by 10-20%)
- **Typical ratio**: 30-60% compression for natural language text
- **Overhead**: ~50-100 bytes for algorithm metadata (BWT index, Huffman tree)

### Time Complexity
- **Compression**: O(n² log n) dominated by BWT suffix array construction
- **Decompression**: O(n log n) dominated by Huffman tree traversal
- **Memory usage**: O(n²) for BWT suffix array, O(n) for other algorithms

### Scalability & Limits
- **Input size limit**: ~1MB practical limit due to BWT memory usage
- **Concurrent requests**: Limited by available RAM (recommended: 2GB+)
- **File upload limit**: 10MB (configurable in multer)
- **Real-time performance**: <100ms for typical text inputs (<10KB)

### Memory Usage
- **BWT**: O(n²) for suffix array
- **MTF**: O(1) per symbol (256-symbol table)
- **RLE**: O(n) for run encoding
- **Huffman**: O(n) for frequency table and tree

## Technical Dependencies

### Backend
- **Node.js**
- **Express.js**
- **Multer**
- **CORS**

### Frontend
- **React**: 18+ 
- **Vite**: 4+ 
- **Tailwind CSS**: 4+ 
- **Axios**: 1.4+ 
- **PostCSS**: 
- **Autoprefixer**: 10+ 

### C++ Core
- **Compiler**: GCC 7+ or Clang 6+ or MSVC 2017+
- **Standard**: C++11 (minimum)
- **Dependencies**: Standard library only
- **Platform**: Cross-platform (Windows/Linux/macOS)
- **Build System**: Manual compilation or Makefile

## Data Formats & Communication

### Compression API Response
```json
{
  "compressed": "10101010001101010100001011000010100011000000001111011",
  "primaryIndex": 42,
  "originalSize": 156,
  "compressedSize": 47,
  "timestamp": "2024-01-15T10:30:00.000Z"
}
```

### Decompression API Response
```json
{
  "decompressed": "Original text content here...",
  "timestamp": "2024-01-15T10:30:00.000Z"
}
```

### File Upload
- **Supported formats**: .txt, .json, .md
- **Encoding**: UTF-8
- **Max size**: 10MB (configurable)
- **Processing**: In-memory buffer
- **Validation**: File type and size checks

### C++ CLI Communication
- **Input**: JSON via stdin
- **Output**: JSON via stdout
- **Error handling**: Non-zero exit codes for failures
- **Format**: UTF-8 encoded JSON strings

