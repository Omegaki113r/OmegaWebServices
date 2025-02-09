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


// Variables to track incoming data rate
let totalData = 0; // Total data in bytes
let lastTimestamp = Date.now(); // Timestamp of the last message
let dataRate = 0; // Data rate in bytes per second
let dataRateKBps = 0; // Data rate in kilobytes per second
let dataRateMBps = 0; // Data rate in megabytes per second
let dataRateMbps = 0; // Data rate in megabits per second

// Setup the broker's on 'publish' event to log when a message is published
aedes.on('publish', (packet, client) => {
    const currentTimestamp = Date.now();

    // Calculate the size of the payload (in bytes)
    const payloadSize = packet.payload.length;

    // Accumulate the total data
    totalData += payloadSize;

    // Calculate the time elapsed since the last message
    const elapsedTime = (currentTimestamp - lastTimestamp) / 1000; // in seconds

    if (elapsedTime > 0) {
        // Calculate data rate in bytes per second
        dataRate = totalData / elapsedTime;

        // Convert data rate to kilobytes per second (KBps)
        dataRateKBps = dataRate / 1024; // 1 KB = 1024 bytes

        // Convert data rate to megabytes per second (MBps)
        dataRateMBps = dataRate / 1048576; // 1 MB = 1024 * 1024 bytes

        // Convert data rate to megabits per second (Mbps)
        dataRateMbps = dataRate * 8 / 1048576; // Convert bytes/sec to bits/sec and then to megabits
    }

    // Log the topic, payload, and incoming data rate
    // console.log('Message Published:');
    // console.log('Topic:', packet.topic);
    // console.log('Payload Size (bytes):', payloadSize);
    // console.log('Total Data Received (bytes):', totalData);
    // console.log('Elapsed Time (seconds):', elapsedTime);
    // console.log('Incoming Data Rate (bytes/sec):', dataRate);
    // console.log('Incoming Data Rate (KB/sec):', dataRateKBps.toFixed(2)); // Display with 2 decimal points
    console.log('Incoming Data Rate (MB/sec):', dataRateMBps.toFixed(4)); // Display with 4 decimal points for MBps
    // console.log('Incoming Data Rate (Mbps):', dataRateMbps.toFixed(4)); // Display with 4 decimal points for Mbps

    // Update the last timestamp
    lastTimestamp = currentTimestamp;
});



// Handle incoming connections from clients
aedes.on('client', (client) => {
    console.log('Client Connected:', client.id);
});

// Handle client disconnections
aedes.on('clientDisconnect', (client) => {
    console.log('Client Disconnected:', client.id);
});
