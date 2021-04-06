# User Guide for power amplifier PTT

## Table of Contents

1. [Introduction](#introduction)
1. [What You'll Need](#what-youll-need)
1. [Install necessary parts](#install-necessary-parts)
    * [USB/serial Cable](#usbserial-cable)
    * [Out-of-Tree (OOT) Modules](#out-of-tree-oot-modules)
    * [Hier Blocks](#hier-blocks)
1. [How to view and use](#how-to-view-and-use)

## Introduction

This user guide aims to provide users with all of the information needed to use the power amplifier PTT blocks. This guide assumes the user knows how to navigate GNU Radio Companion (GRC) and Linux command terminals. Some basic tutorials for GNU Radio are available [here](https://wiki.gnuradio.org/index.php/Tutorials) or on slides 24-27 [here](https://docs.google.com/presentation/d/145syBke3wD0GXqM9OnpUmSf0r15e0uf7wZKPRpoonRI/edit?usp=sharing).

## What You'll Need

These are the parts needed to use the power amplifier PPT blocks.

* Computer running Linux (Ubuntu 20.04 was used in creation of this user guide) 
* An install of GNU Radio 3.8.2 from source (install guide available [here](https://gitlab.com/ORCASat/ttc/sdr-ground-station/-/blob/master/USER_GUIDE.md))
* A USB/serial cable (with patch cables if needed)
* Oscilloscope with probes (optonal for viewing state of PTT)

## Install necessary parts

These are links to all of the necessary programs and resources for the power amplifier PPT along with instructions on how to install them.

### USB/serial Cable

Allow USB/serial cable file in Linux to be accessed without admin permissions.
```
sudo usermod -a -G dialout $USER
```

Bind USB/serial cable to a static name. This is done since USB/serial cables are given a name, such as "ttyUSB0", when plugged in which is not reserved to that device so a static name must be implemented manually.

<div align="center">

![](/images/cable_ids.png)

Where the idVendor and idProduct are located in the "lsusb" command output.

<div align="left">

Find the idVendor and idProducts of the USB/serial cable being used, idVendor is number left of colon after "ID", idProduct is number right of colon after "ID"
```
lsusb
```
Create a file to store your static device name
```
sudo touch /etc/udev/rules.d/99-usb-serial.rules
```
Edit the file that was created, any text editor can be used
```
sudo vi /etc/udev/rules.d/99-usb-serial.rules
```
Press "i" to enter insert mode in Vim then right click > paste in the following data, substituting the proper idVendor and idProduct (HIGHLY recommended the substitution is not done in Vim because Vim is a pain). The SYMLINK will be the static name of the device, the default name is the default name used by the "Power Amplifier PTT" hier block.
```
SUBSYSTEM=="tty", ATTRS{idVendor}=="xxxx", ATTRS{idProduct}=="xxxx", SYMLINK+="ptt"
```
Press "Esc" to exit insert mode. Press ":wq" then press "Enter" to save and exit Vim.

Additional resource on how to set a static device name [here](https://unix.stackexchange.com/questions/66901/how-to-bind-usb-device-under-a-static-name) at the second answer.

(OPTIONAL) If there is a Vagrant VM from the Transceiver POC Firmware repository running, it may accidentally take the USB/serial cable being used for power amplifier PTT. This can be fixed by adding vendorid and productid tags for the USB/serial cable being used for UART communication in the Vagrantfile.

Find the vendorid and productid of the USB/serial cable being used. This is the same as finding the idVendor and idProduct.
```
lsusb
```
Open ~/tranceiver-poc-firmware/Vagrantfile. Replace the last block of code before the "end"s, which is the USB/serial cable filter, substituting the "#"s for the vendorid and productid of the USB/serial cable found in the previous step.
```
# Add USB device filter to VM to capture USB/serial cable
    vb.customize ["usbfilter", "add", "0",
      "--target", :id,
      "--name", "TTL232R-3V3",
      "--vendorid", "0x####",
      "--productid", "0x####"]
```
If "vagrant up" has already been run, open the VirtualBox application to the Vagrant VM and go Settings > USB > click "Remove selected USB filter" (USB port with red X) repeatedly until all USB filters have been removed, then exit by pressing "OK".

Apply the new USB device filter
```
vagrant up
or
vagrant reload
```

### Out-of-Tree (OOT) Modules

Install commands for the OOT module gr-ampkey. The OOT module is in a directory denoted by the prefix "gr-". If you make changes to the OOT modules make sure you delete the ~/build directory before pushing to this repository. This is because that directory is specific to your computer and the OOT module building process won't work on another persons computer if they are using the ~/build directory from another computer.

Install power amplifier PTT OOT module
```
git clone https://gitlab.com/ORCASat/ttc/power-amplifier-ptt.git
```
Each OOT module must be installed into GNU Radio. They must also be updated if any edits are made to them. These are both done from a command terminal located at the desired OOT module's directory. After editing or updating an OOT module, GRC must be restarted to apply the changes that were made.

Install/Update gr-ampkey for GNU Radio
```
cd power-amplifier-ptt/gr-ampkey
./build.sh
```
Installing the OOT module can be done manually using the following commands in the ~/power-amplifier-ptt/gr-ampkey directory.
```
mkdir build
cd build
cmake ..
make
sudo make install
sudo ldconfig
```
Updating the OOT module can be done manually using the following commands in the ~/power-amplifier-ptt/gr-ampkey/build directory.
```
cmake ..
make
sudo make install
sudo ldconfig
```

### Hier blocks

To make the hier block named "Power Amplifier PTT" of this OOT module available, in GRC open the ~/power-amplifier-ptt/gr-ampkey/examples/amp_key_hier.grc flowgraph and click the "Generate the flow graph" button on the upper toolbar. Then open a flowgraph and the "Power Amplifier PTT" hier block should be present in the "GRC Hier Blocks" tab on the right side of GRC.

To view the flowgraph of the "Power Amplifier PTT" hier block, right click on it and click More > Open Hier.

## How to view and use

These are instructions on how to use the "Power Amplifier PTT" hier block and view the changing state of the RTS# pin of a USB-to-serial converter.

To use the "Power Amplifier PTT" hier block add it to the desired flowgraph, set the editable parameters, and connect it directly before the Tx Sink (ie "UHD: USRP Sink", "osmocom Sink") of the flowgraph.

Plug the USB/serial cable into the computer being used. Connect the probes of the oscilloscope to the SDR# and GND pins of the USB/serial cable. The SDR# pin will be at 3.3V while idle and 0V for pre_tx milliseconds before data transmission, during data transmission, and for post_tx milliseconds after data transmission. Set up the appropriate oscilloscope triggers to view when the state of the SDR# pin switches.

<div align="center">

![](/images/ampkey_hier_block.png)

How the "Power Amplifier PTT" hier block should be connected to the flowgraph.

![](/images/flowgraph_ampkey_hier_block.png)

Viewing the flowgraph of the "Power Amplifier PTT" hier block.

<div align="left">
