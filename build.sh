#!/bin/bash
gnome-terminal -e '/bin/bash -c "mkdir build; cd build/; cmake ..; make; make install; ldconfig; exec bash;" ' 
