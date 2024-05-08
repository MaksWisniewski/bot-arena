# Use a base image that has the necessary environment for your project
FROM gcc:13.2

# Set the working directory inside the container
WORKDIR /usr/src/app

# Specify the directory to be mounted
VOLUME /usr/src/app

# Copy the contents of the test directory into the container
COPY . .

USER root
# Install required packages
RUN apt update
RUN apt install nlohmann-json3-dev
USER 1000

# RUN echo "Additional commands go here"

# Specify the command to run your application (if applicable)
# CMD [ "executable", "parameters" ]
