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
RUN apt update && apt install -y zsh nlohmann-json3-dev python3 python3-numpy python3-click python3-pygame python3-pip 

# Add a new user with user ID 1000 and set zsh as its shell
RUN useradd -u 1000 -m -s $(which zsh) myuser

USER myuser

# USER 1000
# # Change default shell to zsh for user 1000
# RUN chsh -s $(which zsh) && \
#     usermod -s $(which zsh) 1000


RUN echo 'echo "Welcome to Debian" && ls' >> ~/.zshrc

# RUN echo "Additional commands go here"

# Specify the command to run your application (if applicable)
# CMD [ "executable", "parameters" ]
