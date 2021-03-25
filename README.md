# particle_photon_pc_keyboard

Particle firmware for Photon to use it as a USB keyboard with MQTT support.

# Useful Features

## Login
My primary purpose for creating this was for the `/login` topic. The payload for this topic is a string for your pin. The sequence of key presses and pin entry has been designed to cover every scenario I've encountered for what state the Windows 10 lock screen might be in when trying to run the login sequence, and the timing of those events prevents pin entry from reaching whatever application you have running behind LoginUI (since it takes a moment for the login screen to close after successful pin entry). I use this to unlock my computer via Home Assistant scripts. 

## Tap
This provides the ability to perform single key presses. I intend to use this to create a virtual keyboard in Home Assistant.

# Getting Started

To get this up and running, you'll need to create your configuration file, then compile and flash the firmware onto the device. 

## Create Your Configuration

You'll need to create your own `config.h` in the `src/` folder modeled after `config.h.example` (which is also in the `src/` folder).  

The config requires you to provide:  
- your WiFi credentials (as well as optional alternate WiFi credentials for redundancy / easy of password changes)
- your MQTT server info

## Compile and Flash Firmware

Use Particle CLI to run these commands. Launch via workbench link.

Command to compile and flash via usb!
```
particle compile photon --saveTo firmware.bin && particle usb dfu && particle flash --usb firmware.bin
```

If the device fails to flash it could be because of a programming error that has left the device waiting for an event that will never occur. In such situations where the device never flashes yellow (indicating it has entered dfu mode to be flashed), you'll need to put it in dfu mode manually by following these steps:

1. Hold `Setup` + `Reset` on the Photon.  
2. Release `Reset` (but continue to hold `Setup`).  
3. Wait until it blinks yellow.
4. Release `Setup`.  

## Viewing Logs

If anything is not working, you might want to start with connecting to the device to view the logs.

Use this in a separate Particle CLI terminal to start viewing logs:  
```
particle serial monitor --port COM6 --follow
```
The `--follow` allows you to connect and then have it reconnect right away after the device is flashed so you can view logs from the initial code that runs to get things set up.  
Uncomment this line at the beginning of setup() so you dont miss any logs while the monitor reconnects:
```
delay(4000); //wait 4 seconds
```

# License

Copyright (c) 2021 Nathan Fast

This project is licensed under the MIT license. See the `LICENSE` file for more details.