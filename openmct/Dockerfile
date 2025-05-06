 # Use Node.js 22 as the base image
FROM node:22-alpine

# Set working directory
WORKDIR /app

# Define build arguments
ARG TELEMETRY_SERVER_IP
ARG TELEMETRY_SERVER_PORT

# Set environment variables
ENV TELEMETRY_SERVER_IP=${TELEMETRY_SERVER_IP:-localhost}
ENV TELEMETRY_SERVER_PORT=${TELEMETRY_SERVER_PORT:-8000}

# Copy package files
COPY package*.json ./

# Install dependencies
RUN npm install

# Copy source code
COPY . .

# Build the application
RUN npm run build

# Expose the port the app runs on
EXPOSE 8080

# Start the application
CMD ["npm", "start"]