.global _start
.extern main
.extern exit
.extern environ
.extern _init_signal
.globl open

_start:
	# Setup local signal handling
	call _init_signal
	
	# Grab the environment pointer and store it in the respective global
	movl 0x8(%esp),%eax
	movl %eax,environ
	
	# argc, argv, and envp are already on the stack.
	call main
	# Main returned an integer in eax
	pushl %eax
	# Quit the program if main returned
	call exit
# This should never happen unless the kernel fucks up
.wait:
	hlt
	jmp .wait 
