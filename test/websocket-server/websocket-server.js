/*
 * @file websocket-server.js
 * @author Omegaki113r
 * @date Tuesday, 11th February 2025 1:33:46 pm
 * @copyright Copyright <<projectCreationYear>> - 2025 0m3g4ki113r, Xtronic
 * */
/*
* Project: websocket-server
* File Name: websocket-server.js
* File Created: Tuesday, 11th February 2025 1:33:46 pm
* Author: Omegaki113r (omegaki113r@gmail.com)
* -----
* Last Modified: Tuesday, 11th February 2025 1:33:46 pm
* Modified By: Omegaki113r (omegaki113r@gmail.com)
* -----
* Copyright <<projectCreationYear>> - 2025 0m3g4ki113r, Xtronic
* -----
* HISTORY:
* Date      	By	Comments
* ----------	---	---------------------------------------------------------
*/

const WebSocket = require('ws');

// Create a WebSocket server on port 8080
const wss = new WebSocket.Server({ port: 8080 });

// Handle connection event
wss.on('connection', (ws) => {
    // console.log('Client connected');

    // Send a message to the client
    // ws.send('Hello from WebSocket server');

    // Handle incoming messages from clients
    ws.on('message', (message) => {
        // console.log('Received message:', message);
    });

    // Handle client disconnection
    ws.on('close', () => {
        console.log('Client disconnected');
    });
});

console.log('WebSocket server running on ws://localhost:8080');

