# User Guide for power amplifier PTT

## Table of Contents

1. [Introduction](#introduction)
1. [What You'll Need](#what-youll-need)
1. [Install necessary parts](#install-necessary-parts)
    * [GNU Radio](#gnu-radio)
    * [Out-of-Tree (OOT) Modules](#oot-module)
    * [Hier Blocks](#hier-blocks)
1. [How to view and use](#how-to-view-and-use)

## Introduction

This user guide aims to provide users with all of the information needed to use the power amplifier PTT blocks. This guide assumes the user has some previous experience with GNU Radio Companion (GRC) and GNU Radio and is able to successfully install OOT modules for GNU Radio. Some basic tutorials for GNU Radio [here](https://wiki.gnuradio.org/index.php/Tutorials) or on slides 24-27 [here](https://docs.google.com/presentation/d/145syBke3wD0GXqM9OnpUmSf0r15e0uf7wZKPRpoonRI/edit?usp=sharing).

## What You'll Need

These are the parts needed to use the power amplifier PPT blocks.

* Computer running Linux that can run GNU Radio 3.8.2
* A USB-to-serial converter with patch cables
* Oscilloscope with probes (optonal for viewing state of PTT)

## Install necessary parts

These are links to all of the necessary programs and resources for the power amplifier PPT along with instructions on how to install them.

### GNU Radio

Install dependencies for GNU Radio.
```
$ sudo apt-get install liborc-0.4
$ sudo apt install swig
```
Set PYTHONPATH so OOT modules will be found by GNU Radio.

Determine the GNU Radio install prefix, output of the following command is \{your-prefix}.
```
$ gnuradio-config-info --prefix
```
Finding the Python version being used, "python#" in output of the following command is \{Py-version}
```
$ find {your-prefix} -name gnuradio | grep "packages"
```
In ~/.basrc and ~/.profile of home directory add following 2 lines at ending.
```
$ export PYTHONPATH={your-prefix}/lib/{Py-version}/dist-packages:$PYTHONPATH
$ export LD_LIBRARY_PATH={your-prefix}/lib:$LD_LIBRARY_PATH
```
Restart and open terminals after ~/.basrc and ~/.profile have been saved and check if the PYTHONPATH is saved.
```
$ echo $PYTHONPATH
```
Install GNU Radio from the maint-3.8 personal package archive (PPA).
```
$ sudo add-apt-repository ppa:gnuradio/gnuradio-releases-3.8
$ sudo apt-get update
$ sudo apt install gnuradio
```
Check if the correct version is installed.
```
$ apt-cache policy gnuradio
```
Some additional resources on how to install GNU Radio and fix errors

https://wiki.gnuradio.org/index.php/InstallingGR

https://wiki.gnuradio.org/index.php/ModuleNotFoundError#B._Finding_the_Python_library

### Out-of-Tree (OOT) Modules

Install command for gr-ampkey. If you plan on making changes to the repository it is recommended that you copy the gr-ampkey directory to another location and do editing and building there, then delete the build directory and copy the gr-ampkey directory back into the power-amplifier-ptt directory. Mainly so your build directory isnt accidentally uploaded to the repository.
```
$ git clone https://gitlab.com/ORCASat/ttc/power-amplifier-ptt.git
```
Each OOT module must be installed into GNU Radio initially. They must also be updated if any edits are made to them. These are both done from a terminal in the desired OOT module's directory.

Both the installing and updating can be done with a custom shell script using the following commands.
```
$ ./build.sh
```
Installing can also be done manually using the following commands from the desired OOT module's directory.
```
$ mkdir build
$ cd build
$ cmake ..
$ make
$ sudo make install
$ sudo ldconfig
```
Updating a module can be done manually by running the last 4 commands from the build directory in a terminal.

### Hier blocks

To make the hier block of this OOT module available, in GRC open /examples/amp_key_hier.grc and click the "Generate the flow graph" button. Then open a flowgraph and the hier block should be present in the "GRC Hier Blocks" tab on the right side of GRC.

To view the blocks that compose the hier block, right click on the hier block and go More > Open Hier.

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
