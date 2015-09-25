# -*- coding: utf-8 -*-
"""
Created on Sun Dec  7 18:25:48 2014

@author: kjayasurya
"""

import basis
import numpy as np
import scipy.misc as smisc
import os, warp, fnmatch
import imageproc as ip
import kmeans
import pca
import texture
import gnu
import sophia
from scipy.signal import cspline2d, correlate2d


def MakeHaar():
    res = 512
    haar_set = []
    swtch = 1
    while res > 4:
        swtch = 1
        haar = np.zeros((512,512))
        vm = res
        hm = res
        swtchj = swtch+0
        while vm <= 512:
            swtch = swtchj+0
            hm = res
            while hm <= 512:
                haar[vm-res:vm,hm-res:hm] = swtch+0
                if swtch == 1:
                    swtch = -1
                else:
                    swtch = 1
                    
                hm = hm+res
            haar[vm:-1,hm:-1] = swtch+0
            vm = vm+res
            if swtchj == 1:
                swtchj = -1
            else:
                swtchj = 1
        res = res/2
        haar_set.append(haar)
    haar_set = haar_set[1:-1]
    return haar_set

def CorrHaar(imgset, haars):
    corrs = []
    for mg in imgset:
        corr = map(lambda x: np.array(sophia.Correlate(mg,x,1)), haars)       #Correlating with 6 filters
        corr = np.array(corr) 
        corr = corr.sum(2)                                                  #Summing up the output of 6 filters
#        corr = corr/corr.max()                                              #Normalizing the resulting 512 x 512 picture
#        
        #Considering only the real parts as the complex parts are very small. Don't know if this is correct!
        corrs.append(corr)
    corrs = np.array(corr)
    return corrs

def TestHaar(nmgs):
    haars = MakeHaar()
    corrs = []
    for mg in nmgs:
        corr = map(lambda x: np.array(sophia.Correlate(mg,x,1)),haars)
        corrs.append(corr)
    corrs = np.array(corrs)
    corrs.shape
    b = corrs.sum(1)
    a = b.sum(2)
    return corrs,a

def ReadFiles(indir):
    #Listing all files in the data directory to find image files and fiducial files
    fname_list = []
    for root, subFolders, files in os.walk(indir):
        for filename in fnmatch.filter(files, '*.jpg'):
            fname_list.append( os.path.join(root,filename))
    return fname_list

def TestLaws( mgnames, NJ = 100 ):

    # create laws filters
    filts = texture.BuildLawsFilters()
    # allocate for jets
    NI = len( mgnames ) # number of images
    jets = np.zeros( (NJ*NI, 25 ))
    # for each image
    for i in xrange( NI ):
        # load
        # correlate
        #corrs = BruteCorrelate( data, filts )
        data = mgnames[i]+0
        corrs = map( lambda x: correlate2d( data, x ), filts )
        for j in range( 25 ):
            corrs[i] = cspline2d( abs(corrs[i]), 200 )
        corrs = np.array( corrs )
        # extract random jets
        V,H = data.shape
        vs = range( V )
        hs = range( H )
        np.random.shuffle( vs ); np.random.shuffle( hs )
        for j in range( NJ ):
            jets[i*NJ + j] = corrs[:,vs[j], hs[j] ]
    # k-means clustering
    clust, mmb = kmeans.KMeans( NI, jets )
    #return jets
    cffs,evecs = pca.PCA(clust,3)
    cffs = pca.Map2PCA(clust,evecs)
    gnu.Save('Laws_results.txt',cffs)
    return clust,cffs

############Program Start#######################
fnames = ReadFiles('data')
mgs = map(lambda x: basis.LoadGray(x), fnames)

nmgs=[]
for i in range(len(mgs)):
    nm = ip.Plop(mgs[i],512,512)
    nmgs.append(nm)

fids = map(lambda x: basis.MakeGrid(x),nmgs)
fids = np.array(fids)
morph = warp.Morph(nmgs[1],fids[1],nmgs[2],fids[2],NI=10)
Imageset = []
Imageset.append(nmgs[1])

for i in morph:
    Imageset.append(i)
Imageset.append(nmgs[2])

for i in range(len(Imageset)):
    nm = 'output\\morph_'+str(i)+'.png'
    smisc.imsave(nm,Imageset[i])

clust,cffs = TestLaws(Imageset)

qvecs,a = TestHaar(Imageset)
qvecs = qvecs.sum(3).sum(2)
cffs,evecs = pca.PCA(qvecs,3)

cffs  = pca.Map2PCA(qvecs,evecs)
gnu.Save('Haar_results.txt',cffs)