import os,sys,fnmatch
import scipy.misc as smisc
import numpy as np
import sophia
import pca
import Image
import basis
import gnu

def ReadBrodatz(indir):
    brodatz_fname = [];
    brodatz_mg_path=[];
    #Reading the files in Brodatz directory
    for root, subFolders, files in os.walk(indir):
        for filename in fnmatch.filter(files, '*.gif'):
            brodatz_mg_path.append(os.path.join(root,filename))
            brodatz_fname.append(filename)

    #Creating the list of images and creating 4 256X256 images
    brodatz_mgs_256 = [];
    for mg_fname in brodatz_mg_path:
        img = Image.open( mg_fname )
        mg = sophia.i2a(img.convert('L'))/255.0
        
        mg1 = mg[0:256,0:256]       #Top-Left corner
        mg2 = mg[0:256,-257:-1]     #Top-Right corner
        mg3 = mg[-257:-1,0:256]     #Bottom-Left corner
        mg4 = mg[-257:-1,-257:-1]   #Bottom-Right corner
        
        brodatz_mgs_256.append(mg1)
        brodatz_mgs_256.append(mg2)
        brodatz_mgs_256.append(mg3)
        brodatz_mgs_256.append(mg4)
        
    return brodatz_fname,brodatz_mgs_256
def Train(brodatz_f,brodatz_256_mgs ):
    
    #A container function that can be imported or run with user created arrays and filenames. 
    #Coded Just for convenience incase of code testing!!
    
    qvecs = map(lambda x: basis.GaborCorr(x),brodatz_256_mgs)
    qvecs = np.array(qvecs)
    cffs,evecs = pca.PCA(qvecs,3)
    cffs = pca.Map2PCA(qvecs,evecs)
    vecs_num = 0
    for name in brodatz_f:
        plot_vecs = cffs[vecs_num:vecs_num+4]
        vecs_num = vecs_num + 4
        gnu.Save('graph_output\\'+name[0:-4]+'.txt',plot_vecs)
    
    for i in brodatz_f:
        print i+'\n'
    
    ss = 'unset key\n'
    ss +='splot for [i=1:112] \'graph_output\\D\'.i.\'.txt\'\n'
    
    fp1 = open('plot_brodatz_256_textures.txt','w')
    fp1.write(ss)
    fp1.flush()
    return brodatz_f,cffs

brodatz_f,brodatz_256_mgs = ReadBrodatz('brodatz')

qvecs = map(lambda x: basis.GaborCorr(x),brodatz_256_mgs)
qvecs = np.array(qvecs)
cffs,evecs = pca.PCA(qvecs,3)
cffs = pca.Map2PCA(qvecs,evecs)
vecs_num = 0
for name in brodatz_f:
    plot_vecs = cffs[vecs_num:vecs_num+4]
    vecs_num = vecs_num + 4
    gnu.Save('graph_output\\'+name[0:-4]+'.txt',plot_vecs)

for i in brodatz_f:
    print i+'\n'

ss = 'unset key\n'
ss +='splot for [i=1:112] \'graph_output\\D\'.i.\'.plt\'\n'

fp1 = open('plot_brodatz_256_textures.txt','w')
fp1.write(ss)
fp1.flush()
