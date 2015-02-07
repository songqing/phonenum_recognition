#!/usr/bin/env python
#coding=UTF-8

import getopt, os, sys

def con_user(path):
    filelist = os.listdir(path)
    file_content = open('content.txt','w')
    for user in filelist:
        user_file = os.path.join(path, user)
        if os.path.isfile(user_file):
            file_user = open(user_file)
            password = file_user.read()
            file_s = 'filename:' + user + '\n'
            content_s = 'content:' + password + '\n'
            file_content.write(user + '\n')
    file_content.close()
    print '''The file "content.txt" is saved!'''

def usage():
    print """USAGE: con_user.py content_dirs
    The content result will saved as content.txt"""

if __name__ == "__main__":
    try:
        opts, args = getopt.getopt(sys.argv[1:],'',[])
    except getopt.GetoptError:
        usage()
        sys.exit(0)
    if not args:
        usage()
        sys.exit(0)
    else:
        for arg in args:
            if os.path.isdir(arg):
                con_user(arg)
            else:
                usage()
                print "content_dirs muest a dir!"
                sys.exit(0)
