# Function Generator Using 8051 Microcontroller

<div align="center">

![8051](https://img.shields.io/badge/Microcontroller-8051-blue.svg)
![C](https://img.shields.io/badge/Language-C-green.svg)
![Hardware](https://img.shields.io/badge/Type-Embedded%20System-red.svg)
![Status](https://img.shields.io/badge/Status-Complete-brightgreen.svg)

*A versatile signal generator capable of producing multiple waveforms with user-configurable frequency*

</div>

## 📋 Overview

This project implements a comprehensive function generator using the 8051 microcontroller, capable of generating four distinct waveforms: **Sine**, **Square**, **Triangle**, and **Sawtooth** waves. The system features an intuitive keypad interface for frequency input and waveform selection, with real-time display on an LCD screen.

<img width="1231" height="507" alt="image" src="https://github.com/user-attachments/assets/d3090a3d-e8f8-4d66-9eb2-a577c0b5e79f" />

## ✨ Features

- **🌊 Multiple Waveforms**: Sine, Square, Triangle, and Sawtooth wave generation
- **🔢 Frequency Control**: User-configurable frequency input via 4x4 keypad
- **📺 LCD Display**: Real-time parameter display and user interface
- **🎛️ DAC Integration**: High-quality analog output using DAC0808
- **⚡ Real-time Generation**: Immediate waveform output upon parameter selection
- **🔄 Interactive Interface**: Intuitive keypad navigation and control

## 🛠️ Hardware Components

| Component | Description | Purpose |
|-----------|-------------|---------|
| **AT89C51** | 8051 Microcontroller | Main processing unit |
| **16x2 LCD** | Character Display | User interface and parameter display |
| **DAC0808** | 8-bit Digital-to-Analog Converter | Analog signal generation |
| **4x4 Keypad** | Matrix Keypad | Frequency input and waveform selection |
| **Op-Amp (LM741)** | Operational Amplifier | Signal conditioning and amplification |
| **Crystal Oscillator** | 11.0592 MHz | System clock generation |

## 🔌 Pin Configuration

### LCD Interface (Port 3)
- **RS** → P3.2
- **RW** → P3.3  
- **E** → P3.4
- **Data** → Port 0

### Keypad Interface (Port 1)
- **Columns** → P1.0 - P1.3
- **Rows** → P1.4 - P1.7

### DAC Output
- **DAC** → Port 2

## 🎯 Waveform Specifications

| Waveform | Key | Description |
|----------|-----|-------------|
| **Sine** | Column 4, Row 1 | Smooth sinusoidal wave using 50-point lookup table |
| **Square** | Column 4, Row 2 | Digital square wave with 50% duty cycle |
| **Triangle** | Column 4, Row 3 | Linear ramp up/down triangular wave |
| **Sawtooth** | Column 4, Row 4 | Linear ramp with sharp reset |

## 🚀 How to Use

1. **Power On**: System displays "FUNCTION GENERATOR"
2. **Select Waveform**: Press corresponding key from Column 4
3. **Enter Frequency**: Use numeric keypad (0-9) to input desired frequency in Hz
4. **Generate**: Press 'e' key to start waveform generation
5. **Output**: Analog signal available at DAC output (Port 2)
<img width="1397" height="787" alt="image" src="https://github.com/user-attachments/assets/daa607ec-5e4d-4768-a9c1-808fcd60089a" />

### Keypad Layout
```
[1] [2] [3] [Sine]
[4] [5] [6] [Square]  
[7] [8] [9] [Triangle]
[e] [0] [#] [Sawtooth]
```

## 💻 Software Features

- **Real-time Processing**: Efficient timer-based waveform generation
- **Memory Optimization**: Compact sine wave lookup table (50 points)
- **Error Handling**: Input validation and boundary checks
- **Modular Design**: Separate functions for each waveform type
- **User-Friendly Interface**: Clear LCD prompts and feedback

## 📊 Technical Specifications

- **Frequency Range**: 1 Hz to 9999 Hz
- **Resolution**: 8-bit (256 levels)
- **Timer**: 16-bit Timer 0 for precise timing
- **Clock Frequency**: 11.0592 MHz
- **Output Voltage**: 0-5V (configurable with op-amp)

## 🔧 Development Environment

- **IDE**: Keil µVision
- **Language**: Embedded C
- **Compiler**: C51 Compiler
- **Simulation**: Proteus Design Suite
- **Programming**: ISP Programmer

## 🎓 Learning Outcomes

- Microcontroller programming and interfacing
- Digital-to-Analog conversion techniques
- Timer programming for signal generation
- LCD and keypad interfacing
- Signal processing fundamentals
- Embedded system design principles

## 🚀 Future Enhancements

- [ ] Extended frequency range with prescaler
- [ ] Amplitude control functionality
- [ ] Phase shift implementation
- [ ] Duty cycle control for square waves
- [ ] Memory storage for preset configurations
- [ ] PC interface for advanced control

## 👨‍💻 Author

**Het Patel**  
📧 hetkumarpatel07@gmail.com  
🎓 Electronics Engineering Student  

## 📄 License

This project is open source and available under the [MIT License](LICENSE).

---

<div align="center">

**⭐ If you found this project helpful, please give it a star! ⭐**

*Built with ❤️ and lots of ☕*

</div>


<div align="center">

![Footer](https://capsule-render.vercel.app/api?type=waving&color=gradient&height=100&section=footer)

</div>
