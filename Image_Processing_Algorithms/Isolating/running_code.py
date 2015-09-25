# -*- coding: utf-8 -*-

import numpy as np
import scipy.misc as smisc
import scipy.ndimage as nd
import os,sys,fnmatch
import imageproc as ip
import Image

def ReadImages(directory):
    mgfilenames = []
    mgfiles = []
    for root, subFolders, files in os.walk(directory):
        for filename in fnmatch.filter(files, '*fa.jpg'):
            mgfilenames.append(os.path.join(root,filename))

    for mgs in mgfilenames:
        img = Image.open(mgs).convert('YCbCr')
        mgfiles.append(img)
    return mgfiles

def IsolateFaces(mgfiles):
    isolatedmgs = []
    for mgs in mgfiles:
        y,cb,cr = mgs.split()
        mg_cb = np.array(cb)
        mg_cr = np.array(cr)
        mask_cb_1 = mg_cb < 120
#        mask_cb_2 = mg_cb > 112
        mask_cr_1 = mg_cr > 130
#        mask_cr_2 = mg_cr < 145
#        mask1 = np.multiply(mask_cb_1,mask_cb_2)
#        mask2 = np.multiply(mask_cr_1,mask_cr_2)
        mask = np.multiply(mask_cb_1,mask_cr_1) #mask1,mask2 #mask_cb_1,mask_cr_1
        mask = nd.binary_fill_holes(mask)
        isolatedmgs.append(mask)
    return isolatedmgs


def dispMenu(n):
    print '\n\nTotal images processed: ' + str(n)
    num = input( 'Enter the image number to display: ')
    return int(num-1)
    

directory = 'faces'

print('\nReading Images\n')
mgfiles = ReadImages(directory)

#Isolating the skin colors and face from the image
print('\nIsolating Faces in Images\n')
isomgs = IsolateFaces(mgfiles)

choice = '12'
while choice != 0:
    num = dispMenu(len(isomgs))
    if num == -1:
        break;
    elif (num < len(isomgs) and num > -1):
        smisc.imshow(mgfiles[num])
        smisc.imshow(isomgs[num])
    else:
        print '\nPlease enter a valid number\n'
