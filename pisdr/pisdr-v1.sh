#!/bin/bash

# This bash script installs all needed tools for using
# RTL2832U based DVB-T tuners as software defined
# receivers (SDR) on Debian/Ubuntu based systems
#
#	librtlsdr - basic library needed to communicate with RTL2832U  
#	dump1090 - ADS-B message decoder with web page google maps display of aircraft locations
#	multimon-ng - RF demodulator (can be used for decoding APRS packets)


install_deps()
{
	echo "Installing build dependencies"

	build_deps="
		libusb-1.0-0-dev
		build-essential
		libqt4-dev
		cmake
		make
		git
		libpulse-dev
	"

	sudo apt-get update

	for p in ${build_deps}
	do
		sudo apt-get install $p
	done
}

install_librtlsdr()
{
	echo "Downloading librtlsdr sources"

	cd sdr_sources
	git clone git://git.osmocom.org/rtl-sdr.git
	cd rtl-sdr 
	mkdir build
	cd build

	echo "Compiling librtlsdr"
	cmake ../ -DINSTALL_UDEV_RULES=ON
	make

	echo "Installing librtlsdr"
	sudo make install
	sudo ldconfig
	cd ../../
	echo "installed librtlsdr"

	echo "Disabling DVB-T kernel module"
	sudo echo "blacklist dvb_usb_rtl28xxu" > /etc/modprobe.d/blacklist-dvb_usb_rtl28xxu.conf
	sudo depmod -ae 
	sudo update-initramfs -u
}

install_dump1090()
{
	echo "Downloading dump1090 sources"
	git clone https://github.com/MalcolmRobb/dump1090
	cd dump1090
	echo "Compiling dump1090"
	make
	cd ../
	echo "Done compiling dump1090"
}

install_multimonNG()
{
	echo "Downloading multimon-ng sources"
	git clone https://github.com/EliasOenal/multimon-ng
	cd multimon-ng
	mkdir build
	cd build
	
	echo "Compiling multimon-ng"	
	qmake ../multimon-ng.pro
	make
	
	echo "Installing multimon-ng"
	sudo make install
	cd ../../
	echo "Installed multimon-ng"
}

usage(){
	echo "This script will install all necessary software
to be able to use an RTL2832U based DVB-T stick
as a software defined receiver (SDR)"
	echo "Usage: "	
	echo "	./pisdr.sh --install (installs librtlsdr, dump1090, multimon-ng)"
	echo "	./pisdr.sh --install-deps (only installs build dependencies)"
	echo "	./pisdr.sh --help (displays this message)"
	exit
}

if [ $# -eq 1 -a "$1" == "--install" ]; then
	mkdir sdr_sources
	install_deps
	install_librtlsdr
	install_dump1090
	install_multimonNG
	exit
fi

if [ $# -eq 1 -a "$1" == "--install-deps" ]; then
	install_deps
	exit
fi

if [ $# -eq 1 -a "$1" == "--help" ]; then
	usage
	exit
fi

usage
