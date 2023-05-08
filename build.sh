#!/usr/bin/bash/env bash

if [ "x$(uname -rv | grep Darwin)" != "x" ];
then
	if [ "x$(which brew)" == "x" ];
	then
		echo "Install homebrew (https://brew.sh)."
		exit 1
	fi

	brew install binutils gcc
	exit 0
fi

DIST=$(uname -rv)

case ${DIST,,} in
	*"ubuntu"*|*"debian"*)
		apt-get install -y gcc binutils make
		;;
	*"arch"*|*"majaro"*)
		pacman -S gcc binutils make --need -noconfirm
		;;
	*)
		echo "Distro not found: Need to install gcc (any C compiler) and make"
esac
