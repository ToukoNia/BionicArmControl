import serial
import time
import csv
from datetime import datetime
import os


# Configure the serial connection
arduino_port = "COM3"  # Arduino's port
baud_rate = 9600       # same as arduino baud rate
output_file = "emgData.csv"

script_directory = os.path.dirname(os.path.abspath(__file__))
file_path = os.path.join(script_directory, output_file) #ensures csv file saves to same directory as python script

# Open serial connection
ser = serial.Serial(arduino_port, baud_rate)
time.sleep(2) 

print("Connected to Arduino on", arduino_port)

with open(file_path, mode='a', newline='') as file:
    writer = csv.writer(file)
    writer.writerow(["Timestamp", "Sensor1", "Sensor2"])

    try:
        print("Recording data. Press Ctrl+C to stop.")
        
        # Continuously read data from Arduino
        while True:
            # Read a line of data from Arduino
            line = ser.readline().decode('utf-8').strip()  # Read and decode line
            data = line.split(",")  #converts data line into array

            timestamp = datetime.now().strftime("%H:%M:%S")
            data = [timestamp] + data
            
            # Write the data to the CSV file
            writer.writerow(data)
            print(f"Received: {data}")
            
    except KeyboardInterrupt:
        print("\nRecording stopped")
    except Exception as e:
        print(f"Error: {e}")
    finally:
        ser.close()
        print("Serial connection closed.")

