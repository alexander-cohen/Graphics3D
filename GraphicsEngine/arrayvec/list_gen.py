def newAV(type, typen):
	with open('baseAV.c.template') as f:
		ctemp = f.read()
	with open('baseAV.h.template') as f:
		htemp = f.read()
	with open('%sarrayvec.h'%(typen), 'w') as f:
		f.write(htemp.replace('@', typen).replace('$', type))
	with open('%sarrayvec.c'%(typen), 'w') as f:
		f.write(ctemp.replace('@', typen).replace('$', type))

newAV('material', 'mat')
newAV('double', 'double')
newAV('int', 'int')
newAV('int *', 'intptr')
newAV('Vec2', 'vec2')
newAV('Vec3', 'vec3')
newAV('void *', 'voidptr')