# General

This repository contains a simple Linux Kernel Module for a PCI to Parallel Port Adapter. It was made for educational purposes and doesn't intend to replace an existing driver!

# Content

- pci_parport.c: Sourcefile for LKM
- Makefile: To build and compile LKM

# Functionality

I have written this code for a PCI to Parallel Port Adapter to which I have connected a little board. On the Board a 7 Segment display is connected to the Data Out Pins. 5 DIP-Switches are connected to the Inputs of the port. The Linux Kernel Module reads in the state of the DIP-switches every 200ms and then sets the 7 segment display according to the DIP's value. 

But the purpose of this Linux Kernel Module is to show you how to access a PCI device and its IO space.

# Dependencies

Make sure, you have the kernel headers and binutils (make, gcc, ...) installed

# More information

For more information about the source code and the module, check out the following video:

- [Linux Kernel Module for a PCI device](https://youtu.be/9k0iGbSnUT0)
