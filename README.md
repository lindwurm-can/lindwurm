


# Lindwurm

Lindwurm is an open source cross-platform tool based on C++ and the Qt framework. It is a CAN bus tracing and fuzzing tool especially aimed for penetration testing.

## About

When it comes to automotive development, there exists a multitude of professional grade CAN development tools, such as the quasi industry standard Vector CANoe. These tools may suite well for a developer working with the given specification. However, the workflow of these tools is anything but ideal for penetration testers.

Working as a penetration tester I have quite a lot of experience using Burp Suite for web application tests. Thereby I've come to appreciate the individual tools of Burp Suite and their workflow. In preparation for a penetration test of an ECU I was conducting, I started hacking a small CAN tracing and sending application. Over time, I considered implementing some of it's features with a workflow similar to Burp and also adapting a frame filtering feature like Wireshark.

Based on this prototype I started Lindwurm as an attempt to build a fully functional and operable tool for automotive CAN penetration tests.

## Alpha release

This is a first public early *alpha* release of Lindwurm. Expect that functions are buggy and erroneous or not yet fully implemented. Keep in mind if you using it on a *real* car!

Although Lindwurm should be cross-platform, it is so far only tested with Qt 5.15.2 on Linux. Even if it will start on other platforms, some functions might not work as expected.

To see CAN frames sent with the composer in the tracer, *local echo* must be enabled. However, some of Qt's CAN driver plugins do *NOT* implement loopback mode (e.g. TinyCAN or PeakCAN). Using them as a SocketCAN device on Linux is a workaround.

## Build instructions

### Using qmake from command line:

```
git clone https://github.com/lindwurm-can/lindwurm.git
mkdir build
cd build
qmake ../lindwurm/Lindwurm.pro
make
```

Or use *Qt Creator* to open the project.


## Quick Start

#### Create virtual socketcan interface
```
sudo ip link add dev vcan0 type vcan
sudo ip link set up vcan0
```
#### Start Lindwurm
```
cd bin
./lindwurm
```

## Website

https://www.lindwurm-can.org/

## License

This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.

## Contact

Feel free to contact me at sascha@lindwurm-can.org if you have questions or want to contribute.

## Disclaimer

There is no warranty, expressed or implied, associated with this product. Use at your own risk. Especially when using on a real car.
