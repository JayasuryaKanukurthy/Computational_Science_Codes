import numpy as np
import scipy.misc as smisc
import scipy.ndimage as nd
import warp
import goeface as gf
import eigenimage as eigen
import gnu

def GetAverageFids(group):
    txtnames = gf.GetTxtNames(group)
    mgnames = gf.MakeImageNames(txtnames)
    fids = gf.LoadFidFiles(txtnames)
    mgs = gf.LoadImages(mgnames)
    avggrid = gf.AverageGrid(fids)
    avgface = gf.AverageFace(avggrid,fids,mgs)
    filename = group[5:]+'.jpg'
    average_faces.append(avgface)
    smisc.imsave('output/'+filename,avgface)
    return avggrid,avgface,mgs,fids

def GetWarpdImages(group,grid,mask,f):
    txtnames = gf.GetTxtNames(group)
    mgnames = []
    fanames = []
    warpedmgs = []
    for txt in txtnames:
        if f in txt:
            fanames.append(txt)
            mgnm = txt[:-3]+'jpg'
            mgnames.append(mgnm)
        else:
            continue
    fanames.sort()
    mgnames.sort()
    famgs = gf.LoadImages(mgnames)
    fafids = gf.LoadFidFiles(fanames)
    #return famgs,fafids
    #Warping all FA/FB Images to the average grid
    for i in range(len(famgs)):
        mg = mask*famgs[i]
        warped=warp.Warp(mg,fafids[i],grid,warp.RInf(fafids[i]))
        warpedmgs.append(warped[0])
    return warpedmgs, fanames

def MapFaces(emgs,warpd):
    vec = []
    for i in range(len(warpd)):
        vec.append(eigen.ProjectEigen(emgs,warpd[i]))
    return vec

def Distance(famgs,fanms,fbmgs,fbnms,eigenmg,name):
    vecfa = np.array(MapFaces(eigenmg,famgs))
    vecfb = np.array(MapFaces(eigenmg,fbmgs))
    #Writing files to plot array
    plotarray = np.array([vecfa[:,0:3],vecfb[:,0:3]])

    gnu.Save(name+'_fa.txt', plotarray[0] )
    gnu.Save(name+'_fb.txt',plotarray[1])
    
    ss = 'set terminal png\n'
    ss +='set output "'+name+'fa_fb.png"\n'
    ss +='set multiplot\n'
    ss +='set size 1,0.5\n'
    ss +='set origin 0,0.0\n'
    ss +='set view 0,0,1,2\n'
    ss +='splot "'+name+'_fa.txt", "'+name+'_fb.txt"\n'
    ss +='unset multiplot\n'
    ss +='unset size\n'
    ss +='unset origin\n'
    
    fp1 = open('plot'+name+'_fa_fb.txt','w')
    fp1.write(ss)
    fp1.flush()
    result=[]
    for fb in vecfb:
        distance = []
        for fa in vecfa:
            dist = np.sqrt(((fb-fa)**2).sum())
            distance.append(dist)
        idx = np.array(distance).argsort()
        result.append(list(idx))
    return result

groups = ('data/AsianFemale', 'data/WhiteMale')    
average_faces = []
avggrid = []
mgs_grp1 = []
mgs_grp2 = []
fids_grp1 = []
fids_grp2 = []
masked_faces = []
eigenfaces = []

avg,avf,mgs_grp1,fids_grp1 = GetAverageFids(groups[0])
avggrid.append(avg)
average_faces.append(avf)

avg1,avf1,mgs_grp2,fids_grp2 = GetAverageFids(groups[1])
avggrid.append(avg1)
average_faces.append(avf1)

mask = smisc.imread('mask.jpg')
mask = mask>1
mask = np.array(mask,dtype=int)


fa_mgs1,asian_fa_txtnm= GetWarpdImages(groups[0],avggrid[0],mask,'fa')
fb_mgs1,asian_fb_txtnm= GetWarpdImages(groups[0],avggrid[0],mask,'fb')

emgs1,evals1 = eigen.EigenImages(np.array(fa_mgs1))

fa_mgs2,white_fa_txtnm = GetWarpdImages(groups[1],avggrid[1],mask,'fa')
fb_mgs2,white_fb_txtnm = GetWarpdImages(groups[1],avggrid[1],mask,'fb')

emgs2,evals2 = eigen.EigenImages(np.array(fa_mgs2))
eigenmg1 = []
eigenmg2 = []

#Sorting the eigenvectors according to eigenvalues
for i in np.argsort(evals1)[::-1]:
    if evals1[i] >= 0:
        eigenmg1.append(emgs1[i])

#Sorting the eigenvectors according to eigenvalues
for i in np.argsort(evals2)[::-1]:
    if evals2[i] >= 0:
        eigenmg2.append(emgs2[i])

print('Keeping 5 eigen vectors for group 1 as well as group 2')

eigenmg1 = eigenmg1[0:5]
eigenmg2 = eigenmg2[0:5]

for i in range(5):
    smisc.imsave('output/'+groups[0][5:]+'_'+str(i)+'.png',eigenmg1[i])
    smisc.imsave('output/eface_group2'+groups[1][5:]+'_'+str(i)+'.png',eigenmg2[i])

vecs1 = np.array(MapFaces(eigenmg1, mgs_grp1))
vecs2 = np.array(MapFaces(eigenmg2, mgs_grp2))

plotarray= np.array([vecs1[:,0:3],vecs2[:,0:3]])

#Writing files to plot array
gnu.Save('AsianFemale.txt', plotarray[0] )
gnu.Save('WhiteMale.txt',plotarray[1])

ss = 'set terminal png\n'
ss +='set output "diff_in_groups.png"\n'
ss +='set multiplot\n'
ss +='set size 1,0.5\n'
ss +='set origin 0,0.0\n'
ss +='set view 0,0,1,2\n'
ss +='splot "AsianFemale.txt","WhiteMale.txt"\n"'
ss +='unset multiplot\n'
ss +='unset size\n'
ss +='unset origin\n'

fp1 = open('plotaf.txt','w')
fp1.write(ss)
fp1.flush()

#Comparing fa images and fb images
comparision_scores_af = Distance(fa_mgs1,asian_fa_txtnm,fb_mgs1,asian_fb_txtnm,eigenmg1,'AsianFemale')
comparision_scores_wm = Distance(fa_mgs2,white_fa_txtnm,fb_mgs2,white_fb_txtnm,eigenmg2,'WhiteMale')

choice = 1
while(choice != 0):
    print 'Choose Ethnic Group:\n1.AsianFemale \n2.WhiteMale\n0.Exit'
    choice = input('Enter Choice: ')
    if choice == 1:
        print 'Total Images Processed in group: '+str(len(fa_mgs1))
        print 'Choose the FA Image Number to display the closest FB match:'
        fach = input('Enter Image # (from 1 to '+str(len(fa_mgs1))+'): ')
        print (asian_fa_txtnm[fach-1])
        smisc.imshow(fa_mgs1[fach-1])
        print (comparision_scores_af[fach-1])
        #print (asian_fb_txtnm[comparision_scores_af[fach-1]])
        closest = int(comparision_scores_af[fach-1][0])
        print ('The closest match is: \n ' + asian_fb_txtnm[closest])
        smisc.imshow(fb_mgs1[closest])
    elif choice == 2:
        print 'Total Images Processed in group: '+str(len(fa_mgs2))
        print 'Choose the FA Image Number to display the closest FB match:'
        fach = input('Enter Image # (from 1 to '+str(len(fa_mgs2))+'): ')
        smisc.imshow(fa_mgs2[fach-1])
        print (white_fa_txtnm[fach-1])
        print (comparision_scores_wm[fach-1])
        smisc.imshow(fb_mgs2[fach-1])
        #print (white_fb_txtnm[comparision_scores_wm[fach-1]])
        closest = int(comparision_scores_wm[fach-1][0])
        print ('The closest match is: \n ' + white_fb_txtnm[closest])
    elif choice == 0:
        break
    else:
        print ('\n Choose a valid choice')        
print '\n\nThank You'
