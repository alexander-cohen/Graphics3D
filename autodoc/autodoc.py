import re
from sys import argv
import os

def cls():
    os.system('cls' if os.name=='nt' else 'clear')
    
def autodoc(filename):
    with open(filename) as f:
        lines = f.readlines()
    lidx = 0
    for lidx in range(len(lines)):
        line = lines[lidx]
        if re.match('[_a-zA-Z].*\(.*\) *;.*', line) is not None: # anything not indented that has a letter, parens, then a semicolon, then other shit at the end
            docparts = []
            line = line.strip()
            print '\n\n' + line + '\n'
            desc = raw_input('Description of this function:\n')
            if desc.strip() == '':
                continue
            docparts.append(desc)
            docparts.append('') # newline basically
            argstart = line.find('(')+1
            argend = line.find(')')
            args = line[argstart:argend].split(',')
            for arg in args:
                if arg == '':
                    continue
                argdesc = raw_input('\nDescription of argument "%s":\n' % (arg))
                argparts = arg.split(' ')
                i = -1
                while argparts[i] == '': # handle spaces before commas
                    i -= 1
                arglast = argparts[i]
                match = re.search('[_a-zA-Z][_a-zA-Z0-9]*', arglast) # in case of pointers
                if match is not None:
                    argname = arglast[slice(*match.span())]
                else:
                    argname = raw_input('\nCould not determine function name. Enter function name: ').strip()
                docparts.append('@param %s %s' % (argname, argdesc))
            retdesc = raw_input('\nDescription of return value:\n')
            if retdesc.strip() != '':
                docparts.append('') # newline again to separate
                docparts.append('@returns ' + retdesc.strip())
            comment = '\n/**\n'
            for part in docparts:
                comment += starAndCap(part)
            comment += ' */\n'
            lines[lidx] = comment + lines[lidx]
    return ''.join(lines)

def starAndCap(st):
    ''' idea: read from st into new buf word by word until len of new buf
    >= 77, then add buf into new string with a newline and star attached '''
    buf = ''
    newst = ''
    words = st.split(' ')
    for word in words:
        if len(buf) + len(word) >= 77:
            newst += ' * ' + buf.strip() + '\n' # kill hanging space
            buf = ''
        while len(word) >= 77: # comes after buf clear because that needs to happen anyway if len(word) too big
            newst += ' * ' + word[:77] + '\n'
            word = word[77:]
        buf += word + ' '
    # clear remainder of buf
    newst += ' * ' + buf.strip() + '\n'
    return newst

if __name__ == '__main__':
    for arg in argv[1:]:
        with open(arg+'.autodoc', 'w') as f:
            cls()
            print 'FILE', arg, '\n\n\n\n'
            f.write(autodoc(arg))
                
