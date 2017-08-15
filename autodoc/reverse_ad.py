from os import rename as mv
from sys import argv

if __name__ == '__main__':
    for arg in argv[1:]:
        mv(arg+'.bak', arg)
