# FileSystems_MRND

## OVERVIEW:

This is a simple implementation of File Systems. This was developed using a 100 MB HardDisk.  This uses a block structure with direct and indirect implementations for memory locations (With top of the memory being the Meta Data). This handles external fragmentation by having scope for non-continuous allocation of blocks and internal fragmentation with small block size (Block size can be initialized by user.)   

Hard Disk Link : https://drive.google.com/open?id=16ZStufxgXD7CS2p8GxvR_GzHqrjZ2AVY

## COMMANDS:

Command to create HardDisk : fsutil file createnew \<hddName\>.hdd \<size in bytes\>
  
  ### File System commands
  

MOUNT \<harddiskfilename\> \<blockSize\> - should use the given file as the disk.

ex: MOUNT harddisk.hdd 16384

 

COPYTOFS \<sourceFileName\> \<destFileName\> - should upload the file which is present in the machine's file system to your file system.

ex: COPYTOFS dog.jpg puppy.jpg

 

COPYFROMFS <sourceFileName> <destFileName> - should download the file from your file system to machine's file system.

ex: COPYFROMFS puppy.jpg dogfromfs.jpg

 

FORMAT \<blockSize\> - should format your file system with the given block size.

ex: FORMAT 16384

 

LS - should list all the files present in your file system.

 

DELETE \<fileName\> - should delete the file specified from your file system.

 

DEBUG - should list all the files with all their details and also details like available space, occupied space and total space of your file system.

 

UNMOUNT - unmount the current disk.

 

EXIT - stop the program.
