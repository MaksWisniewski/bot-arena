# Use a base image that has the necessary environment for your project
FROM alpine

# Set the working directory inside the container
WORKDIR /usr/src/app

# Specify the directory to be mounted
VOLUME /usr/src/app

# Copy the contents of the test directory into the container
COPY . .

# Install required packages
RUN apk add --no-cache g++ make nlohmann-json

# RUN echo "Additional commands go here"

# Specify the command to run your application (if applicable)
# CMD [ "executable", "parameters" ]