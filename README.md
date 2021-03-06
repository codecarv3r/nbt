# libnbt
A library for reading and writing to the Named Binary Tag specification created by @notch for Minecraft

## Why
A c99 library called [cNBT](https://github.com/FliPPeh/cNBT) for manipulating data in the NBT format is available, but is somewhat incomplete. It does not provide a way to easily manipulate the in-memory data structure, instead making the data structure public to the user. Also, it does not support NBT data in the Little Endian byte order, so it cannot be easily used for Minecraft PE data. My hope in creating this library is to make it easier to create applications which can edit Minecraft and Minecraft PE data.

## Currently Implemented Features
* Parsing of NBT data (both compressed and uncompressed)
* Write from in-memory tree to binary data
* Tree manipulation
* Printing with indentation
* Printing with pipes
* Printing with colors
* A very basic application which will print a file's nbt tree
* Compression support (both read and write)

## Future Features
* Consistant API
* Graceful failure for corrupt data
* Command line NBT data editor

## Instructions
* Clone the repository
* Open the Xcode project
* Click the "Run" button

## Notes
* If you don't use Xcode, you should still be able to use the project, but you will have to find your own way to build
* I use zlib for compression
* I use libedit for prompting. To turn this off, simply swith USE_READLINE in edit.c to 0
