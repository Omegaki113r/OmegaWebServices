const express = require('express');
const multer = require('multer');
const fs = require('fs');
const path = require('path');
const bodyParser = require('body-parser');

const app = express();

// Set body parser options (increase the limit to allow larger files)
app.use(bodyParser.raw({
    limit: '100mb',  // Adjust this according to your file size (e.g., 100MB)
    type: 'application/octet-stream'
}));

const uploadDir = path.join(__dirname, 'uploads');
if (!fs.existsSync(uploadDir)) {
    fs.mkdirSync(uploadDir);
}

const storage = multer.diskStorage({
    destination: (req, file, cb) => {
        cb(null, uploadDir);
    },
    filename: (req, file, cb) => {
        cb(null, 'uploaded_file.bin');
    }
});

const upload = multer({ storage: storage });

app.post('/uploads', upload.single('file'), (req, res) => {
    const filePath = path.join(uploadDir, 'uploaded_file.bin');
    if (req.body) {
        fs.appendFile(filePath, req.body, (err) => {
            if (err) {
                console.error("Error saving the chunk:", err);
                return res.status(500).send('Internal Server Error');
            }
            console.log(`Received chunk, total file size: ${fs.statSync(filePath).size} bytes`);
            res.status(200).send('Chunk received successfully');
        });
    } else {
        res.status(400).send('No data received');
    }
});

const hostname = '0.0.0.0';
const port = 3000;

app.listen(port, hostname, () => {
    console.log(`Server running at http://${hostname}:${port}/`);
});
