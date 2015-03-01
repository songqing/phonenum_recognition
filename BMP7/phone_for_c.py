#%matplotlib inline
import os

#import matplotlib
import numpy as np
#import matplotlib.pyplot as plt
#%matplotlib inline

# Make sure that caffe is on the python path:
#caffe_root = '/home/songqing/dl/caffe/'  # this file is expected to be in {caffe_root}/examples
import sys
sys.path.insert(0, '/home/songqing/dl/caffe/python')
sys.path.append('/usr/lib/python2.7/dist-packages')

import caffe

#filename='0'
#piccount=0
#counttemp=0
# Set the right path to your model definition file, pretrained model weights,
# and the image you would like to classify.
#MODEL_FILE = caffe_root+'examples/mnist/lenet.prototxt'
#PRETRAINED = '/home/songqing/dl/caffe/examples/mnist/zsq11_lenet_iter_10000.caffemodel'
def classifynum(correctphone, bmpnum):
	MODEL_FILE = '/home/songqing/dl/caffe/examples/mnist/lenet.prototxt'
	PRETRAINED = '/home/songqing/dl/caffe/examples/mnist/zsq11_lenet_iter_10000.caffemodel'
	piccount=0
	iternum=0
	finallist=[]
	poslist=[]
	ratelist=[]
	outfile=open('smallpic.txt','a+')
	outfile.write('phone '+str(bmpnum)+'\n')
	outfile.write(str(correctphone)+'\n')
	for iternum in range(1,140):
		#print iternum
		counttemp=piccount
		piccount=piccount+1
		filename='0'
		if counttemp< 10:
			filename=filename+'00'+str(counttemp)
		elif counttemp < 100:
			filename=filename+'0'+str(counttemp)
		else:
			filename=filename+str(counttemp)

		IMAGE_FILE = '/home/songqing/dl/dl/phone/phonenum_recognition/BMP7/testpic/pic'+filename+'.bmp'
		if(os.path.isfile(IMAGE_FILE) == False):
			continue
		caffe.set_phase_test()
		caffe.set_mode_cpu()
		net = caffe.Classifier(MODEL_FILE, PRETRAINED)
#                       mean=np.load(caffe_root + 'python/caffe/imagenet/ilsvrc_2012_mean.npy'),
#							channel_swap=(2,1,0),
#                      raw_scale=28,
#                       image_dims=(28, 28))
		input_image = caffe.io.load_image(IMAGE_FILE,color=False)
#plt.imshow(input_image)
		prediction=net.predict([input_image],oversample=False)
		nummax=prediction[0].argmax()
	#print 'prediction shape:', prediction[0].shape
#plt.plot(prediction[0])
	#print 'predicted class:', prediction[0].argmax()
	#print 'predicted class:', prediction[0][nummax]
	#print 'predicted class:', prediction[0]

# if rate is small, put 0 to it's value
		if(prediction[0][nummax]<0.98 and nummax!=1):
			if(nummax==9):
				if(prediction[0][nummax]<0.9):
					prediction[0][nummax]=0
			else:
				prediction[0][nummax]=0
		if(prediction[0][nummax]<0.995 and nummax==1):
			prediction[0][nummax]=0
		if(prediction[0][nummax]==0):
			outfile.write(str(nummax)+' '+str(prediction[0][nummax])+'\n')
			continue
#in a range , get the max value
		if((len(finallist)==0)):
			if(nummax==1):# the first num is 1
				finallist.append(nummax)
				ratelist.append(prediction[0][nummax])
				poslist.append(iternum)
		elif(iternum - poslist[-1]<=2):
			if(prediction[0][nummax] > ratelist[-1]):
				if(len(finallist)>1):
					if(iternum-poslist[-2] < 14):
						if(nummax==1):
							if(prediction[0][nummax] > 0.999):
								finallist[-1]=nummax
								ratelist[-1]=prediction[0][nummax]
								poslist[-1]=iternum
						else:
							finallist[-1]=nummax
							ratelist[-1]=prediction[0][nummax]
							poslist[-1]=iternum
				else:
					finallist[-1]=nummax
					ratelist[-1]=prediction[0][nummax]
					poslist[-1]=iternum
#put into the list OR update, limit operation
		elif(iternum - poslist[-1] <=5 and iternum-poslist[-1] >=4):
			if(len(finallist)==1):#add not update
				if(nummax==1):
					if(prediction[0][nummax] > 0.999):
						finallist.append(nummax)
						ratelist.append(prediction[0][nummax])
						poslist.append(iternum)
				elif(prediction[0][nummax] > 0.99):
					finallist.append(nummax)
					ratelist.append(prediction[0][nummax])
					poslist.append(iternum)
			else:
				if(ratelist[-1]> 0.999 and len(finallist) < 11):# last rate is too high, add
					if(nummax==finallist[-1] and finallist[-1]== finallist[-2]):
						if(nummax!=1 and iternum-poslist[-2] > 14):
							finallist.append(nummax)
							ratelist.append(prediction[0][nummax])
							poslist.append(iternum)
					elif(nummax==1):
						if(finallist[-1]!=0):
							if(prediction[0][nummax] > 0.9999 and iternum- poslist[-2] > 14):
								finallist.append(nummax)
								ratelist.append(prediction[0][nummax])
								poslist.append(iternum)
					elif(prediction[0][nummax] > 0.99):
						finallist.append(nummax)
						ratelist.append(prediction[0][nummax])
						poslist.append(iternum)
				elif(ratelist[-1]>0.98 and nummax==9 and len(finallist) < 11):# 9 rate is lower, add
					if(nummax==finallist[-1] and finallist[-1]== finallist[-2]):
						if(nummax!=1 and iternum-poslist[-2] > 14):
							finallist.append(nummax)
							ratelist.append(prediction[0][nummax])
							poslist.append(iternum)
					elif(nummax==1 and iternum - poslist[-2] > 14):
						if(prediction[0][nummax] > 0.9999):
							finallist.append(nummax)
							ratelist.append(prediction[0][nummax])
							poslist.append(iternum)
					elif(prediction[0][nummax] > 0.99):
						finallist.append(nummax)
						ratelist.append(prediction[0][nummax])
						poslist.append(iternum)
				elif(prediction[0][nummax] > ratelist[-1]):#update
					if(iternum-poslist[-2] < 14):
						if(nummax==1):
							if(finallist[-1]==0):
								if(prediction[0][nummax] > 0.9999):
									finallist[-1]=nummax
									ratelist[-1]=prediction[0][nummax]
									poslist[-1]=iternum
							elif(prediction[0][nummax] > 0.999):
								finallist[-1]=nummax
								ratelist[-1]=prediction[0][nummax]
								poslist[-1]=iternum
						else:
							finallist[-1]=nummax
							ratelist[-1]=prediction[0][nummax]
							poslist[-1]=iternum

#direct put into the list
		elif((iternum-poslist[-1])>5 and len(finallist) < 11):
			if(len(finallist) <= 2):
				if(nummax==1):
					if(prediction[0][nummax] > 0.993):
						finallist.append(nummax)
						ratelist.append(prediction[0][nummax])
						poslist.append(iternum)
				else:
					finallist.append(nummax)
					ratelist.append(prediction[0][nummax])
					poslist.append(iternum)
			else:# 2 same num might detect as 3 nums
				if(nummax==finallist[-1] and finallist[-1]== finallist[-2]):
					if(nummax!=1 and iternum-poslist[-2] > 14):
						finallist.append(nummax)
						ratelist.append(prediction[0][nummax])
						poslist.append(iternum)
				else:
					if(nummax==1):
						if(prediction[0][nummax] > 0.993):
							finallist.append(nummax)
							ratelist.append(prediction[0][nummax])
							poslist.append(iternum)
					else:
						finallist.append(nummax)
						ratelist.append(prediction[0][nummax])
						poslist.append(iternum)

		outfile.write(str(nummax)+' '+str(prediction[0][nummax])+'\n')

#%timeit net.predict([input_image])

# Resize the image to the standard (256, 256) and oversample net input sized crops.
		input_oversampled = caffe.io.oversample([caffe.io.resize_image(input_image, net.image_dims)], net.crop_dims)
# 'data' is the input blob name in the model definition, so we preprocess for that input.
		caffe_input = np.asarray([net.preprocess('data', in_) for in_ in input_oversampled])
# forward() takes keyword args for the input blobs with preprocessed input arrays.
#%timeit net.forward(data=caffe_input)
#	print finallist
#	print ratelist
#	print poslist
#	outfile.close()
	outfile.write(str(finallist)+'\n')
	outfile.write(str(ratelist)+'\n')
	outfile.write(str(poslist)+'\n')
	outfile.close()
	str_out=''
	n_for=0
	while(n_for<len(finallist)):
		str_out+=str(finallist[n_for])
		n_for=n_for+1
	
#	print str_out
	return str(str_out)
	
if __name__ == '__main__':
	#piccount=0
	#finallist=[]
	#poslist=[]
	#ratelist=[]
	#outfile=open('smallpic.txt','w')
	#for i in range(1,130):
	classifynum(15650701111,1)
	#outfile.close()
	#print finallist
	#print ratelist
	#print poslist

