	jump.i	#lab0		;jump.i	lab0
lab0:
	add.i	0,4,16		;add.i	x,y,$t0
	mov.i	16,8		;mov.i	$t0,g
	exit			;exit
