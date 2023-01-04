	.set JONES_VERSION,47

        /*
        rsp  - stack pointer
        rbp - struct base
        r9 - rstack pointer

        storing in rbp struct while running
        rsp
        rbp
        rbx
        
        */ 

        // TODO: I think it would be really cool if I could
        // generate this via c & include it here

        .set OLD_RSI, 0
        .set OLD_RBP, 8
        .set OLD_RBX, 16
        .set STACK_TOP, 32
        .set RSTACK_TOP, 40
        .set R0, 48
        .set SZ_OFFSET, 56
        .set STATE_OFFSET, 64
        .set HERE_OFFSET, 72
        .set LATEST_OFFSET, 80
        .set OUTPUT_MAX, 88
        .set OUTPUT_CURRENT, 96
        .set BASE_OFFSET, 104
        .set INPUT_CURRENT, 112
        
        .set WORDBUF, 128

        
        .macro NEXT
	lodsq
	jmp     *(%rax)
	.endm

	.macro PUSHRSP reg
	lea     -8(%r9),%r9	// push reg on to return stack
	movq     \reg,(%r9)
	.endm

	.macro POPRSP reg
	movq     (%r9),\reg		// pop top of return stack to reg
	lea     8(%r9),%r9
	.endm

        
DOCOL:
	PUSHRSP %rsi		// push %esi on to the return stack
	add     $8,%rax		// %eax points to codeword, so make
	mov     %rax,%rsi		// %esi point to first data word
	NEXT

	.set F_IMMED,0x80
	.set F_HIDDEN,0x20
	.set F_LENMASK,0x1f	// length mask

	// Store the chain of links.
	.set link,0

	.macro defword name, namelen, flags=0, label
	.align 8
	.globl name_\label
name_\label :
	.int link, 0		// link
	.set link,name_\label
	.byte \flags+\namelen	// flags + length byte
	.ascii "\name"		// the name
	.align 8		// padding to next 8 byte boundary
	.globl \label
\label :
	.int DOCOL, 0		// codeword - the interpreter
	// list of word pointers follow
	.endm
        
	.macro defcode name, namelen, flags=0, label
	.align 8
	.globl name_\label
name_\label :
	.int link, 0		// link
	.set link,name_\label
	.byte \flags+\namelen	// flags + length byte
	.ascii "\name"		// the name
	.align 8		// padding to next 4 byte boundary
	.globl \label
\label :
	.int code_\label,0	// codeword
	.align 8
	.globl code_\label
code_\label :			// assembler code follows
	.endm
        
	defcode "DROP",4,,DROP
	pop %rax		// drop top of stack
	NEXT

	defcode "SWAP",4,,SWAP
	pop %rax		// swap top two elements on stack
	pop %rbx
	push %rax
	push %rbx
	NEXT

	defcode "DUP",3,,DUP
	mov (%rsp),%rax		// duplicate top of stack
	push %rax
	NEXT

	defcode "OVER",4,,OVER
	mov 8(%rsp),%rax	// get the second element of stack
	push %rax		// and push it on top
	NEXT

	defcode "ROT",3,,ROT
	pop %rax
	pop %rbx
	pop %rcx
	push %rbx
	push %rax
	push %rcx
	NEXT

	defcode "-ROT",4,,NROT
	pop %rax
	pop %rbx
	pop %rcx
	push %rax
	push %rcx
	push %rbx
	NEXT

	defcode "2DROP",5,,TWODROP // drop top two elements of stack
	pop %rax
	pop %rax
	NEXT

	defcode "2DUP",4,,TWODUP // duplicate top two elements of stack
	mov (%rsp),%rax
	mov 8(%rsp),%rbx
	push %rbx
	push %rax
	NEXT

	defcode "2SWAP",5,,TWOSWAP // swap top two pairs of elements of stack
	pop %rax
	pop %rbx
	pop %rcx
	pop %rdx
	push %rbx
	push %rax
	push %rdx
	push %rcx
	NEXT

        defcode "?DUP",4,,QDUP	// duplicate top of stack if non-zero
	mov (%rsp),%rax
	test %rax,%rax
	jz 1f
	push %rax
1:	NEXT

	defcode "1+",2,,INCR
	incq (%rsp)		// increment top of stack
	NEXT

	defcode "1-",2,,DECR
	decq (%rsp)		// decrement top of stack
	NEXT

	defcode "8+",2,,INCR8
	addq $8,(%rsp)		// add 8 to top of stack
	NEXT

	defcode "8-",2,,DECR8
	subq $8,(%rsp)		// subtract 8 from top of stack
	NEXT

	defcode "+",1,,ADD
	pop %rax		// get top of stack
	addq %rax,(%rsp)	// and add it to next word on stack
	NEXT

	defcode "-",1,,SUB
	pop %rax		// get top of stack
	subq %rax,(%rsp)	// and subtract it from next word on stack
	NEXT

	defcode "*",1,,MUL
	pop %rax
	pop %rbx
	imulq %rbx,%rax
	push %rax		// ignore overflow
	NEXT
        
/*
	In this FORTH, only /MOD is primitive.  Later we will define the / and MOD words in
	terms of the primitive /MOD.  The design of the i386 assembly instruction idiv which
	leaves both quotient and remainder makes this the obvious choice.
*/

	defcode "/MOD",4,,DIVMOD
	xor %rdx,%rdx
	pop %rbx
	pop %rax
	idivq %rbx
	push %rdx		// push remainder
	push %rax		// push quotient
	NEXT


/*
	Lots of comparison operations like =, <, >, etc..

	ANS FORTH says that the comparison words should return all (binary) 1's for
	TRUE and all 0's for FALSE.  However this is a bit of a strange convention
	so this FORTH breaks it and returns the more normal (for C programmers ...)
	1 meaning TRUE and 0 meaning FALSE.
*/

	defcode "=",1,,EQU	// top two words are equal?
	pop %rax
	pop %rbx
	cmp %rbx,%rax
	sete %al
	movzbq %al,%eax
	push %rax
	NEXT

	defcode "<>",2,,NEQU	// top two words are not equal?
	pop %rax
	pop %rbx
	cmp %rbx,%rax
	setne %al
	movzbq %al,%rax
	push %rax
	NEXT

	defcode "<",1,,LT
	pop %rax
	pop %rbx
	cmp %rax,%rbx
	setl %al
	movzbq %al,%rax
	push %rax
	NEXT

	defcode ">",1,,GT
	pop %rax
	pop %rbx
	cmp %rax,%rbx
	setg %al
	movzbq %al,%rax
	push %rax
	NEXT

	defcode "<=",2,,LE
	pop %rax
	pop %rbx
	cmp %rax,%rbx
	setle %al
	movzbq %al,%rax
	push %rax
	NEXT

	defcode ">=",2,,GE
	pop %rax
	pop %rbx
	cmp %rax,%rbx
	setge %al
	movzbq %al,%rax
	push %rax
	NEXT

	defcode "0=",2,,ZEQU	// top of stack equals 0?
	pop %rax
	test %rax,%rax
	setz %al
	movzbq %al,%rax
	push %rax
	NEXT

	defcode "0<>",3,,ZNEQU	// top of stack not 0?
	pop %rax
	test %rax,%rax
	setnz %al
	movzbq %al,%rax
	push %rax
	NEXT

	defcode "0<",2,,ZLT	// comparisons with 0
	pop %rax
	test %rax,%rax
	setl %al
	movzbq %al,%rax
	push %rax
	NEXT

	defcode "0>",2,,ZGT
	pop %rax
	test %rax,%rax
	setg %al
	movzbq %al,%rax
	push %rax
	NEXT

	defcode "0<=",3,,ZLE
	pop %rax
	test %rax,%rax
	setle %al
	movzbq %al,%rax
	push %rax
	NEXT

	defcode "0>=",3,,ZGE
	pop %rax
	test %rax,%rax
	setge %al
	movzbq %al,%rax
	push %rax
	NEXT

	defcode "AND",3,,AND	// bitwise AND
	pop %rax
	andq %rax,(%rsp)
	NEXT

	defcode "OR",2,,OR	// bitwise OR
	pop %rax
	orq %rax,(%rsp)
	NEXT

	defcode "XOR",3,,XOR	// bitwise XOR
	pop %rax
	xorq %rax,(%rsp)
	NEXT

	defcode "INVERT",6,,INVERT // this is the FORTH bitwise "NOT" function (cf. NEGATE and NOT)
	notq (%esp)
	NEXT
        
        
/*
	RETURN STACK ----------------------------------------------------------------------

	These words allow you to access the return stack.  TODO Recall that the register ??? always points to
	the top of the return stack.
*/

	defcode ">R",2,,TOR
	pop %rax		// pop parameter stack into %eax
	PUSHRSP %rax		// push it on to the return stack
	NEXT

	defcode "R>",2,,FROMR
	POPRSP %rax		// pop return stack on to %eax
	push %rax		// and push on to parameter stack
	NEXT

	defcode "RSP@",4,,RSPFETCH
	push    %r9
	NEXT

	defcode "RSP!",4,,RSPSTORE
	pop     %r9
	NEXT

	defcode "RDROP",5,,RDROP 
        add $8, %r9
	NEXT

        defcode "R0",2,,RZ
        movq	R0(%rbp), %rax
        push %rax
        NEXT

/*
	BRANCHING ----------------------------------------------------------------------

	It turns out that all you need in order to define looping constructs, IF-statements, etc.
	are two primitives.

	BRANCH is an unconditional branch. 0BRANCH is a conditional branch (it only branches if the
	top of stack is zero).

	The diagram below shows how BRANCH works in some imaginary compiled word.  When BRANCH executes,
	%esi starts by pointing to the offset field (compare to LIT above):

	+---------------------+-------+---- - - ---+------------+------------+---- - - - ----+------------+
	| (Dictionary header) | DOCOL |            | BRANCH     | offset     | (skipped)     | word       |
	+---------------------+-------+---- - - ---+------------+-----|------+---- - - - ----+------------+
								   ^  |			      ^
								   |  |			      |
								   |  +-----------------------+
								  %esi added to offset

	The offset is added to %esi to make the new %esi, and the result is that when NEXT runs, execution
	continues at the branch target.  Negative offsets work as expected.

	0BRANCH is the same except the branch happens conditionally.

	Now standard FORTH words such as IF, THEN, ELSE, WHILE, REPEAT, etc. can be implemented entirely
	in FORTH.  They are IMMEDIATE words which append various combinations of BRANCH or 0BRANCH
	into the word currently being compiled.

	As an example, code written like this:

		condition-code IF true-part THEN rest-code

	compiles to:

		condition-code 0BRANCH OFFSET true-part rest-code
					  |		^
					  |		|
					  +-------------+
*/

        
	defcode "BRANCH",6,,BRANCH
	add (%rsi),%rsi		// add the offset to the instruction pointer
	NEXT

	defcode "0BRANCH",7,,ZBRANCH
	pop %rax
	test %rax,%rax		// top of stack is zero?
	jz code_BRANCH		// if so, jump back to the branch function above
	lodsq			// otherwise we need to skip the offset
	NEXT

/*
	RETURNING FROM FORTH WORDS ----------------------------------------------------------------------

	Time to talk about what happens when we EXIT a function.  In this diagram QUADRUPLE has called
	DOUBLE, and DOUBLE is about to exit (look at where %esi is pointing):

		QUADRUPLE
		+------------------+
		| codeword         |
		+------------------+		   DOUBLE
		| addr of DOUBLE  ---------------> +------------------+
		+------------------+               | codeword         |
		| addr of DOUBLE   |		   +------------------+
		+------------------+	   	   | addr of DUP      |
		| addr of EXIT	   |		   +------------------+
		+------------------+	   	   | addr of +        |
						   +------------------+
					   %esi -> | addr of EXIT     |
						   +------------------+

	What happens when the + function does NEXT?  Well, the following code is executed.
*/

	defcode "EXIT",4,,EXIT
	POPRSP %rsi		// pop return stack into %esi
	NEXT

/*
	EXIT gets the old %esi which we saved from before on the return stack, and puts it in %esi.
	So after this (but just before NEXT) we get:

		QUADRUPLE
		+------------------+
		| codeword         |
		+------------------+		   DOUBLE
		| addr of DOUBLE  ---------------> +------------------+
		+------------------+               | codeword         |
	%esi ->	| addr of DOUBLE   |		   +------------------+
		+------------------+	   	   | addr of DUP      |
		| addr of EXIT	   |		   +------------------+
		+------------------+	   	   | addr of +        |
						   +------------------+
						   | addr of EXIT     |
						   +------------------+

	And NEXT just completes the job by, well, in this case just by calling DOUBLE again :-)

	LITERALS ----------------------------------------------------------------------

	The final point I "glossed over" before was how to deal with functions that do anything
	apart from calling other functions.  For example, suppose that DOUBLE was defined like this:

	: DOUBLE 2 * ;

	It does the same thing, but how do we compile it since it contains the literal 2?  One way
	would be to have a function called "2" (which you'd have to write in assembler), but you'd need
	a function for every single literal that you wanted to use.

	FORTH solves this by compiling the function using a special word called LIT:

	+---------------------------+-------+-------+-------+-------+-------+
	| (usual header of DOUBLE)  | DOCOL | LIT   | 2     | *     | EXIT  |
	+---------------------------+-------+-------+-------+-------+-------+

	LIT is executed in the normal way, but what it does next is definitely not normal.  It
	looks at %esi (which now points to the number 2), grabs it, pushes it on the stack, then
	manipulates %esi in order to skip the number as if it had never been there.

	What's neat is that the whole grab/manipulate can be done using a single byte single
	i386 instruction, our old friend LODSL.  Rather than me drawing more ASCII-art diagrams,
	see if you can find out how LIT works:
*/

	defcode "LIT",3,,LIT
	// %esi points to the next command, but in this case it points to the next
	// literal 32 bit integer.  Get that literal into %eax and increment %esi.
	// On x86, it's a convenient single byte instruction!  (cf. NEXT macro)
	lodsq
	push %rax		// push the literal number on to stack
	NEXT



/*
	MEMORY ----------------------------------------------------------------------

	As important point about FORTH is that it gives you direct access to the lowest levels
	of the machine.  Manipulating memory directly is done frequently in FORTH, and these are
	the primitive words for doing it.
*/

	defcode "!",1,,STORE
	pop %rbx		// address to store at
	pop %rax		// data to store there
	mov %rax,(%rbx)		// store it
	NEXT

	defcode "@",1,,FETCH
	pop %rbx		// address to fetch
	mov (%rbx),%rax		// fetch it
	push %rax		// push value onto stack
	NEXT

	defcode "+!",2,,ADDSTORE
	pop %rbx		// address
	pop %rax		// the amount to add
	addq %rax,(%rbx)	// add it
	NEXT

	defcode "-!",2,,SUBSTORE
	pop %rbx		// address
	pop %rax		// the amount to subtract
	subq %rax,(%rbx)	// add it
	NEXT


/*
	! and @ (STORE and FETCH) store 32-bit words.  It's also useful to be able to read and write bytes
	so we also define standard words C@ and C!.

	Byte-oriented operations only work on architectures which permit them (i386 is one of those).
 */

	defcode "C!",2,,STOREBYTE
	pop %rbx		// address to store at
	pop %rax		// data to store there
	mov %al,(%rbx)		// store it
	NEXT

	defcode "C@",2,,FETCHBYTE
	pop %rbx		// address to fetch
	xor %rax,%rax
	movb (%rbx),%al		// fetch it
	push %rax		// push value onto stack
	NEXT

        /* C@C! is a useful byte copy primitive. */
	defcode "C@C!",4,,CCOPY
	movq 8(%rsp),%rbx	// source address
	movb (%rbx),%al		// get source character
	pop %rdi		// destination address
	stosb			// copy to destination
	push %rdi		// increment destination address
	incq 8(%rsp)		// increment source address
	NEXT

        /* and CMOVE is a block copy operation. */
	defcode "CMOVE",5,,CMOVE
	mov %rsi,%rdx		// preserve %esi
	pop %rcx		// length
	pop %rdi		// destination address
	pop %rsi		// source address
	rep movsb		// copy source to destination
	mov %rdx,%rsi		// restore %esi
	NEXT


        /*
	BUILT-IN VARIABLES ----------------------------------------------------------------------

	These are some built-in variables and related standard FORTH words.  Of these, the only one that we
	have discussed so far was LATEST, which points to the last (most recently defined) word in the
	FORTH dictionary.  LATEST is also a FORTH word which pushes the address of LATEST (the variable)
	on to the stack, so you can read or write it using @ and ! operators.  For example, to print
	the current value of LATEST (and this can apply to any FORTH variable) you would do:

	LATEST @ . CR

	To make defining variables shorter, I'm using a macro called stackvar, similar to defword and
	defcode above.
        
	The built-in variables are:

	STATE		Is the interpreter executing code (0) or compiling a word (non-zero)?
	LATEST		Points to the latest (most recently defined) word in the dictionary.
	HERE		Points to the next free byte of memory.  When compiling, compiled words go here.
	S0		Stores the address of the top of the parameter stack.
	BASE		The current base for printing and reading numbers.

        */

        .macro stackvar name, namelen, offset, flags=0, label
        defcode \name,\namelen,\flags,\label
        lea  \offset(%rbp),%rax
        push %rax
        NEXT
	.endm

        stackvar "STATE",5,STATE_OFFSET,,STATE
        stackvar "HERE",4,HERE_OFFSET,,HERE
        stackvar "LATEST",6,LATEST_OFFSET,,LATEST
        stackvar "S0",2,SZ_OFFSET,,SZ
        stackvar "BASE",4,BASE_OFFSET,,BASE

       /*
	BUILT-IN CONSTANTS ----------------------------------------------------------------------

	It's also useful to expose a few constants to FORTH.  When the word is executed it pushes a
	constant value on the stack.

	The built-in constants are:

	VERSION		Is the current version of this FORTH.
	R0		The address of the top of the return stack.
	DOCOL		Pointer to DOCOL.
	F_IMMED		The IMMEDIATE flag's actual value.
	F_HIDDEN	The HIDDEN flag's actual value.
	F_LENMASK	The length mask in the flags/len byte.

        */

	.macro defconst name, namelen, flags=0, label, value
	defcode \name,\namelen,\flags,\label
	push $\value
	NEXT
	.endm

	defconst "VERSION",7,,VERSION,JONES_VERSION
	defconst "DOCOL",5,,__DOCOL,DOCOL
	defconst "F_IMMED",7,,__F_IMMED,F_IMMED
	defconst "F_HIDDEN",8,,__F_HIDDEN,F_HIDDEN
	defconst "F_LENMASK",9,,__F_LENMASK,F_LENMASK
 
        
        /*
	PARAMETER (DATA) STACK ----------------------------------------------------------------------

	These functions allow you to manipulate the parameter stack.  Recall that Linux sets up the parameter
	stack for us, and it is accessed through %esp.
*/

	defcode "DSP@",4,,DSPFETCH
	mov %rsp,%rax
	push %rax
	NEXT

	defcode "DSP!",4,,DSPSTORE
	pop %rsp
	NEXT


/*
	INPUT AND OUTPUT ----------------------------------------------------------------------

	These are our first really meaty/complicated FORTH primitives.  I have chosen to write them in
	assembler, but surprisingly in "real" FORTH implementations these are often written in terms
	of more fundamental FORTH primitives.  I chose to avoid that because I think that just obscures
	the implementation.  After all, you may not understand assembler but you can just think of it
	as an opaque block of code that does what it says.

	Let's discuss input first.

	The FORTH word KEY reads the next byte from stdin (and pushes it on the parameter stack).
	So if KEY is called and someone hits the space key, then the number 32 (ASCII code of space)
	is pushed on the stack.

	In FORTH there is no distinction between reading code and reading input.  We might be reading
	and compiling code, we might be reading words to execute, we might be asking for the user
	to type their name -- ultimately it all comes in through KEY.

	The implementation of KEY uses an input buffer of a certain size (defined at the end of this
	file).  It calls the Linux read(2) system call to fill this buffer and tracks its position
	in the buffer using a couple of variables, and if it runs out of input buffer then it refills
	it automatically.  The other thing that KEY does is if it detects that stdin has closed, it
	exits the program, which is why when you hit ^D the FORTH system cleanly exits.

     buffer			      bufftop
	|				 |
	V				 V
	+-------------------------------+--------------------------------------+
	| INPUT READ FROM STDIN ....... | unused part of the buffer            |
	+-------------------------------+--------------------------------------+
	                  ^
			  |
		       currkey (next character to read)

	<---------------------- BUFFER_SIZE (4096 bytes) ---------------------->
*/

	defcode "KEY",3,,KEY
	call _KEY
	push %rax		// push return value on stack
	NEXT
        
_KEY:
	mov INPUT_CURRENT(%rbp),%rbx
        cmp $0, %rbx
	je  2f
        xor %rax,%rax
        mov (%rbx), %al
        inc %rbx
        cmp $0, %al
        jne not_zero_char
        mov $' ', %al
        mov $0, %rbx //set currkey to null, flagging we're about finished
not_zero_char:    
	mov %rbx,INPUT_CURRENT(%rbp)	// increment current input char
	ret //TODO stop passing data through rax
2:
        mov $2, %rdx // return code indicating we've run out of input
        call fpause
        jmp _KEY


/*
	TODO write this comment for my special version of init
*/

	defcode "EMIT",4,,EMIT

        mov OUTPUT_CURRENT(%rbp),%rbx
	pop %rax
        mov %al, (%rbx)
        inc %rbx
        mov %rbx, OUTPUT_CURRENT(%rbp)
        mov OUTPUT_MAX(%rbp), %rax
        cmp %rax, %rbx
        jne emit_done // we still have some space in the buffer
        mov $4, %rdx // return code indicating we've run out of input
        call fpause
emit_done:
	NEXT        


        
	defcode "WORD",4,,WORD
	call _WORD
	push %rdi		// push base address
	push %rcx		// push length
	NEXT
        
_WORD:
	/* Search for first non-blank character.  Also skip \ comments. */
1:
	call _KEY		// get next key, returned in %eax
	cmpb $'\\',%al		// start of a comment?
	je 4f			// if so, skip the comment
	cmpb $' ',%al
	jbe 1b			// if so, keep looking

	/* Search for the end of the word, storing chars as we go. */
        mov $0, %rcx
	lea WORDBUF(%rbp),%rdi	// pointer to return buffer
2:
	stosb			// add character to return buffer
        inc %rcx
        cmp $32, %rcx           // check if we've reached the max
        jz 3f
	call _KEY		// get next key, returned in %al
	cmpb $' ',%al		// is blank?
	ja 2b			// if not, keep looping
3:      
        movb $0, (%rdi) 
	lea WORDBUF(%rbp),%rdi	// return address of the word 
	ret

	/* Code to skip \ comments to end of the current line. */
4:
	call _KEY
	cmpb $'\n',%al		// end of line yet?
	jne 4b
	jmp 1b
        
	defcode "NUMBER",6,,NUMBER
	pop %rcx		// length of string
	pop %rdi		// start address of string
	call _NUMBER
	push %rax		// parsed number
	push %rcx		// number of unparsed characters (0 = no error)
	NEXT


_NUMBER:
	xor %rax,%rax
	xor %rbx,%rbx

	test %rcx,%rcx		// trying to parse a zero-length string is an error, but will return 0.
	jz 5f

	movq BASE_OFFSET(%rbp),%rdx	// get BASE (in %dl)

	// Check if first character is '-'.
	movb (%rdi),%bl		// %bl = first character in string
	inc %rdi
	push %rax		// push 0 on stack
	cmpb $'-',%bl		// negative number?
	jnz 2f
	pop %rax
	push %rbx		// push <> 0 on stack, indicating negative
	dec %rcx
	jnz 1f
	pop %rbx		// error: string is only '-'.
	movq $1,%rcx
	ret

	// Loop reading digits.
1:	imulq %rdx,%rax		// %eax *= BASE
	movb (%rdi),%bl		// %bl = next character in string
	inc %rdi

	// Convert 0-9, A-Z to a number 0-35.
2:	subb $'0',%bl		// < '0'?
	jb 4f
	cmp $10,%bl		// <= '9'?
	jb 3f
	subb $17,%bl		// < 'A'? (17 is 'A'-'0')
	jb 4f
	addb $10,%bl

3:	cmp %dl,%bl		// >= BASE?
	jge 4f

	// OK, so add it to %eax and loop.
	add %rbx,%rax
	dec %rcx
	jnz 1b

	// Negate the result if first character was '-' (saved on the stack).
4:	pop %rbx
	test %rbx,%rbx
	jz 5f
	neg %rax

5:	ret


	defcode "FIND",4,,FIND
	pop %rcx		// %ecx = length
	pop %rdi		// %edi = address
	call _FIND
	push %rax		// %eax = address of dictionary entry (or NULL)
	NEXT

_FIND:
	push %rsi		// Save %esi so we can use it in string comparison.

	// Now we start searching backwards through the dictionary for this word.
	mov LATEST_OFFSET(%rbp),%rdx	// LATEST points to name header of the latest word in the dictionary
1:	test %rdx,%rdx		// NULL pointer?  (end of the linked list)
	je 4f

	// Compare the length expected and the length of the word.
	// Note that if the F_HIDDEN flag is set on the word, then by a bit of trickery
	// this won't pick the word (the length will appear to be wrong).
	xor %rax,%rax
	movb 8(%rdx),%al	// %al = flags+length field
	andb $(F_HIDDEN|F_LENMASK),%al // %al = name length
	cmpb %cl,%al		// Length is the same?
	jne 2f

	// Compare the strings in detail.
	push %rcx		// Save the length
	push %rdi		// Save the address (repe cmpsb will move this pointer)
	lea 9(%rdx),%rsi	// Dictionary string we are checking against.
	repe cmpsb		// Compare the strings.
	pop %rdi
	pop %rcx
	jne 2f			// Not the same.

	// The strings are the same - return the header pointer in %eax
	pop %rsi
	mov %rdx,%rax
	ret

2:	mov (%rdx),%rdx		// Move back through the link field to the previous word
	jmp 1b			// .. and loop.

4:	// Not found.
	pop %rsi
	xor %rax,%rax		// Return zero to indicate not found.
	ret

	defcode ">CFA",4,,TCFA
	pop %rdi
	call _TCFA
	push %rdi
	NEXT
_TCFA:
	xor %rax,%rax
	add $8,%rdi		// Skip link pointer.
	movb (%rdi),%al		// Load flags+len into %al.
	inc %rdi		// Skip flags+len byte.
	andb $F_LENMASK,%al	// Just the length, not the flags.
	add %rax,%rdi		// Skip the name.
	addq $7,%rdi		// The codeword is 8-byte aligned.
	andq $~7,%rdi
	ret

/*
	Related to >CFA is >DFA which takes a dictionary entry address as returned by FIND and
	returns a pointer to the first data field.

	FIND returns a pointer to this
	|				>CFA converts it to a pointer to this
	|					   |
	|					   |	>DFA converts it to a pointer to this
	|					   |		 |
	V					   V		 V
	+---------+---+---+---+---+---+---+---+---+------------+------------+------------+------------+
	| LINK    | 6 | D | O | U | B | L | E | 0 | DOCOL      | DUP        | +          | EXIT       |
	+---------+---+---+---+---+---+---+---+---+------------+------------+------------+------------+
						   codeword

	(Note to those following the source of FIG-FORTH / ciforth: My >DFA definition is
	different from theirs, because they have an extra indirection).

	You can see that >DFA is easily defined in FORTH just by adding 4 to the result of >CFA.
*/

	defword ">DFA",4,,TDFA
	.int TCFA, 0		// >CFA		(get code field address)
	.int INCR8, 0		// 8+		(add 8 to it to get to next word)
	.int EXIT, 0		// EXIT		(return from FORTH word)

        
	// QUIT must not return (ie. must not call EXIT).
	defword "QUIT",4,,QUIT
	.int RZ,0,RSPSTORE,0	// R0 RSP!, clear the return stack
	.int INTERPRET, 0		// interpret the next word
	.int BRANCH,0, -16,-1		// and loop (indefinitely)
                                        // neg 1 because it is a 64 bit -16
        
	defcode "INTERPRET",9,,INTERPRET
	call _WORD		// Returns %ecx = length, %edi = pointer to word.

	// Is it in the dictionary?
	xor %r11,%r11 // r11 holds if it is a literal number
	call _FIND		// Returns %eax = pointer to header or 0 if not found.
	test %rax,%rax		// Found?
	jz 1f

	// In the dictionary.  Is it an IMMEDIATE codeword?
	mov %rax,%rdi		// %edi = dictionary entry
	movb 8(%rdi),%al	// Get name+flags.
	push %ax		// Just save it for now.
	call _TCFA		// Convert dictionary entry (in %edi) to codeword pointer.
	pop %ax
	andb $F_IMMED,%al	// Is IMMED flag set?
	mov %rdi,%rax
	jnz 4f			// If IMMED, jump straight to executing.

	jmp 2f

1:	// Not in the dictionary (not a word) so assume it's a literal number.
	incq %r11
	call _NUMBER		// Returns the parsed number in %eax, %ecx > 0 if error
	test %rcx,%rcx
	jnz 6f
	mov %rax,%rbx
	mov $LIT,%rax		// The word is LIT

2:	// Are we compiling or executing?
	movq STATE_OFFSET(%rbp),%rdx
	test %rdx,%rdx
	jz 4f			// Jump if executing.

	// Compiling - just append the word to the current dictionary definition.
	call _COMMA
	test %r11,%r11 // Was it a literal?
	jz 3f
	mov %rbx,%rax		// Yes, so LIT is followed by a number.
	call _COMMA 
3:	NEXT

4:	// Executing - run it!
	test %r11,%r11		// Literal?
	jnz 5f

	// Not a literal, execute it now.  This never returns, but the codeword will
	// eventually call NEXT which will reenter the loop in QUIT.
	jmp *(%rax)

5:	// Executing a literal, which means push it on the stack.
	push %rbx
	NEXT

6:	// Parse error (not a known word or a number in the current BASE).
	// Print an error message followed by up to 40 characters of context.
/*	mov $2,%rbx		// 1st param: stderr
	mov $errmsg,%rcx	// 2nd param: error message
	mov $errmsgend-errmsg,%rdx // 3rd param: length of string
	mov $__NR_write,%rax	// write syscall
	int $0x80

	mov $errmsgnl,%rcx	// newline
	mov $1,%rdx
	mov $__NR_write,%rax	// write syscall
	int $0x80
*/
        mov $3, %rdx
        call fpause
        
	NEXT


	defcode "CREATE",6,,CREATE

	// Get the name length and address.
	pop %rcx		// %ecx = length
	pop %rbx		// %ebx = address of name

	// Link pointer.
	mov HERE_OFFSET(%rbp),%rdi	// %edi is the address of the header
	mov LATEST_OFFSET(%rbp),%rax	// Get link pointer
	stosq			// and store it in the header.

	// Length byte and the word itself.
	mov %cl,%al		// Get the length.
	stosb			// Store the length/flags byte.
	push %rsi
	mov %rbx,%rsi		// %esi = word
	rep movsb		// Copy the word
	pop %rsi
	add $7,%rdi		// Align to next 8 byte boundary.
	and $~7,%rdi

	// Update LATEST and HERE.
	mov HERE_OFFSET(%rbp),%rax
	mov %rax,LATEST_OFFSET(%rbp)
	mov %rdi,HERE_OFFSET(%rbp)
	NEXT

/*
	Because I want to define : (COLON) in FORTH, not assembler, we need a few more FORTH words
	to use.

	The first is , (COMMA) which is a standard FORTH word which appends a 32 bit integer to the user
	memory pointed to by HERE, and adds 4 to HERE.  So the action of , (COMMA) is:

							previous value of HERE
								 |
								 V
	+---------+---+---+---+---+---+---+---+---+-- - - - - --+------------+
	| LINK    | 6 | D | O | U | B | L | E | 0 |             |  <data>    |
	+---------+---+---+---+---+---+---+---+---+-- - - - - --+------------+
                   len                         pad		              ^
									      |
									new value of HERE

	and <data> is whatever 32 bit integer was at the top of the stack.

	, (COMMA) is quite a fundamental operation when compiling.  It is used to append codewords
	to the current word that is being compiled.
*/

	defcode ",",1,,COMMA
	pop %rax		// Code pointer to store.
	call _COMMA
	NEXT
_COMMA:
	mov HERE_OFFSET(%rbp),%rdi	// HERE
	stosq			// Store it.
	mov %rdi,HERE_OFFSET(%rbp)	// Update HERE (incremented)
	ret

/*
	Our definitions of : (COLON) and ; (SEMICOLON) will need to switch to and from compile mode.

	Immediate mode vs. compile mode is stored in the global variable STATE, and by updating this
	variable we can switch between the two modes.

	For various reasons which may become apparent later, FORTH defines two standard words called
	[ and ] (LBRAC and RBRAC) which switch between modes:

	Word	Assembler	Action		Effect
	[	LBRAC		STATE := 0	Switch to immediate mode.
	]	RBRAC		STATE := 1	Switch to compile mode.

	[ (LBRAC) is an IMMEDIATE word.  The reason is as follows: If we are in compile mode and the
	interpreter saw [ then it would compile it rather than running it.  We would never be able to
	switch back to immediate mode!  So we flag the word as IMMEDIATE so that even in compile mode
	the word runs immediately, switching us back to immediate mode.
*/

	defcode "[",1,F_IMMED,LBRAC
	xor %rax,%rax
	movq %rax,STATE_OFFSET(%rbp)	// Set STATE to 0.
	NEXT

	defcode "]",1,,RBRAC
	movq $1,STATE_OFFSET(%rbp)	// Set STATE to 1.
	NEXT

/*
	Now we can define : (COLON) using CREATE.  It just calls CREATE, appends DOCOL (the codeword), sets
	the word HIDDEN and goes into compile mode.
*/

	defword ":",1,,COLON
	.int WORD, 0		// Get the name of the new word
	.int CREATE, 0		// CREATE the dictionary entry / header
	.int LIT, 0, DOCOL, 0, COMMA, 0	// Append DOCOL  (the codeword).
	.int LATEST, 0, FETCH, 0, HIDDEN, 0 // Make the word hidden (see below for definition).
	.int RBRAC, 0		// Go into compile mode.
	.int EXIT, 0		// Return from the function.

/*
	; (SEMICOLON) is also elegantly simple.  Notice the F_IMMED flag.
*/

	defword ";",1,F_IMMED,SEMICOLON
	.int LIT, 0, EXIT, 0, COMMA, 0	// Append EXIT (so the word will return).
	.int LATEST, 0, FETCH, 0, HIDDEN, 0 // Toggle hidden flag -- unhide the word (see below for definition).
	.int LBRAC, 0		// Go back to IMMEDIATE mode.
	.int EXIT, 0		// Return from the function.

/*
	EXTENDING THE COMPILER ----------------------------------------------------------------------

	Words flagged with IMMEDIATE (F_IMMED) aren't just for the FORTH compiler to use.  You can define
	your own IMMEDIATE words too, and this is a crucial aspect when extending basic FORTH, because
	it allows you in effect to extend the compiler itself.  Does gcc let you do that?

	Standard FORTH words like IF, WHILE, ." and so on are all written as extensions to the basic
	compiler, and are all IMMEDIATE words.

	The IMMEDIATE word toggles the F_IMMED (IMMEDIATE flag) on the most recently defined word,
	or on the current word if you call it in the middle of a definition.

	Typical usage is:

	: MYIMMEDWORD IMMEDIATE
		...definition...
	;

	but some FORTH programmers write this instead:

	: MYIMMEDWORD
		...definition...
	; IMMEDIATE

	The two usages are equivalent, to a first approximation.
*/

	defcode "IMMEDIATE",9,F_IMMED,IMMEDIATE
	movq LATEST_OFFSET(%rbp),%rdi	// LATEST word.
	addq $8,%rdi		// Point to name/flags byte.
	xorb $F_IMMED,(%rdi)	// Toggle the IMMED bit.
	NEXT

/*
	'addr HIDDEN' toggles the hidden flag (F_HIDDEN) of the word defined at addr.  To hide the
	most recently defined word (used above in : and ; definitions) you would do:

		LATEST @ HIDDEN

	'HIDE word' toggles the flag on a named 'word'.

	Setting this flag stops the word from being found by FIND, and so can be used to make 'private'
	words.  For example, to break up a large word into smaller parts you might do:

		: SUB1 ... subword ... ;
		: SUB2 ... subword ... ;
		: SUB3 ... subword ... ;
		: MAIN ... defined in terms of SUB1, SUB2, SUB3 ... ;
		HIDE SUB1
		HIDE SUB2
		HIDE SUB3

	After this, only MAIN is 'exported' or seen by the rest of the program.
*/

	defcode "HIDDEN",6,,HIDDEN
	pop %rdi		// Dictionary entry.
	add $8,%rdi		// Point to name/flags byte.
	xorb $F_HIDDEN,(%rdi)	// Toggle the HIDDEN bit.
	NEXT
        
	defword "HIDE",4,,HIDE
	.int WORD, 0		// Get the word (after HIDE).
	.int FIND, 0		// Look up in the dictionary.
	.int HIDDEN, 0		// Set F_HIDDEN flag.
	.int EXIT, 0		// Return.

        /*
	' (TICK) is a standard FORTH word which returns the codeword pointer of the next word.

	The common usage is:

	' FOO ,

	which appends the codeword of FOO to the current word we are defining (this only works in compiled code).

	You tend to use ' in IMMEDIATE words.  For example an alternate (and rather useless) way to define
	a literal 2 might be:

	: LIT2 IMMEDIATE
		' LIT ,		\ Appends LIT to the currently-being-defined word
		2 ,		\ Appends the number 2 to the currently-being-defined word
	;

	So you could do:

	: DOUBLE LIT2 * ;

	(If you don't understand how LIT2 works, then you should review the material about compiling words
	and immediate mode).

	This definition of ' uses a cheat which I copied from buzzard92.  As a result it only works in
	compiled code.  It is possible to write a version of ' based on WORD, FIND, >CFA which works in
	immediate mode too.
*/
	defcode "'",1,,TICK
	lodsq			// Get the address of the next word and skip it.
	push %rax		// Push it on the stack.
	NEXT

/*
	LITERAL STRINGS ----------------------------------------------------------------------

	LITSTRING is a primitive used to implement the ." and S" operators (which are written in
	FORTH).  See the definition of those operators later.

*/

	defcode "LITSTRING",9,,LITSTRING
	lodsq			// get the length of the string
	push %rsi		// push the address of the start of the string
	push %rax		// push length on the stack
	addq %rax,%rsi		// skip past the string
 	addq $7,%rsi		// but round up to next 4 byte boundary
	andq $~7,%rsi
	NEXT
        
	defcode "CHAR",4,,CHAR
	call _WORD		// Returns %ecx = length, %edi = pointer to word.
	xor %rax,%rax
	movb (%rdi),%al		// Get the first character of the word.
	push %rax		// Push it onto the stack.
	NEXT        

        defcode "EXECUTE",7,,EXECUTE
	pop %rax		// Get xt into %eax
	jmp *(%rax)		// and jump to it.

        
        //this works with most things but not rax or rbp
	.macro SWAP mem, reg
        movq    \reg, %rax
        movq    \mem(%rbp), \reg
        movq    %rax, \mem(%rbp)
        .endm

        defcode "FOO",3,,FOO
        NEXT
        
        defcode "PAUSE_WITH_CODE",15,,PAUSECODE
        pop %rdx
        call fpause
        NEXT
        
fpause:
        mov     %r9, RSTACK_TOP(%rbp)
        mov     %rsi, OLD_RSI(%rbp)
        SWAP    OLD_RBX, %rbx
        //SWAP    OLD_RDI, %rdi
        //SWAP    OLD_ESI, %esi
        SWAP	STACK_TOP, %rsp
        //modified swap for ebp
        //movl    %rbp, %rax
        //movl    OLD_EBP(%ebp), %ebp
        movq    OLD_RBP(%rbp), %rbp

        // the caller to this code should have put
        // its desired return code in rdx
        mov    %rdx, %rax
        ret


        
        // THIS NEEDS TO BE THE LAST WORD
        .globl  name_SYSCALL0
	defcode "SYSCALL0",8,,SYSCALL0
	pop %rax		// System call number (see <asm/unistd.h>)
	int $0x80
	push %rax		// Result (negative for -errno)
	NEXT

        
        
        .globl fstart
fstart: 
        mov     $cold_start, %esi
        NEXT

        .globl fcontinue
fcontinue:
        mov     %rbp,OLD_RBP(%rdi)
        mov     %rdi,%rbp // make rbp the root of data struct
        SWAP	STACK_TOP, %rsp
        //mov     %rsp,OLD_RSP(%rdi)
        //mov     %rbp,OLD_RBP(%rdi)
        mov     %rbx,OLD_RBX(%rbp)
        mov     OLD_RSI(%rbp), %rsi        
        mov     RSTACK_TOP(%rbp), %r9
        //mov     $cold_start, %esi
testme: 
        ret
cold_start:
        .int QUIT, 0
