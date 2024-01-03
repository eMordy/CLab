;errors in second pass- label used as an operand has not defined

.entry LENGTH 
.extern W 
MAIN:		mov r3 ,LEN
LOOP:		jmp L1(#-1,r6)
			prn #-5  
			bne WE(r4,r5)   
			sub r1 , r4
			bne L33 
L1:			inc KO
.entry LOOP 
			bne LOOP(K,W)  
END:		stop 
STR:		.string "abcdef"  	
LENGTH:		.data 6,-9 , 15  
K:			.data 22	 	
.extern L3 

