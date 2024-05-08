#!/bin/bash

image_name="bot_area_image"

# Display available options
echo "Available Options:"
echo "1. Run Docker build command"
echo "2. Run Docker container"
echo "3. Compile c++ bots"
echo "4. Clean c++ bots"


# Prompt user for option choice
read -p "Enter the number of the option you want to select: " choice

# Execute selected option
case $choice in
    1)
        # Run Docker build command
        docker build -t "$image_name" -f docker/Dockerfile .
        ;;
    2)
        # Run Docker container
        docker run --rm -v "$(pwd):/usr/src/app" -it "$image_name" sh
        ;;
    3)
        # Compile c++ bots
        docker run --rm -v "$(pwd):/usr/src/app" -it "$image_name" sh -c "cd cpp_bots && make clean && make"
        ;;
    4)
        # Compile c++ bots
        docker run --rm -v "$(pwd):/usr/src/app" -it "$image_name" sh -c "cd cpp_bots && make clean"
        ;;
    *)
        echo "Invalid option. Please enter a valid number."
        ;;
esac
