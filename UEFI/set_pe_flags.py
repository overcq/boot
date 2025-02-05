#!/bin/python
import pefile

file_path = "fileloader"
pe = pefile.PE( file_path )
pe.FILE_HEADER.Characteristics |= 0x20
pe.OPTIONAL_HEADER.DllCharacteristics |= 0x100
pe.OPTIONAL_HEADER.CheckSum = 0
pe.OPTIONAL_HEADER.SizeOfStackReserve = 0x100000
pe.OPTIONAL_HEADER.SizeOfStackCommit = 0x1000
pe.OPTIONAL_HEADER.SizeOfHeapReserve = 0x100000
pe.OPTIONAL_HEADER.SizeOfHeapCommit = 0x1000
pe.OPTIONAL_HEADER.MajorSubsystemVersion = 1
pe.write( filename=file_path )
