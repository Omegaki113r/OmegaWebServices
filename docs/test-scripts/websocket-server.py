'''
@file websocket-server.py
@author Omegaki113r
@date Tuesday, 21st January 2025 11:05:14 pm
@copyright Copyright <<projectCreationYear>> - 2025 0m3g4ki113r, Xtronic
*//*
Project: test-scripts
File Name: websocket-server.py
File Created: Tuesday, 21st January 2025 11:05:14 pm
Author: Omegaki113r (omegaki113r@gmail.com)
-----
Last Modified: Tuesday, 21st January 2025 11:19:37 pm
Modified By: Omegaki113r (omegaki113r@gmail.com)
-----
Copyright <<projectCreationYear>> - 2025 0m3g4ki113r, Xtronic
-----
HISTORY:
Date      	By	Comments
----------	---	---------------------------------------------------------
'''

import asyncio
import websockets

# This is a simple handler function that will be called when a client connects.
async def echo(websocket, path):
    print(f"Client connected from {websocket.remote_address}")
    try:
        # Keep receiving messages from the client and send them back (echo).
        async for message in websocket:
            print(f"Received message: {message}")
            await websocket.send(f"Echo: {message}")
    except websockets.exceptions.ConnectionClosed as e:
        print(f"Connection closed with {websocket.remote_address}: {e}")

# Define the main server function
async def main():
    # Start the WebSocket server
    async with websockets.serve(echo, "127.0.0.1", 8765):
        print("Server started on ws://127.0.0.1:8765")
        # Keep the server running indefinitely
        await asyncio.Future()

# Start the event loop to run the server
asyncio.run(main())
