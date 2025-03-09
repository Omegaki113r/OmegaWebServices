/*
 * @file web-server.js
 * @author Omegaki113r
 * @date Tuesday, 11th February 2025 8:09:17 pm
 * @copyright Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * */
/*
* Project: web-server
* File Name: web-server.js
* File Created: Tuesday, 11th February 2025 8:09:17 pm
* Author: Omegaki113r (omegaki113r@gmail.com)
* -----
* Last Modified: Tuesday, 11th February 2025 8:09:17 pm
* Modified By: Omegaki113r (omegaki113r@gmail.com)
* -----
* Copyright 2025 - 2025 0m3g4ki113r, Xtronic
* -----
* HISTORY:
* Date      	By	Comments
* ----------	---	---------------------------------------------------------
*/

const express = require('express');
const fs = require('fs');

const app = express();

app.get("/ota", (request, response) => {
    response.download("uploads/data.txt");
});

app.post("/log", (request, response) => {
    const writeStream = fs.createWriteStream('uploads/log.txt');
    request.pipe(writeStream);

    request.on('end', () => {
        writeStream.end();
        response.status(200).send('Log data saved successfully');
    });

    request.on('error', (err) => {
        console.error('Request error:', err);
        writeStream.end();
        response.status(500).send('Error processing stream');
    });

    writeStream.on('error', (err) => {
        console.error('Write stream error:', err);
        response.status(500).send('Error writing to file');
    });
});

app.listen(3000);