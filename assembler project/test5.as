; test5 - cheked

.entry L
.entry ABC
.entry CBA
.extern A
.extern B
.extern C
M:	mov #-2,r0
 	mov r0,S1
	mov S1 , S2
	cmp X	,	#4
	cmp r3 , r4
L: 	add #1 , S3
 	add r1,r3
S: 	sub #5 ,	X
   	sub X,X
   	not r0
	not S1
ABC: 	not X
	lea S2 ,	S3
	inc A
	inc r7
	inc S2
CBA: 	dec B
	dec r7
	dec S2
	jmp C
	bne STR1
	bne S3
	prn C
	prn B
	prn #-7
	prn S1
	rts 
E: 	stop

STR1: 	.string "abcd"
STR2: 	.string "abcdef"
STR3: 	.string "abc"
X: 	.data +4
ARR: 	.data 	+5,-6	, 4 , +8,	 -3
S1: 	.data 123 , 	-1
S2: 	.string "acd"
S3: 	.data 	+5, 7
