import React, { useState, useRef } from 'react';
import axios from 'axios';

const Compressor = () => {
    const API_BASE_URL = import.meta.env.VITE_API_BASE_URL || 'http://localhost:5000';

    const [inputText, setInputText] = useState('');
    const [compressed, setCompressed] = useState('');
    const [decompressed, setDecompressed] = useState('');
    const [primaryIndex, setPrimaryIndex] = useState(null);
    const [loading, setLoading] = useState(false);
    const [decompressing, setDecompressing] = useState(false);
    const [isExpanded, setIsExpanded] = useState(false);
    const [compressionHistory, setCompressionHistory] = useState([]);
    const [compressionTime, setCompressionTime] = useState(0);
    const [uploading, setUploading] = useState(false);
    const [uploadedFile, setUploadedFile] = useState(null);
    const fileInputRef = useRef(null);

    const handleCompress = async () => {
        if (!inputText.trim()) return;

        const startTime = performance.now();
        setLoading(true);
        setCompressed('');
        setDecompressed('');
        setPrimaryIndex(null);
        setIsExpanded(false);

        try {
            const res = await axios.post(`${API_BASE_URL}/api/compress`, { text: inputText });
            const endTime = performance.now();

            setCompressed(res.data.compressed);
            setPrimaryIndex(res.data.primaryIndex);
            setCompressionTime(endTime - startTime);

            // Add to history
            const newEntry = {
                id: Date.now(),
                original: inputText,
                compressed: res.data.compressed,
                primaryIndex: res.data.primaryIndex,
                timestamp: res.data.timestamp,
                originalSize: res.data.originalSize,
                compressedSize: res.data.compressedSize,
                compressionTime: endTime - startTime
            };
            setCompressionHistory(prev => [newEntry, ...prev.slice(0, 9)]); // Keep last 10

        } catch (err) {
            alert('Compression failed: ' + (err.response?.data?.error || err.message));
        }
        setLoading(false);
    };

    const handleDecompress = async () => {
        if (!compressed || primaryIndex === null) return;

        setDecompressing(true);
        setDecompressed('');

        try {
            const res = await axios.post(`${API_BASE_URL}/api/decompress`, {
                compressed,
                primaryIndex
            });
            setDecompressed(res.data.decompressed);
        } catch (err) {
            alert('Decompression failed: ' + (err.response?.data?.error || err.message));
        }
        setDecompressing(false);
    };

    const handleFileUpload = async (event) => {
        const file = event.target.files[0];
        if (!file) return;

        setUploading(true);
        setUploadedFile(null);
        const formData = new FormData();
        formData.append('file', file);

        const API_BASE_URL = process.env.NODE_ENV === 'production'
            ? 'https://your-render-backend-url.onrender.com'
            : 'http://localhost:5000';

        try {
            const res = await axios.post(`${API_BASE_URL}/api/upload`, formData, {
                headers: { 'Content-Type': 'multipart/form-data' }
            });

            setInputText(res.data.decompressed || 'File content loaded');
            setCompressed(res.data.compressed);
            setPrimaryIndex(res.data.primaryIndex);
            setUploadedFile({
                name: file.name,
                size: file.size,
                type: file.type,
                lastModified: file.lastModified
            });

        } catch (err) {
            alert('File upload failed: ' + (err.response?.data?.error || err.message));
        }
        setUploading(false);
    };

    const exportResults = () => {
        if (!compressed) return;

        const data = {
            compressed: compressed,
            primaryIndex: primaryIndex,
            originalSize: inputText.length,
            compressedSize: compressed.length,
            timestamp: new Date().toISOString()
        };

        const blob = new Blob([JSON.stringify(data, null, 2)], { type: 'application/json' });
        const url = URL.createObjectURL(blob);
        const a = document.createElement('a');
        a.href = url;
        a.download = `compressed_${Date.now()}.json`;
        a.click();
        URL.revokeObjectURL(url);
    };

    const compressionRatio = inputText && compressed ?
        ((inputText.length - compressed.length) / inputText.length * 100).toFixed(1) : 0;

    const displayCompressed = isExpanded ? compressed : compressed.substring(0, 50) + (compressed.length > 50 ? '...' : '');

    return (
        <div className="min-h-screen bg-gradient-to-br from-slate-900 via-blue-900 to-slate-900 text-white">
            {/* Header */}
            <div className="bg-black/20 backdrop-blur-md border-b border-white/10">
                <div className="max-w-7xl mx-auto px-6 py-8">
                    <h1 className="text-4xl md:text-5xl font-bold text-center bg-gradient-to-r from-blue-400 to-cyan-400 bg-clip-text text-transparent">
                        Multi Level Compressor
                    </h1>
                </div>
            </div>

            {/* Main Content */}
            <div className="max-w-7xl mx-auto px-6 py-8">
                <div className="grid lg:grid-cols-2 gap-8">

                    {/* Input Panel */}
                    <div className="bg-black/20 backdrop-blur-md border border-white/10 rounded-2xl shadow-2xl p-8 h-[700px] flex flex-col">
                        <h2 className="text-2xl font-bold text-center mb-6 text-blue-300">Input Text</h2>

                        {/* File Upload */}
                        <div className="mb-4">
                            <input
                                type="file"
                                ref={fileInputRef}
                                onChange={handleFileUpload}
                                accept=".txt,.json,.md"
                                className="hidden"
                            />
                            <button
                                onClick={() => fileInputRef.current?.click()}
                                disabled={uploading}
                                className="bg-white/10 hover:bg-white/20 text-white border border-white/20 font-medium py-2 px-4 rounded-lg transition-all duration-200 w-full mb-4 disabled:opacity-50"
                            >
                                {uploading ? 'Uploading...' : 'Upload Text File'}
                            </button>

                            {/* File Upload Status */}
                            {uploadedFile && (
                                <div className="bg-green-500/10 border border-green-500/30 rounded-lg p-3 mb-4">
                                    <div className="flex items-center gap-3">
                                        <div className="text-green-400">
                                            📄
                                        </div>
                                        <div className="flex-1">
                                            <div className="text-green-300 font-semibold text-sm">{uploadedFile.name}</div>
                                            <div className="text-green-200 text-xs">
                                                {(uploadedFile.size / 1024).toFixed(1)} KB •
                                                {new Date(uploadedFile.lastModified).toLocaleDateString()}
                                            </div>
                                        </div>
                                        <button
                                            onClick={() => {
                                                setUploadedFile(null);
                                                setInputText('');
                                                setCompressed('');
                                                setPrimaryIndex(null);
                                                if (fileInputRef.current) fileInputRef.current.value = '';
                                            }}
                                            className="text-red-400 hover:text-red-300 text-sm"
                                        >
                                            ✕
                                        </button>
                                    </div>
                                </div>
                            )}
                        </div>

                        <textarea
                            value={inputText}
                            onChange={(e) => setInputText(e.target.value)}
                            placeholder="Enter your text here to compress..."
                            className="flex-1 w-full p-4 bg-black/30 border border-white/20 rounded-xl text-white font-mono resize-none focus:outline-none focus:border-blue-400 transition-colors"
                        />

                        <button
                            onClick={handleCompress}
                            disabled={loading || !inputText.trim()}
                            className="bg-gradient-to-r from-blue-500 to-cyan-500 hover:from-blue-600 hover:to-cyan-600 text-white font-semibold py-3 px-6 rounded-xl transition-all duration-300 shadow-lg hover:shadow-xl transform hover:-translate-y-0.5 mt-4 disabled:opacity-50 disabled:cursor-not-allowed"
                        >
                            {loading ? 'Compressing...' : 'Compress Text'}
                        </button>
                    </div>

                    {/* Output Panel */}
                    <div className="bg-black/20 backdrop-blur-md border border-white/10 rounded-2xl shadow-2xl p-8 h-[700px] flex flex-col">
                        <h2 className="text-2xl font-bold text-center mb-6 text-green-300">Compressed Output</h2>

                        {compressed ? (
                            <div className="flex-1 flex flex-col min-h-0">
                                {/* Scrollable Output Container - Takes most of the space */}
                                <div className="flex-1 bg-black/40 border border-green-400/30 rounded-xl p-4 text-green-300 font-mono text-sm overflow-y-auto overflow-x-hidden min-h-0 max-h-96">
                                    <div className="whitespace-pre-wrap break-all">
                                        {displayCompressed}
                                    </div>
                                </div>

                                {/* Always Visible Controls and Stats - Fixed at bottom */}
                                <div className="mt-4 space-y-4 flex-shrink-0">
                                    <button
                                        onClick={() => setIsExpanded(!isExpanded)}
                                        className="bg-white/10 hover:bg-white/20 text-white border border-white/20 font-medium py-2 px-4 rounded-lg transition-all duration-200"
                                    >
                                        {isExpanded ? 'Show First 50 Bits' : 'Show All Bits'}
                                    </button>

                                    {/* Results Section */}
                                    <div className="space-y-3">
                                        <div className="bg-yellow-500/10 border border-yellow-500/30 rounded-lg p-2 text-center">
                                            <span className="text-yellow-300 font-semibold text-sm">Primary Index: {primaryIndex}</span>
                                        </div>

                                        {/* Stats Grid */}
                                        <div className="grid grid-cols-2 gap-3">
                                            <div className="bg-black/20 backdrop-blur-md border border-white/10 rounded-xl p-2 text-center">
                                                <div className="text-xs text-gray-400">Original</div>
                                                <div className="text-sm font-bold">{inputText.length}</div>
                                            </div>
                                            <div className="bg-black/20 backdrop-blur-md border border-white/10 rounded-xl p-2 text-center">
                                                <div className="text-xs text-gray-400">Compressed</div>
                                                <div className="text-sm font-bold">{compressed.length}</div>
                                            </div>
                                            <div className="bg-black/20 backdrop-blur-md border border-white/10 rounded-xl p-2 text-center">
                                                <div className="text-xs text-gray-400">Ratio</div>
                                                <div className="text-sm font-bold">{compressionRatio}%</div>
                                            </div>
                                            <div className="bg-black/20 backdrop-blur-md border border-white/10 rounded-xl p-2 text-center">
                                                <div className="text-xs text-gray-400">Time</div>
                                                <div className="text-sm font-bold">{compressionTime.toFixed(0)}ms</div>
                                            </div>
                                        </div>

                                        {/* Stats Explanation */}
                                        <div className="bg-blue-500/10 border border-blue-500/30 rounded-lg p-3">
                                            <h4 className="text-blue-300 font-semibold text-sm mb-2">Understanding the Stats:</h4>
                                            <div className="text-xs text-blue-200 space-y-1">
                                                <div><span className="font-medium">Original:</span> Number of characters in your input text</div>
                                                <div><span className="font-medium">Compressed:</span> Length of the binary output (bits)</div>
                                                <div><span className="font-medium">Ratio:</span>
                                                    {compressionRatio > 0 ? (
                                                        <span className="text-green-300">Positive = space saved</span>
                                                    ) : (
                                                        <span className="text-yellow-300">Negative = overhead from compression algorithm</span>
                                                    )}
                                                </div>
                                                <div><span className="font-medium">Time:</span> How long the compression took</div>
                                                <div className="text-blue-300 mt-2">
                                                    💡 <span className="font-medium">Tip:</span> Longer, repetitive texts compress better!
                                                </div>
                                            </div>
                                        </div>

                                        {/* Action Buttons */}
                                        <div className="flex gap-3">
                                            <button
                                                onClick={handleDecompress}
                                                disabled={decompressing || !compressed}
                                                className="bg-white/10 hover:bg-white/20 text-white border border-white/20 font-medium py-2 px-4 rounded-lg transition-all duration-200 flex-1 disabled:opacity-50"
                                            >
                                                {decompressing ? 'Decompressing...' : 'Decompress'}
                                            </button>
                                            <button
                                                onClick={exportResults}
                                                className="bg-white/10 hover:bg-white/20 text-white border border-white/20 font-medium py-2 px-4 rounded-lg transition-all duration-200 flex-1"
                                            >
                                                Export
                                            </button>
                                        </div>
                                    </div>
                                </div>
                            </div>
                        ) : (
                            <div className="flex-1 flex items-center justify-center text-gray-400 text-center">
                                Compressed output will appear here...
                            </div>
                        )}
                    </div>
                </div>

                {/* Decompressed Result */}
                {decompressed && (
                    <div className="bg-black/20 backdrop-blur-md border border-white/10 rounded-2xl shadow-2xl p-8 mt-8">
                        <h3 className="text-2xl font-bold text-center mb-6 text-purple-300">Decompressed Result</h3>
                        <div className="bg-black/40 border border-purple-400/30 rounded-xl p-4 text-purple-200 font-mono max-h-60 overflow-y-auto">
                            {decompressed}
                        </div>
                        <div className="mt-4 text-center">
                            <span className={`px-4 py-2 rounded-lg ${decompressed === inputText ? 'bg-green-500/20 text-green-300' : 'bg-red-500/20 text-red-300'}`}>
                                {decompressed === inputText ? 'Perfect Match!' : 'Mismatch Detected'}
                            </span>
                        </div>
                    </div>
                )}

                {/* Compression History */}
                {compressionHistory.length > 0 && (
                    <div className="bg-black/20 backdrop-blur-md border border-white/10 rounded-2xl shadow-2xl p-8 mt-8">
                        <h3 className="text-2xl font-bold text-center mb-6 text-cyan-300">Recent Compressions</h3>
                        <div className="grid md:grid-cols-2 lg:grid-cols-3 gap-4">
                            {compressionHistory.map((entry) => (
                                <div key={entry.id} className="bg-black/20 backdrop-blur-md border border-white/10 rounded-2xl p-4 hover:bg-white/5 transition-colors cursor-pointer" onClick={() => {
                                    setInputText(entry.original);
                                    setCompressed(entry.compressed);
                                    setPrimaryIndex(entry.primaryIndex);
                                }}>
                                    <div className="text-sm text-gray-400 mb-2">
                                        {new Date(entry.timestamp).toLocaleString()}
                                    </div>
                                    <div className="text-xs text-gray-500 mb-2">
                                        {entry.originalSize} → {entry.compressedSize} chars
                                    </div>
                                    <div className="text-xs text-gray-500">
                                        {entry.compressionTime.toFixed(0)}ms
                                    </div>
                                </div>
                            ))}
                        </div>
                    </div>
                )}
            </div>
        </div>
    );
};

export default Compressor; 