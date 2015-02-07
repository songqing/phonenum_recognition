#! /usr/bin/python
# -*- coding: cp936 -*-
# filename : fileCount.py
# 用于统计当前文件夹(改脚本所在文件)下所有的目录及目录中文件名
# 只支持三级目录
# Author:yupianer
# Date : 20121229

import glob
import os

i=0
path = os.getcwd()
filetmp=file('filename.txt','w')
list_out=glob.glob('*')
for m in list_out:
    i=i+1
    filetmp.writelines(str(i)+' : '+m+'\n')
    #写入第一层文件
    
    if os.path.isdir(m):
        list_in=glob.glob(m+'/*')
        for n in list_in:
            i=i+1
            filetmp.writelines(str(i)+' : '+n.replace(m,'- -')+'\n')
            #写入第二层文件
            
            if os.path.isdir(n):
                filetmp.writelines(str(i)+' : '+n+'\n')
                list_three=glob.glob(n+'/*')
                for z in list_three:
                    i =i+1
                    filetmp.writelines(str(i)+' : '+z.replace(n,'- -\- -')+'\n')
                    #写入第三层文件
    
filetmp.close()
print '执行完成！'


