/*
 * @file web-server.js
 * @author Omegaki113r
 * @date Tuesday, 11th February 2025 8:09:17 pm
 * @copyright Copyright <<projectCreationYear>> - 2025 0m3g4ki113r, Xtronic
 * *//*
* Project: web-server
* File Name: web-server.js
* File Created: Tuesday, 11th February 2025 8:09:17 pm
* Author: Omegaki113r (omegaki113r@gmail.com)
* -----
* Last Modified: Tuesday, 11th February 2025 8:09:17 pm
* Modified By: Omegaki113r (omegaki113r@gmail.com)
* -----
* Copyright <<projectCreationYear>> - 2025 0m3g4ki113r, Xtronic
* -----
* HISTORY:
* Date      	By	Comments
* ----------	---	---------------------------------------------------------
*/

const UPLOAD_IDX = 0;
const OTA_IDX = 1;

const request_paths = ['/uploads', '/ota'];

const http = require('http');
const fs = require('fs');
const path = require('path');
const multer = require('multer');

// Set up multer to store uploaded files
const upload = multer({
    dest: 'uploads/', // The directory where files will be uploaded
    // limits: { fileSize: 10 * 1024 * 1024 }, // Limit file size to 10 MB
}).single('file'); // We expect a single file with the field name "file"

function handle_post(req, res) {
    console.log(req);
    upload(req, res, (err) => {
        if (err) {
            // If an error occurs during file upload
            res.statusCode = 400;
            res.setHeader('Content-Type', 'text/plain');
            res.end('Error uploading file: ' + err.message);
        } else {
            // If the file is uploaded successfully
            res.statusCode = 200;
            res.setHeader('Content-Type', 'application/json');
            // res.setHeader('Content-Type', 'text/plain');
            res.end(JSON.stringify({
                message: 'File uploaded successfully!',
                file: req.file, // File information
            }));
        }
    });
}

function handle_get(request, result) { }

// Create the server
const server = http.createServer((req, res) => {
    console.log(req.url);
    switch (req.url) {
        case request_paths[UPLOAD_IDX]: {
            if ('POST' === req.method) {
                handle_post(req, res);
            }
            break;
        }
        case request_paths[OTA_IDX]: {
            if ('GET' === req.method) {
                handle_get(req, res);
            }
            break;
        }

        default: {
            res.statusCode = 404;
            res.end();
            break;
        }
    }

    // // Handle POST requests (File upload)
    // if (req.method === 'POST') {
    //     if (req.url === '/uploads') {
    //         // Use multer to handle file uploads
    //         upload(req, res, (err) => {
    //             if (err) {
    //                 // If an error occurs during file upload
    //                 res.statusCode = 400;
    //                 res.setHeader('Content-Type', 'text/plain');
    //                 res.end('Error uploading file: ' + err.message);
    //             } else {
    //                 // If the file is uploaded successfully
    //                 res.statusCode = 200;
    //                 res.setHeader('Content-Type', 'application/json');
    //                 // res.setHeader('Content-Type', 'text/plain');
    //                 res.end(JSON.stringify({
    //                     message: 'File uploaded successfully!',
    //                     file: req.file, // File information
    //                 }));
    //             }
    //         });
    //     } else {
    //         // If an error occurs during file upload
    //         res.statusCode = 400;
    //         res.setHeader('Content-Type', 'text/plain');
    //         res.end('Error uploading file');
    //     }
    // } else {
    //     // For any non-POST request, return a simple message
    //     res.statusCode = 200;
    //     // res.setHeader('Content-Type', 'text/html');
    //     res.setHeader('Content-Type', 'text/plain');
    //     const data = fs.readFileSync('uploads/data.txt', 'utf8');
    //     res.end(data);
    // }
});

// Define the port and hostname
const hostname = '192.168.1.4'; // localhost
const port = 3000;

// Start the server
// server.listen(port, hostname, () => {
//     console.log(`Server running at http://${hostname}:${port}/`);
// });


server.listen(port, () => {
    console.log(`Server running at http://${hostname}:${port}/`);
});