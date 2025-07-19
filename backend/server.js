const express = require('express');
const cors = require('cors');
const { spawn } = require('child_process');
const multer = require('multer');
const path = require('path');

const app = express();
const PORT = process.env.PORT || 5000;

app.use(cors());
app.use(express.json());

// File upload configuration
const storage = multer.memoryStorage();
const upload = multer({ storage: storage });

// Compression endpoint
app.post('/api/compress', (req, res) => {
    const { text } = req.body;
    if (!text) return res.status(400).json({ error: 'No text provided' });

    const exeName = process.platform === 'win32' ? 'compressor_cli.exe' : './compressor_cli';
    const compressor = spawn(exeName, [], { cwd: __dirname });

    let output = '';
    let error = '';

    compressor.stdin.write(text);
    compressor.stdin.end();

    compressor.stdout.on('data', (data) => { output += data.toString(); });
    compressor.stderr.on('data', (data) => { error += data.toString(); });

    compressor.on('close', (code) => {
        if (code !== 0) {
            return res.status(500).json({ error: error || 'Compression failed' });
        }
        try {
            const lines = output.trim().split('\n');
            const jsonLine = lines[lines.length - 1];
            const result = JSON.parse(jsonLine);
            res.json({
                ...result,
                timestamp: new Date().toISOString(),
                originalSize: text.length,
                compressedSize: result.compressed.length
            });
        } catch (e) {
            res.status(500).json({ error: 'Invalid output from compressor' });
        }
    });
});

// Decompression endpoint
app.post('/api/decompress', (req, res) => {
    const { compressed, primaryIndex } = req.body;
    if (!compressed || primaryIndex === undefined) {
        return res.status(400).json({ error: 'Missing compressed data or primary index' });
    }

    const exeName = process.platform === 'win32' ? 'decompressor_cli.exe' : './decompressor_cli';
    const decompressor = spawn(exeName, [], { cwd: __dirname });

    let output = '';
    let error = '';

    decompressor.stdin.write(JSON.stringify({ compressed, primaryIndex }));
    decompressor.stdin.end();

    decompressor.stdout.on('data', (data) => { output += data.toString(); });
    decompressor.stderr.on('data', (data) => { error += data.toString(); });

    decompressor.on('close', (code) => {
        if (code !== 0) {
            return res.status(500).json({ error: error || 'Decompression failed' });
        }
        res.json({ 
            decompressed: output.trim(),
            timestamp: new Date().toISOString()
        });
    });
});

// File upload endpoint
app.post('/api/upload', upload.single('file'), (req, res) => {
    if (!req.file) {
        return res.status(400).json({ error: 'No file uploaded' });
    }

    const text = req.file.buffer.toString('utf8');
    if (!text.trim()) {
        return res.status(400).json({ error: 'File is empty' });
    }

    // Use the same compression logic
    const exeName = process.platform === 'win32' ? 'compressor_cli.exe' : './compressor_cli';
    const compressor = spawn(exeName, [], { cwd: __dirname });

    let output = '';
    let error = '';

    compressor.stdin.write(text);
    compressor.stdin.end();

    compressor.stdout.on('data', (data) => { output += data.toString(); });
    compressor.stderr.on('data', (data) => { error += data.toString(); });

    compressor.on('close', (code) => {
        if (code !== 0) {
            return res.status(500).json({ error: error || 'Compression failed' });
        }
        try {
            const lines = output.trim().split('\n');
            const jsonLine = lines[lines.length - 1];
            const result = JSON.parse(jsonLine);
            res.json({
                ...result,
                timestamp: new Date().toISOString(),
                originalSize: text.length,
                compressedSize: result.compressed.length,
                filename: req.file.originalname
            });
        } catch (e) {
            res.status(500).json({ error: 'Invalid output from compressor' });
        }
    });
});

app.listen(PORT, () => {
    console.log(`Server running on port ${PORT}`);
}); 