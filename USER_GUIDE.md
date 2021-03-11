# User Guide for power amplifier PTT

## Table of Contents

1. [Introduction](#introduction)
1. [What You'll Need](#what-youll-need)
1. [Install necessary parts](#install-necessary-parts)
    * [Out-of-Tree (OOT) Modules](#oot-module)
    * [Hier Blocks](#hier-blocks)
1. [How to view and use](#how-to-view-and-use)

## Introduction

This user guide aims to provide users with all of the information needed to use the power amplifier PTT blocks. This guide assumes the user has some previous experience with GNU Radio Companion (GRC) and GNU Radio. Some basic tutorials for GNU Radio [here](https://wiki.gnuradio.org/index.php/Tutorials) or on slides 24-27 [here](https://docs.google.com/presentation/d/145syBke3wD0GXqM9OnpUmSf0r15e0uf7wZKPRpoonRI/edit?usp=sharing).

## What You'll Need

These are the parts needed to use the power amplifier PPT blocks.

* Computer running Linux that can run GNU Radio 3.8.2
* A USB-to-serial converter with patch cables
* Oscilloscope with probes (optonal for viewing state of PTT)

## Install necessary parts

These are links to all of the necessary programs and resources for the power amplifier PPT along with instructions on how to install them.

### Out-of-Tree (OOT) Modules

Install commands for the OOT module gr-ampkey. If you make changes to the OOT modules in this repository, make sure you delete the ~/build directory from the OOT module before pushing to this repository.This is because this directory is specific to your computer and won't work on another persons computer.

Install power amplifier PTT resources
```
$ git clone https://gitlab.com/ORCASat/ttc/power-amplifier-ptt.git
```
Each OOT module must be installed into GNU Radio. They must also be updated if any edits are made to them. These are both done from a terminal located at the desired OOT module's directory. After editing and updating an OOT module, GRC must be restarted to apply the changes made.

Both the installing and updating can be done with a custom shell script using the following commands in ~/power-amplifier-ptt/gr-ampkey.
```
$ ./build.sh
```
Installing can also be done manually using the following commands in ~/power-amplifier-ptt/gr-ampkey.
```
$ mkdir build
$ cd build
$ cmake ..
$ make
$ sudo make install
$ sudo ldconfig
```
Updating the OOT module can be done manually by using the following commands ~/power-amplifier-ptt/gr-ampkey/build.
```
cmake ..
make
sudo make install
sudo ldconfig
```

### Hier blocks

To make the hier block named "Amp Key" of this OOT module available, in GRC open ~/power-amplifier-ptt/gr-ampkey/examples/amp_key_hier.grc and click the "Generate the flow graph" button on the toolbar. Then open a flowgraph and "Amp Key" should be present in the "GRC Hier Blocks" tab on the right side of GRC.

To view the flowgraph of "Amp Key, right click on it and go More > Open Hier.

## How to view and use

These are instructions on how to use the "Amp Key" hier block and view the changing state of the RTS# pin of a USB-to-serial converter.

To use the "Amp Key" hier block add it to the desired flowgraph, set the editable parameters, and connect it directly before the Tx Sink ("UHD: USRP Sink", "osmocom Sink") of the flowgraph.

Plug the USB-to-serial converter into the computer being used. Connect the probes of the oscilloscope to the SDR# and GND pins of the USB-to-serial converter and set up the appropriate triggers to view when the state of the SDR# pin switches.

If a flowgraph with the "Amp Key" hier block crashes and the block stops working, restarting the computer being used is advised.

<div align="center">

![](/images/ampkey_hier_block.png)

The "Amp Key" hier block how it should be connected to the flowgraph.

![](/images/inside_ampkey_hier_block.png)

Viewing inside the "Amp Key" hier block.

<div align="center">
