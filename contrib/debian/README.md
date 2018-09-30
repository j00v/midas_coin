
Debian
====================
This directory contains files used to package midasd/midas-qt
for Debian-based Linux systems. If you compile midasd/midas-qt yourself, there are some useful files here.

## midas: URI support ##


midas-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install midas-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your midasqt binary to `/usr/bin`
and the `../../share/pixmaps/midas128.png` to `/usr/share/pixmaps`

midas-qt.protocol (KDE)

