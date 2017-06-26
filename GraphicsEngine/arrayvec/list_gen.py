def newAV(type, typen):
	with open('baseAV.c.template') as f:
		ctemp = f.read()
	with open('baseAV.h.template') as f:
		htemp = f.read()
	with open('%sarrayvec.h'%(typen), 'w') as f:
		f.write(htemp.replace('@', typen).replace('$', type))
	with open('%sarrayvec.c'%(typen), 'w') as f:
		f.write(ctemp.replace('@', typen).replace('$', type))

newAV('int *', 'intptr')
newAV('int', 'int')
newAV('double', 'double')
newAV('void *', 'voidptr')
