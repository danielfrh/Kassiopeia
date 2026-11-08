#!/usr/bin/env python3


# Analyzing fields from Kassiopeia ROOT input
# purpose: analyse fields along calaculated field lines

import sys
import os

# import module from Kasper
sys.path += [ os.environ['KASPERSYS'] + '/lib/python', os.environ['KASPERSYS'] + '/lib64/python' ]
import KassiopeiaReader

if len(sys.argv) < 2:
    raise RuntimeError("No input file provided.")

# create reader instance
reader = KassiopeiaReader.Iterator(sys.argv[1])

# load step-data tree
reader.loadTree('component_step_world_DATA')
print("#tracks:", len(reader.getTracks('TRACK_INDEX')))
#print('#steps: ', len(reader))
#print('fields: ', dir(reader))
