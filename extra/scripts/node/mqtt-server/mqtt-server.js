/*
 * @file mqtt-server.js
 * @author Omegaki113r
 * @date Monday, 10th February 2025 3:05:16 am
 * @copyright Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * *//*
* Project: mqtt-server
* File Name: mqtt-server.js
* File Created: Monday, 10th February 2025 3:05:16 am
* Author: Omegaki113r (omegaki113r@gmail.com)
* -----
* Last Modified: Monday, 10th February 2025 3:05:16 am
* Modified By: Omegaki113r (omegaki113r@gmail.com)
* -----
* Copyright 2025 - 2025 0m3g4ki113r, Xtronic
* -----
* HISTORY:
* Date      	By	Comments
* ----------	---	---------------------------------------------------------
*/

const aedes = require('aedes')(); // Import and create an instance of the aedes MQTT broker
const net = require('net'); // To create a TCP server

// Create a TCP server that listens on port 1883
const server = net.createServer(aedes.handle);

// Make the server listen on port 1883 (MQTT default port)
server.listen(1883, () => {
    console.log('MQTT broker is running on port 1883');
});

// Setup the broker's on 'publish' event to log when a message is published
aedes.on('publish', (packet, client) => {
    if (client) {
        const topic = packet.topic;
        const payload = packet.payload.toString(); // Convert buffer to string
        console.log(`Received message from client ${client.id}:`);
        console.log(`Topic: ${topic}`);
        console.log(`Payload: ${payload}`);


        const sending_packet = {
            topic: 'broker/announcement',
            payload: Buffer.from('Hello from the Aedes broker!'), // Payload must be a Buffer
            qos: 0, // Quality of Service (0 = at most once)
            retain: false // Whether to retain the message
        };
        aedes.publish(sending_packet);
    }
});

aedes.on('subscribe', (subscription, client) => {
    console.log(subscription[0].topic + ": " + subscription[0].qos);
});

// Handle incoming connections from clients
aedes.on('client', (client) => {
    console.log('Client Connected:', client.id);
});

// Handle client disconnections
aedes.on('clientDisconnect', (client) => {
    console.log('Client Disconnected:', client.id);
});
