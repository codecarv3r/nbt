#libnbt
A library for reading and writing to the Named Binary Tag specification created by @notch for Minecraft

##Why
A c99 library called [cNBT](https://github.com/FliPPeh/cNBT) for manipulating data in the NBT format is available, but is somewhat incomplete. It does not provide a way to easily manipulate the in-memory data structure, instead making the data structure public to the user. Also, it does not support NBT data in the Little Endian byte order, so it cannot be easily used for Minecraft PE data. My hope in creating this library is to make it easier to create applications which can edit Minecraft and Minecraft PE data.

##Currently Implemented Features
* Parsing of NBT data (uncompressed)
* Tree manipulation
* Printing with indentation
* Printing with pipes
* A very basic application which will print a file's nbt tree

##Future Features
* Write from in-memory tree to binary data
* Compression support (both read and write)
* Consistant API
* Graceful failure for corrupt data
* Colorful print style
* Command line NBT data editor

##Prerequisites
* A compiler which supports c99

I use Xcode to build libnbt and have included the Xcode project, but Xcode is not required.
