image_name="bot_area_image"
if [ "$1" = "select-economy" ] || [ "$1" = "-e" ]; then
    shift
    docker run --rm -v "$(pwd):/usr/src/app":z -it "$image_name" python3 main.py select-economy "$@"
    exit $?
fi

if [ "$1" = "run-simulation" ] || [ "$1" = "--sim" ]; then
    shift
    docker run --rm -v "$(pwd):/usr/src/app":z -it "$image_name" python3 main.py run-simulation "$@"
    exit $?
fi

if [ "$1" = "rerun-simulation" ] || [ "$1" = "--sim-r" ]; then
    docker run --rm -v "$(pwd):/usr/src/app":z -it "$image_name" python3 main.py rerun-simulation
    exit $?
fi

if [ "$1" = "concurrent-rerun-simulation" ] || [ "$1" = "--csim-r" ]; then
    docker run --rm -v "$(pwd):/usr/src/app":z -it "$image_name" python3 main.py concurrent-rerun-simulation
    exit $?
fi

if [ "$1" = "run-simulation-from-config" ] || [ "$1" = "--sim-c" ]; then
    shift
    docker run --rm -v "$(pwd):/usr/src/app":z -it "$image_name" python3 main.py run-simulation-from-config "$@"
    exit $?
fi

if [ "$1" = "compile" ] || [ "$1" = "--compile" ]; then
    shift
    docker run --rm -v "$(pwd):/usr/src/app":z -it "$image_name" sh -c 'cd cpp_bots && make "$@"' -- "$@"
    exit $?
fi

if [ "$1" = "clean" ] || [ "$1" = "--clean" ]; then
    shift
    docker run --rm -v "$(pwd):/usr/src/app":z -it "$image_name" sh -c "cd cpp_bots && make clean"
    exit $?
fi

if [ "$1" = "build-docker" ] || [ "$1" = "--build-docker" ]; then
    docker build -t "$image_name" -f docker/Dockerfile .
    exit $?
fi

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
        docker run --rm -v "$(pwd):/usr/src/app":z -it "$image_name" zsh
        ;;
    3)
        # Compile c++ bots
        docker run --rm -v "$(pwd):/usr/src/app":z -it "$image_name" sh -c "cd cpp_bots && make all"
        ;;
    4)
        # Clean c++ bots
        docker run --rm -v "$(pwd):/usr/src/app":z -it "$image_name" sh -c "cd cpp_bots && make clean"
        ;;
    5)
        # Run sim
        docker run --rm -v "$(pwd):/usr/src/app":z -it "$image_name" python3 main.py run-simulation
        ;;
    6)
        # Run sim for minmax and random on 5x5
        docker run --rm -v "$(pwd):/usr/src/app":z -it "$image_name" python3 main.py run-simulation --b1 minmax --b2 random_bot.py --map map5x5.json
        ;;
    *)
        echo "Invalid option. Please enter a valid number."
        ;;
esac
