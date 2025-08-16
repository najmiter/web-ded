#!/bin/bash

echo "Building web version..."
make web

if [ $? -eq 0 ]; then
    echo "Build successful! Starting local server..."
    echo "Open http://localhost:8000 in your browser"
    echo "Press Ctrl+C to stop the server"
    cd web && python3 -m http.server 8000
else
    echo "Build failed. Please check the error messages above."
    exit 1
fi
