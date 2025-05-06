# The context of this dockerfile should be the root of the project

FROM python:3.13

# copy onboard server and codec modules into the container
WORKDIR /app
COPY onboard-server-py onboard-server-py
COPY bcp-redis-client/python bcp-redis-client/python
COPY codec codec

# set our working directory to the onboard-server-py module
WORKDIR /app/onboard-server-py

# Install the local packages that the onboard-server package depends on
RUN pip install --no-cache-dir \
    file:../codec \
    file:../bcp-redis-client/python

# Install the onboard-server package
RUN pip install --no-cache-dir file:.

EXPOSE 8888

# Command to run your application
CMD ["python", "run.py"]