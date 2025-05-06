# The context of this dockerfile should be the root of the project

FROM python:3.13-slim

# copy command and codec modules into the container
WORKDIR /app
COPY command command
COPY codec codec

# set our working directory to the command module
WORKDIR /app/command

# Install the required dependencies
RUN python -m pip install --upgrade pip && \
    python -m pip install -r requirements.txt

EXPOSE 8000

# Command to run your application
CMD ["python", "run.py"]