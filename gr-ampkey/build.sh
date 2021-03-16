#!/bin/sh -x
# run uninstall target
# if [ -d "build" ]; then
#	cd build
#	sudo make uninstall
#	cd ..
#	sudo rm -rf build
# fi

# manually remove rogue shared objects (not
# doing this step will not affect the build,
# but there may be runtime Python import issues)
# sudo rm -f /usr/local/lib/libgnuradio-ampkey.so*
# sudo rm -rf /usr/local/lib/python3/dist-packages/ampkey

# build and install
mkdir build
cd build
cmake ..
make
sudo make install
sudo ldconfig
