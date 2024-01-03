;file test2.as checked
mcr m3
		jmp LOOP
endmcr
mcr m1
		inc r6
		mov r6,L3
		sub r1, r4
		cmp r3, #-6
		bne END
endmcr
.entry LIST
.extern fn1
MAIN:		add r3, LIST
		jsr fn1
LOOP:		prn #48
		lea STR, r6
		m1
		add r7, r6
		clr K
mcr m2
endmcr
		sub L3,L3
.entry MAIN
		m3
END:		stop
STR:		.string "abcd"
LIST:		.data 6, -9 
		.data -100
K:		.data 31
.extern L3
