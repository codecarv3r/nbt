#libnbt
A library for reading and writing to the Named Binary Tag specification created by @notch for Minecraft

##Why
A c library called [cNBT](https://github.com/FliPPeh/cNBT) for manipulating data in the NBT format is available, but is somewhat incomplete. It does not provide a way to easily manipulate the in-memory data structure, instead making the data structure public to the user. Also, it does not support NBT data in the Little Endian byte order, so it cannot be easily used for Minecraft PE data. My hope in creating this library is to make it easier to create applications which can edit Minecraft and Minecraft PE data.

##Currently Implemented Features
* Parsing of NBT data (uncompressed)
* Tree manipulation
* Printing with indentation
* Printing with pipes
* A very basic application which will print a file's nbt tree

##Prerequisites
* A compiler which supports c99

I use Xcode to build libnbt and have included the Xcode project, but Xcode is not required.