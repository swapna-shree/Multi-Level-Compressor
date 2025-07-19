# Multi-Level Text Compression Pipeline

A high-performance text compression system implementing a 4-stage compression pipeline: Burrows-Wheeler Transform (BWT) + Move-To-Front (MTF) + Run-Length Encoding (RLE) + Huffman Coding.

## Architecture

### Compression Pipeline
```
Input Text → BWT → MTF → RLE → Huffman → Compressed Binary
```

### Decompression Pipeline
```
Compressed Binary → Huffman Decode → RLE Decode → MTF Decode → BWT Decode → Original Text
```

## Technical Implementation

### 1. Burrows-Wheeler Transform (BWT)
- **Purpose**: Reorders characters to group similar patterns together
- **Implementation**: `BWT.cpp` / `BWT.h`
- **Output**: Transformed string + primary index for reconstruction
- **Time Complexity**: O(n² log n) for suffix array construction
- **Space Complexity**: O(n²)

### 2. Move-To-Front (MTF)
- **Purpose**: Converts repeated characters to small integers
- **Implementation**: `MTF.cpp` / `MTF.h`
- **Algorithm**: Maintains 256-symbol table, moves accessed symbols to front
- **Output**: Integer sequence with high frequency of small values
- **Time Complexity**: O(n) per symbol access

### 3. Run-Length Encoding (RLE)
- **Purpose**: Compresses consecutive identical values
- **Implementation**: `RLE.cpp` / `RLE.h`
- **Format**: `(count, value)` pairs for runs > 1
- **Output**: Compressed run-length encoded sequence
- **Time Complexity**: O(n)

### 4. Huffman Coding
- **Purpose**: Variable-length encoding based on frequency
- **Implementation**: `Huffman.cpp` / `Huffman.h`
- **Algorithm**: Builds optimal prefix code tree
- **Output**: Binary compressed data + Huffman tree
- **Time Complexity**: O(n log n) for tree construction

## File Structure

```
Huffman/
├── Compressor.cpp          # Main compression pipeline class
├── compressor_cli.cpp      # CLI compression executable
├── decompressor_cli.cpp    # CLI decompression executable
├── BWT.cpp/BWT.h          # Burrows-Wheeler Transform
├── MTF.cpp/MTF.h          # Move-To-Front encoding
├── RLE.cpp/RLE.h          # Run-Length Encoding
├── Huffman.cpp/Huffman.h  # Huffman coding
├── backend/
│   ├── server.js          # Node.js Express API server
│   ├── compressor_cli.exe # Compiled compression executable
│   └── package.json       # Backend dependencies
└── frontend/
    ├── src/
    │   ├── components/
    │   │   └── Compressor.jsx # React UI component
    │   └── index.css          # Tailwind CSS styles
    ├── tailwind.config.js     # Tailwind configuration
    └── package.json           # Frontend dependencies
```

## API Endpoints

### POST /api/compress
- **Input**: `{ "text": "string" }`
- **Output**: `{ "compressed": "binary", "primaryIndex": number, "timestamp": "ISO", "originalSize": number, "compressedSize": number }`
- **Process**: Runs `compressor_cli.exe` with input text

### POST /api/decompress
- **Input**: `{ "compressed": "binary", "primaryIndex": number }`
- **Output**: `{ "decompressed": "string", "timestamp": "ISO" }`
- **Process**: Runs `decompressor_cli.exe` with compressed data

### POST /api/upload
- **Input**: Multipart form data with file
- **Output**: Same as compress endpoint + filename
- **Process**: Reads file content and compresses

## Compilation

### C++ Executables
```bash
# Compress executable
g++ -o compressor_cli.exe compressor_cli.cpp

# Decompress executable  
g++ -o decompressor_cli.exe decompressor_cli.cpp
```

### Backend Setup
```bash
cd backend
npm install
node server.js
```

### Frontend Setup
```bash
cd frontend
npm install
npm run dev
```

## Performance Characteristics

### Compression Efficiency
- **Best case**: Repetitive text with long runs
- **Worst case**: Random data with high entropy
- **Typical ratio**: 20-60% compression for natural language text
- **Overhead**: ~50-100 bytes for algorithm metadata

### Time Complexity
- **Compression**: O(n² log n) dominated by BWT
- **Decompression**: O(n log n) dominated by Huffman decode
- **Memory usage**: O(n²) for BWT suffix array

### Scalability
- **Input size limit**: ~1MB practical limit due to BWT memory usage
- **Concurrent requests**: Limited by available RAM
- **File upload limit**: 10MB (configurable in multer)

## Technical Dependencies

### Backend
- Node.js 16+
- Express.js 4.18+
- Multer 1.4+
- CORS 2.8+

### Frontend
- React 18+
- Vite 4+
- Tailwind CSS 4+
- Axios 1.4+

### C++
- C++11 standard
- Standard library only (no external dependencies)
- Windows/Linux compatible

## Data Formats

### Compression Output
```json
{
  "compressed": "10101010001101010100001011000010100011000000001111011",
  "primaryIndex": 42,
  "originalSize": 156,
  "compressedSize": 47,
  "timestamp": "2024-01-15T10:30:00.000Z"
}
```

### File Upload
- **Supported formats**: .txt, .json, .md
- **Encoding**: UTF-8
- **Max size**: 10MB
- **Processing**: In-memory buffer

## Error Handling

### Compression Errors
- Empty input validation
- Memory allocation failures
- Invalid character encoding
- File I/O errors

### Decompression Errors
- Invalid primary index
- Corrupted binary data
- Missing Huffman tree
- Memory allocation failures

### API Errors
- HTTP 400: Invalid input parameters
- HTTP 500: Internal server errors
- HTTP 413: File too large
- HTTP 415: Unsupported file type
