;this file prints all the possible errors in as files

;label errors

myLabel: 	.string "abc
myLabel: 	.string "this label was already defined"
whitespace : 	.string " whitespace between label name and : " 
thisLabelIsToooooooooooooooLong: .data 12, 4, 56, 78, 10
1intAtStartOfLabel: .string "not a char at label start" 
   : 		.string "no label!" 
r0: .string "is reserved word"
while: .string "is reserved word"

;data and comma errors:


commaAtEnd:	.data 12,34,56,78,
twoCommas: 	.data 1 ,2  , 3,  , 5
commaAtStart: 	.data , 1 , 2 , +3 , -4
multipleErrors: .data , 1 2 , ,-3 , a , 
missingComma: 	.data 1    3 , +4 , 5 , -6
notInteger: 	.data 1   , 3.2 , +4 , 5 , -6
commaAfterInst:		add , r7 , L3
commaAfterInst2: 	inc , r7 
commaAfterInst3: 	rts ,
			cmp #3   r0
			add #5   LAB

;line length errors:

R1: .string 		"this is a veryyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy     longggggggg string"  
R2: .data  1,2,3,4,5,6,7,8,9,1,2,3,4,5,6,7,8,9,1,2,3,4,5,6,7,8,9,1,2,3,4,5,6,7,8,9,1,2,3,4,5,6,7,8


;numbers size errors
 	.data  123234 , 2 , +3 
	.data -232425432523

;strign errors
 
	.string "no closing 
	.string no starting"
	.string "extra chars" . 
	.string somthing "before string"  

;extraneous operands:

	.data 1, 2 e
	.extern hello Eden!
	.entry goodbye Einav!
	mov #1, r2 :)
	cmp A, B, c
	not r3 , 
	sub r0 , r1 , 
	inc r3  , r4
	stop !

;missing opreands:
	mov r4
	cmp #12 , 
	.data
	.string " 	 	 	 
	.entry
	.extern
	cmp #12  r4
	add 
	sub #67
	not
	clr   
	lea r3,  
	inc
	dec

	
;operand errors:

	jmp @A(#3,#2)
	bne S(!WE,#45)
	jsr GTA(#467,\nogood)
	jmp r0(#3,#4)
	jmp r0(#3,#4)
	mov r0, #3
	mov 1label , #3
	cmp r0 , $label
	lea RK ,   #45
	lea #76 ,   r6
	lea r2 ,   r6
	not !A
	not #12
	inc #-12


