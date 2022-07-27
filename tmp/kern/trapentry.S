/* See COPYRIGHT for copyright information. */

#include <inc/mmu.h>
#include <inc/memlayout.h>
#include <inc/trap.h>

#include <kern/picirq.h>


###################################################################
# exceptions/interrupts
###################################################################

/* TRAPHANDLER defines a globally-visible function for handling a trap.
 * It pushes a trap number onto the stack, then jumps to _alltraps.
 * Use TRAPHANDLER for traps where the CPU automatically pushes an error code.
 *
 * You shouldn't call a TRAPHANDLER function from C, but you may
 * need to _declare_ one in C (for instance, to get a function pointer
 * during IDT setup).  You can declare the function with
 *   void NAME();
 * where NAME is the argument passed to TRAPHANDLER.
 */

 #define NOTHINGBUTUGLY()         \
	

#define TRAPHANDLER(name, num)						\
.data;  \
        .long name; \
.text;  \
	.globl name;		/* define global symbol for 'name' */	\
	.type name, @function;	/* symbol type is function */		\
	.align 2;		/* align function definition */		\
	name:			/* function starts here */		\
	.if !(num == 8 || num == 17 || (num >= 10 && num <= 14));   \
	pushl $0;   \
	.endif;     \
	pushl $(num);							\
	jmp _alltraps

/* Use TRAPHANDLER_NOEC for traps where the CPU doesn't push an error code.
 * It pushes a 0 in place of the error code, so the trap frame has the same
 * format in either case.
 */
 /*
#define TRAPHANDLER_NOEC(name, num)					\
	.globl name;							\
	.type name, @function;						\
	.align 2;							\
name:								\
	pushl $0;							\
	pushl $(num);							\
	jmp _alltraps
	*/

#define TRAPHANDLER_NOEC(name, num)					\
.data;  \
	.long name;  \
.text;\
	.globl name;							\
	.type name, @function;						\
	.align 2;							\
	name:								\
	pushl $0;							\
	pushl $(num);							\
	jmp _alltraps



.data
	.globl funs



/*
 * Lab 3: Your code here for generating entry points for the different traps.
 */
funs:

TRAPHANDLER(funs0, 0)
    TRAPHANDLER(funs1, 1)
    TRAPHANDLER(funs2, 2)
    TRAPHANDLER(funs3, 3)
    TRAPHANDLER(funs4, 4)
    TRAPHANDLER(funs5, 5)
    TRAPHANDLER(funs6, 6)
    TRAPHANDLER(funs7, 7)
    TRAPHANDLER(funs8, 8)
    TRAPHANDLER(funs9, 9)
    TRAPHANDLER(funs10, 10)
    TRAPHANDLER(funs11, 11)
    TRAPHANDLER(funs12, 12)
    TRAPHANDLER(funs13, 13)
    TRAPHANDLER(funs14, 14)
    TRAPHANDLER(funs15, 15)
    TRAPHANDLER(funs16, 16)
    TRAPHANDLER(funs17, 17)
    TRAPHANDLER(funs18, 18)
    TRAPHANDLER(funs19, 19)
    TRAPHANDLER(funs20, 20)
    TRAPHANDLER(funs21, 21)
    TRAPHANDLER(funs22, 22)
    TRAPHANDLER(funs23, 23)
    TRAPHANDLER(funs24, 24)
    TRAPHANDLER(funs25, 25)
    TRAPHANDLER(funs26, 26)
    TRAPHANDLER(funs27, 27)
    TRAPHANDLER(funs28, 28)
    TRAPHANDLER(funs29, 29)
    TRAPHANDLER(funs30, 30)
    TRAPHANDLER(funs31, 31)
    TRAPHANDLER(funs32, 32)
    TRAPHANDLER(funs33, 33)
    TRAPHANDLER(funs34, 34)
    TRAPHANDLER(funs35, 35)
    TRAPHANDLER(funs36, 36)
    TRAPHANDLER(funs37, 37)
    TRAPHANDLER(funs38, 38)
    TRAPHANDLER(funs39, 39)
    TRAPHANDLER(funs40, 40)
    TRAPHANDLER(funs41, 41)
    TRAPHANDLER(funs42, 42)
    TRAPHANDLER(funs43, 43)
    TRAPHANDLER(funs44, 44)
    TRAPHANDLER(funs45, 45)
    TRAPHANDLER(funs46, 46)
    TRAPHANDLER(funs47, 47)
    TRAPHANDLER(funs48, 48)

/*
 * Lab 3: Your code here for _alltraps
 */


_alltraps:
	pushl	%ds
	pushl	%es
	pushal
	pushl	$GD_KD
	popl	%ds
	pushl	$GD_KD
	popl	%es
	pushl	%esp
	call	trap
	