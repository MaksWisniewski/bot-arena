#!/bin/bash

image_name="bot_area_image"

# Display available options
echo "Available Options:"
echo "1. Run Docker build command"
echo "2. Run Docker container"
echo "3. Compile c++ bots"
echo "4. Clean c++ bots"
echo "5. Run sim"
echo "6. Run sim for minmax and random on 5x5"




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
        docker run --rm -v "$(pwd):/usr/src/app" -it "$image_name" zsh
        ;;
    3)
        # Compile c++ bots
        docker run --rm -v "$(pwd):/usr/src/app" -it "$image_name" sh -c "cd cpp_bots && make all"
        ;;
    4)
        # Clean c++ bots
        docker run --rm -v "$(pwd):/usr/src/app" -it "$image_name" sh -c "cd cpp_bots && make clean"
        ;;
    5)
        # Run sim
        docker run --rm -v "$(pwd):/usr/src/app" -it "$image_name" zsh -c "python3 main.py run-simulation"
        ;;
    6)
        # Run sim for minmax and random on 5x5
        docker run --rm -v "$(pwd):/usr/src/app" -it "$image_name" zsh -c "python3 main.py run-simulation --b1 minmax --b2 random_bot.py --map map5x5.json"
        ;;
    *)
        echo "Invalid option. Please enter a valid number."
        ;;
esac
