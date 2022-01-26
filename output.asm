	jump.i	#lab0		;jump.i	lab0
lab0:
	mul.i	#23,#90,12	;mul.i	23,90,$t0
	sub.i	#0,12,16	;sub.i	0,$t0,$t1
	mov.i	16,0		;mov.i	$t1,x
	mov.i	#9,4		;mov.i	9,y
	write.i	0		;write.i	x
	write.i	4		;write.i	y
	exit			;exit
