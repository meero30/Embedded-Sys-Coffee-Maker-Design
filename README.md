# Automatic Coffee Maker with Coffee Ground and Water Dispenser (Arduino Uno R3)

This project presents the design and application of an **automatic coffee maker** using an **Arduino Uno R3** microcontroller. The system integrates input devices, sensors, and actuators to automate the brewing process, making it more accessible for individuals with disabilities while ensuring consistent coffee quality.

## 📖 Background
Coffee is one of the most consumed beverages worldwide. However, individuals with disabilities may struggle with traditional coffee-making due to the required dexterity and effort. This project addresses the issue by automating the process of dispensing water and coffee grounds, as well as brewing, with minimal manual intervention.

## 🎯 Objectives
- Design an **Arduino Uno R3–based system** that automates the coffee brewing process.  
- Enable users to set their desired **coffee-to-water ratio** using a keypad.  
- Incorporate **digital I/O, relays, and PWM control** for dispensing coffee grounds, pumping water, and managing the coffee machine.  
- Improve accessibility by reducing the physical effort required.  

## 🛠️ Components
- **Arduino Uno R3** – Microcontroller (system brain)  
- **4x3 Keypad** – User input for cups and coffee-to-water ratio  
- **LCD Display** – Displays brewing status and prompts  
- **Load Cell + HX711 Amplifier** (conceptual, simulated with LM35 sensor) – Measures coffee ground weight  
- **Servo Motors** – Control coffee ground and water dispensers  
- **Water Pump** – Dispenses water into the coffee machine  
- **Relays & Transistors** – Control high-power components like pump and heating element  

## ⚙️ System Flow
1. User inputs number of cups and preferred coffee-to-water ratio via keypad.  
2. Arduino processes inputs and actuates servo motors to open dispensers.  
3. Water and coffee grounds are dispensed according to set ratio.  
4. Lid closes automatically, and brewing begins.  
5. LCD provides real-time updates until coffee is ready.  

## 📊 Significance
- **Accessibility:** Reduces effort for individuals with disabilities.  
- **Consistency:** Ensures precise coffee-to-water ratio every time.  
- **Efficiency:** Saves time and reduces wastage of coffee grounds.  

## 👥 Authors
- **Renz Jericho A. Benitez** – renz_benitez@dlsu.edu.ph  
- **Jason Aaron L. Chua** – jason_aaron_chua@dlsu.edu.ph  
- **Dominic P. Yu** – dominic_yu@dlsu.edu.ph  
- **Miro Manuel L. Hernandez** – miro_hernandez@dlsu.edu.ph  

De La Salle University – Manila  
Department of Electronics and Computer Engineering  

## 📌 References
Full references are available in the [Final Project Report](./Design%20and%20Application%20of%20Automatic%20of%20an%20Coffee%20Maker%20with%20Coffee%20Ground%20and%20Water%20Dispenser%20using%20Arduino%20Uno%20R3-1.pdf).

