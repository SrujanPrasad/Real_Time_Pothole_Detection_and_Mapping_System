# Real-Time Pothole Detection System 

This project was developed as part of the Experiential Learning course to fulfil the requirements of the 5th Semester, and was later carried forward into the 6th Semester with further upgrades and modifications.

## Problem Statement
Every year, thousands of lives are lost on Indian roads due to accidents, with a significant proportion caused by potholes. 
These accidents are more frequent and dangerous during the monsoon season when potholes are hidden under water. 
Poor road infrastructure and the inability to detect potholes in real time pose a serious threat to vehicle safety. 
A dedicated system can detect potholes in real-time and alert drivers effectively, which would greatly reduce the risk of accidents, enabling vehicles to avoid hazards and save lives. 
Such a solution would enhance road safety and ensure a smoother, safer driving experience.

## Objectives
- To develop a low-cost, real-time system that detects road potholes using sensors and maps their locations for proactive repair and enhanced road safety, by incorporating the following practical features:

- To utilize affordable sensors such as accelerometers and ultrasonic modules for reliable pothole detection based on vibration and depth analysis.

- To integrate GPS modules for precise geolocation tagging of each detected pothole.

- To enable wireless transmission of pothole data using communication modules like ESP32 via Wi-Fi or GSM to a cloud-based server.

- To visualize pothole locations and related data on an interactive map/dashboard for easy access by road maintenance authorities.

- To ensure energy-efficient operation using low-power components and optimized power management strategies.

## Methodology 
- ESP32 Initialization: Starts the microcontroller and sensor modules.
- Sensor Data Collection: MPU6050 measures acceleration and tilt. 
- Pothole Detection Logic: Detects Z-axis spikes (> ±1.5g) and tilt to identify potholes
- GPS module detects the location and displays the latitudes and longitudes (Subject to the signal strength)
- Data is sent to the cloud (Thing Speak) and the graphs are displayed, updating in real time.
  and displays on Google Maps.

Below is the flowchart of the same : 

<img width="732" height="640" alt="image" src="https://github.com/user-attachments/assets/d70fab66-8ca2-48ad-85b8-70b46a16d45b" />

## Tech Used 
- ESP32 – Microcontroller with Wi-Fi and Bluetooth. 
- MPU6050 – 6-axis accelerometer + gyroscope. 
- GPS Module – Real-time location logging. 
- 4WD Robot Car – Mobile platform. 
- Ultrasonic sensor (HC-05). 
- Power Supply – Rechargeable Lithium-ion battery with voltage regulation.
- Coding is done on Arduino IDE in C++ language with necessary libraries installed.

## Results

![WhatsApp Image 2025-06-27 at 19 29 13_f3d40344](https://github.com/user-attachments/assets/8ec504b3-7c1d-43c2-a311-4df6af333ea8)

![WhatsApp Image 2025-06-27 at 19 29 14_6b4125d3](https://github.com/user-attachments/assets/723afa90-bf3b-47ed-a401-931e47f14073) 

![WhatsApp Image 2025-07-07 at 21 56 19_7792722e](https://github.com/user-attachments/assets/3b81d8e2-44af-4f5f-9be1-3bf64564bfdf)

![WhatsApp Image 2025-07-07 at 21 53 53_6ae088a4](https://github.com/user-attachments/assets/3290ae65-1015-474d-914f-b6b0c72f69a9) 

![WhatsApp Image 2025-07-07 at 21 55 01_5765713d](https://github.com/user-attachments/assets/fd8eed81-b6ce-4d92-9a21-4c791a0b62dc) 







## Key Outcomes 
- Detection Accuracy: GPS logs within ±5m accuracy.
- Web Interface: Real-time map updates with pothole locations.
- Cost-Effective: Built with readily available components under a student budget 
- Scalable: Easily deployable in urban or rural settings 
- Impact: Enables faster repairs, reduces accident risks, and supports smart infrastructure
