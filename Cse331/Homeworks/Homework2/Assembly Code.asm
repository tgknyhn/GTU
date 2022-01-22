.data
	# Input Arrays
	arr:		.word	3, 10, 7, 9, 4, 11						
	empty:		.word	0					# Empty words are necessary or getSize procedure don't work properly
	arr2:		.word	2, 5, 3, 95, 24, 1, 22, 10, 88
	empty2:		.word	0					# When you remove empty words, arr2 comes right after arr 
	arr3:		.word	4, 11, 59, 12, 53, 51 , 22 , 44 , 5
	empty3:		.word	0					# Because of that getSize doesn't understand that array is finished
	arr4:		.word	11, 4, 9, 10, 3, 7, 51, 14, 22
	empty4:		.word	0					# getSize  looks if the current value is between 0 < x < 100
	arr5:		.word	90, 80, 70, 60, 50, 40, 30, 20, 10, 1
	empty5:		.word	0
	arr6:		.word	1, 2, 3, 4, 5
	empty6:		.word	0									
	seqArr:		.word	0:10		# Sequence array can have most 10 elements since max array size is 10
	lisArr:		.word	0:10		# This array holds biggest seqArr for given arr
	# ASCII Strings
	newLine:	.asciiz "\n"
	space:		.asciiz	" "
	prompt:		.asciiz	"Longest Increasing Subsequence Array : "
	size:		.asciiz	"LIS Size: "
	seqSize:	.asciiz ", Size : "
.text	
.globl main

main:
	## ------------------------- 1. Array ------------------------ ##
	# 1) Find size of the array
	la	$a0,	arr		# a0 = address of the array
	jal	getSize			# v0 = size of the array
	# 2) Find  subsequences of arr and print them
	la 	$a1,	arr		# a1 = address of the array
	move	$a2,	$v0		# a2 = v0 (came from getSize)
	la	$a3,	seqArr		# a3 = address of the seqArr
	jal	findLIS			# Finds Longest Increasing Subsequence (prints rest of them)	
	# 3) Print Longest Increasing Subsequence Array
	la	$a0,	lisArr		# a0 = lisArr address
	move	$a1,	$v0		# v0 = size of the lisArr (returned from findLIS)
	jal	printLongest		# Prints lisArr and it's size to the console
	
	## ------------------------- 2. Array ------------------------ ##
	# 1) Find size of the array
	la	$a0,	arr2		# a0 = address of the array
	jal	getSize			# v0 = size of the array
	# 2) Find  subbsequences of arr and print themhem
	la 	$a1,	arr2		# a1 = address of the array
	move	$a2, 	$v0		# a2 = v0 (came from getSize)rocedure
	la	$a3, 	seqArr		# a3 = address of the seqArr
	jal	findLIS			# Finds Longest Increasing Subsequence (prints rest of them)
	# 3) Print Longest Increasing Subsequence Array
	la	$a0,	lisArr		# a0 = lisArr address
	move	$a1,	$v0		# v0 = size of the lisArr (returned from findLIS)
	jal	printLongest		# Prints lisArr and it's size to the console
	

	## ------------------------- 3. Array ------------------------ ##
	# 1) Find size of the array
	la	$a0,	arr3		# a0 = address of the array
	jal	getSize			# v0 = size of the array
	# Find  subsequences of arr and print them
	la 	$a1,	arr3		# a1 = address of the array
	move	$a2, 	$v0		# a2 = v0 (came from getSize)rocedure
	la	$a3, 	seqArr		# a3 = address of the seqArr
	jal	findLIS			# Finds Longest Increasing Subsequence (prints rest of them)
	# Print Longest Increasing Subsequence Array
	la	$a0,	lisArr		# a0 = lisArr address
	move	$a1,	$v0		# v0 = size of the lisArr (returned from findLIS)
	jal	printLongest		# Prints lisArr and it's size to the console
	
	## ------------------------- 4. Array ------------------------ ##
	# 1) Find size of the array
	la	$a0,	arr4		# a0 = address of the array
	jal	getSize			# v0 = size of the array
	# Find  subsequences of arr and print them
	la 	$a1,	arr4		# a1 = address of the array
	move	$a2, 	$v0		# a2 = v0 (came from getSize)rocedure
	la	$a3, 	seqArr		# a3 = address of the seqArr
	jal	findLIS			# Finds Longest Increasing Subsequence (prints rest of them)
	# Print Longest Increasing Subsequence Array
	la	$a0,	lisArr		# a0 = lisArr address
	move	$a1,	$v0		# v0 = size of the lisArr (returned from findLIS)
	jal	printLongest		# Prints lisArr and it's size to the console
	
	
	## ------------------------- 5. Array ------------------------ ##
	# 1) Find size of the array
	la	$a0,	arr5		# a0 = address of the array
	jal	getSize			# v0 = size of the array
	# Find  subsequences of arr and print them
	la 	$a1,	arr5		# a1 = address of the array
	move	$a2, 	$v0		# a2 = v0 (came from getSize)rocedure
	la	$a3, 	seqArr		# a3 = address of the seqArr
	jal	findLIS			# Finds Longest Increasing Subsequence (prints rest of them)
	# Print Longest Increasing Subsequence Array
	la	$a0,	lisArr		# a0 = lisArr address
	move	$a1,	$v0		# v0 = size of the lisArr (returned from findLIS)
	jal	printLongest		# Prints lisArr and it's size to the console
	
	
	## ------------------------- 6. Array ------------------------ ##
	# 1) Find size of the array
	la	$a0,	arr6		# a0 = address of the array
	jal	getSize			# v0 = size of the array
	# Find  subsequences of arr and print them
	la 	$a1,	arr6		# a1 = address of the array
	move	$a2,	$v0		# a2 = v0 (came from getSize)rocedure
	la	$a3,	seqArr		# a3 = address of the seqArr
	jal	findLIS			# Finds Longest Increasing Subsequence (prints rest of them)
	# Print Longest Increasing Subsequence Array
	la	$a0,	lisArr		# a0 = lisArr address
	move	$a1,	$v0		# v0 = size of the lisArr (returned from findLIS)
	jal	printLongest		# Prints lisArr and it's size to the console
	
	# Return 0
	li	$v0,	10		# Termination op.
	syscall				# Terminate
	
# a1 : arr address
# a2 : arr size
# a3 : seqArr address
# v0 : size of the longest increasing subsequence array
findLIS:
	# initializing variables
	li	$s0,	0		# s0 = size of the lisArr
	la	$s1,	($a1)		# s1 = address of arr
	move	$s2,	$a2		# s2 = size of the arr
	li	$s3,	0		# s3 = 0 (arr index)
	li	$s4,	0		# s4 = 0 (current subset number)
	li	$t0,	1		# t0 = 1
	sllv	$s5, 	$t0,	$s2	# s5 = 2^n (number of subsets arr can have)
	
	la	$s6,	($a3)		# s6 = address of the seqArr
	li	$s7,	0		# s7 = size of the seqArr
	
	
	while_LIS:
		# End condition - while
		slt 	$t1,	$s4,	$s5	# t1 = (s4 < s5) ? 1:0
		beqz	$t1,	endWhile_LIS	# if t1 == 0 then finish the outer loop
		# inside while loop
		li	$t2,	0		# t2 => i = 0
		for_LIS: 			# for(i = 0; i<n; i++)
			# End condition - for
			slt	$t3,	$t2,	$s2		# t3 = (t2 < s2) ? 1:0
			beq	$t3,	0,	endFor_LIS	# if (t3 == 0) then finish for loop			
			# inside for loop
			if_LIS:
				li	$t0,	1		# t1 = 1
				sllv	$t4,	$t0,	$t2	# Shifting t0 value by t2 times and putting the result in t4
				and	$t5,	$t4,	$s4	# t4 AND s4 -> t5
				# if condition check
				beqz	$t5,	endIf_LIS	# if t5 is equal to 0 then finish for loop without printing

				## Succesful, adding the value to seqArr ##
				# getting value
				sll		$t6,	$t2,	2		# t6 = t2 * 4 because next element address not after 1 byte, but 4 byte
				add		$s1,	$s1,	$t6		# Next element in arr
				lw		$a0,	($s1)			# a0 = s1[i]
				sub		$s1,	$s1,	$t6		# Previous element in arr
					
				# Storing it in the seqArr
				add		$s6,	$s6,	$s7		# seqArr = seqArr + seqArrIndex
				sw		$a0,	($s6)			# seqArr[seqArrIndex] = a0
				sub		$s6,	$s6,	$s7		# seqArr = seqArr + seqArrIndex
				add		$s7,	$s7,	4		# seqArrIndex = seqArrIndex + 4
			
			endIf_LIS:
				addi		$t2,	$t2,	1		# i = i + 1
				j		for_LIS				
			
		endFor_LIS:
			srl	$s7,	$s7,	2	# seqArrIndex / 4 = size of the seqArr
			
			# Controlling if the seqArray is in incrementing order
			sub	$sp,	$sp,	4	# allocating memory for the ra
			sw	$ra,	($sp)		# storing ra in stack
			move	$a1,	$s7		# a1 = size of the seqArr
			la	$a0,	($s6)		# a2 = address of the seqArr
			jal	isIncrementing
			lw	$ra,	($sp)		# getting back the ra
			add	$sp,	$sp,	4	# closing stack
			
			# Flag
			beqz	$v0,	skip_print
			
			# If array size is greater than the previous one, then set this array as longest increasing subsequence array
			slt	$t0,	$s0,	$s7	# if seqArr size > lisArr size than set t0 to 1
			beqz	$t0,	skip_longestArray
			
			move	$s0,	$s7		# lisSize = seqSize
			
			sub	$sp,	$sp,	4	# allocating memory for the ra
			sw	$ra,	($sp)		# storing ra in stack
			la	$a0,	($s6)		# a0 = address of the seqArr
			la	$a1,	lisArr		# a1 = address of the lisArr
			move	$a2,	$s7		# a2 = size of the seqArr
			jal	copyArray
			lw	$ra,	($sp)		# getting back the ra
			add	$sp,	$sp, 4		# closing stack
			
			skip_longestArray:
			
			# Printing seqArray
			sub	$sp,	$sp,	4	# allocating memory for the ra
			sw	$ra,	($sp)		# storing ra in stack
			move	$a1,	$s7		# a1 = size of the seqArr
			la	$a2,	($s6)		# a2 = address of the seqArr
			jal	printArray
			lw	$ra,	($sp)		# getting back the ra
			add	$sp,	$sp, 4		# closing stack
			
			skip_print:
			
			# Increment current subset number
			addi	$s4,	$s4,	1	# s4 = s4 + 1
			# Reset seqArr Index
			li	$s7,	0		# seqArrIndex = 0
			# Jump to while
			j 	while_LIS 
		
	endWhile_LIS:
		move	$v0,	$s0			# s0 = size of the lisArr
		# return to return address
		jr	$ra	

# a0 : Address of the lisArr
# a1 : Size of the lisArr
printLongest:
	la	$t0,	($a0)		# t0 = address of the arr
	move	$t1,	$a1		# t1 = size of the arr
	
	# Print promp
	li	$v0,	4		# print asciiz
	la	$a0,	prompt		# a0 = "Longest Increasing Subsequence Array : "
	syscall

	# Print Array
	sub	$sp,	$sp,	4	# Allocate space for ra in stack
	sw	$ra,	($sp)		# Store ra in stack
	move	$a1,	$t1		# argument 1 = size of the lisArr
	la	$a2, ($t0)		# argument 2 = address of the lisArr
	jal	printArray
	lw	$ra,	($sp)		# Get ra back from the stack
	add	$sp,	$sp,	4	# close stack
	
	# Printing new line
	li	$v0,	4		# Print string op.
	la	$a0,	newLine		# a0 = t3 (current value)
	syscall				# Print
	
	jr	$ra

# a0 : arr address
# a1 : copy arr address
# a2 : size of the arr
copyArray:
	la	$t0,	($a0)		# t0 = arr address
	la	$t1,	($a1)		# t1 = copy arr address
	move	$t2,	$a2		# t2 = size of the arr
	sll	$t2,	$t2,	2	# t2 = t2*4 (easy to increment address)
	
	li	$t3,	0		# t3 = i
	copyFor:
		beq	$t2,	$t3,	endCopyFor 	# if (i ==n) then break the loop
		
		add	$t0,	$t0,	$t3		# arr = arr + i
		add	$t1,	$t1,	$t3		# copyArr = copyArr + i
		lw	$t4,	($t0)			# t4 = arr[i]
		sw	$t4,	($t1)			# copyArr[i] = t4
		sub	$t0,	$t0,	$t3		# arr = arr - i
		sub	$t1,	$t1,	$t3		# copyArr = copyArr - i
		
		addi	$t3,	$t3,	4		# i = i + 4
		
		j	copyFor
		
	endCopyFor:	
		jr	$ra				# Return to return address

		
# a0 : array address
# a1 : size of the array
# v0 : equals 0 if it's not incrementing, equals 1 if it's incrementing
isIncrementing:
	la	$t0,	($a0)		# t0 = Address of array
	move	$t1,	$a1		# t1 = size of the array
	sll	$t1,	$t1,	2	# t1 = t1 * 4 (easy to use inside the procedure) 
	
	li	$t2,	0		# t2 = i
	li	$t3,	0		# t3 = j	
	li	$t4,	1		# t4 = flag (initially 1)	
	for1:
		beq	$t2,	$t1,	endFor1		# if i ==size of the array, then end the for1 loop
		addi	$t3,	$t2,	4		# j = i + 4
		for2:
			beq	$t3,	$t1,	endFor2		# if j ==size of the array, then end the for2 loop
			
			# Condition check
			add	$t0,	$t0,	$t2		# arr + i	
			lw	$t5,	($t0)			# t5 = arr[i]
			sub	$t0,	$t0,	$t2		# arr - i
			
			add	$t0,	$t0,	$t3		# arr + j	
			lw	$t6,	($t0)			# t6 = arr[j]
			sub	$t0,	$t0,	$t3		# arr - j
			
			slt	$t7,	$t6,	$t5		# if(arr[j] < arr[i]) then flag = 0
			beqz	$t7,	endIf_II
					
			li	$t4,	0			# flag = 0

			endIf_II:			
				addi	$t3,	$t3,	4	# j = j + 4
		endFor2:
			addi	$t2,	$t2,	4		# i = i + 4
			j	for1
	endFor1:
		move	$v0,	$t4	# assign flag to v0
		jr	$ra		# return to return address
		
# v0 : Returns size of the array
# a0 : Array Address
getSize:
	# Initializing variables
	li	$t4,	100		# Highest value for an element
	la	$t0,	($a0)		# t0 = a0 (Array address)
	addi	$t1,	$0,	0	# t1 = 0 	(Array size)
	addi	$t2,	$0,	1	# t2 = 1 	(current value)	
	
	calculateSize:
		# When condition not satisfied
		lw	$t2,	($t0)			# t2 = arr[index]
		slt	$t3,	$t2,	$t4		# t3 = (t2 < t4) ? 1:0
		# Return condition
		beqz 	$t2,	returnSize		# if t2 == 0 then finish
		beqz	$t3, 	returnSize		# if t3 > 100 then finish
		# When condition not satisfied		
		addi	$t0,	$t0,	4		# Next array element
		addi	$t1,	$t1,	1		# Increment size by 1
		# Jump back
		j calculateSize				# loop	
	
	#Finish procedure
	returnSize:
		addi	$v0,	$t1,	0		# v0 = size of the array
		jr	$ra				# return to return address
	
	
# a1 : Size of the array
# a2 : Array Address
printArray:
	# Initializing variables
	la	$t0,	($a2)				# t0 = a0 (Array address)
	addi	$t1,	$a1,	0			# t1 = a1 	(Array size)
	addi	$t2,	$0,	0			# t2 = 0	(Current size)
	addi	$t3,	$0,	0			# t3 = 0	(Current value)
	
	print:
		# Base condition
		beq	$t1,	$t2,	endPrint	# if current size == array size then finish printing
		# Getting current value
		lw	$t3,	($t0)			# t3 = array[index]
		# Printing the value
		li	$v0,	1			# Print string op.
		addi	$a0,	$t3,	0		# a0 = t3 (current value)
		syscall					# Print
		# Printing space
		li	$v0,	4			# Print string op.
		la	$a0,	space			# a0 = t3 (current value)
		syscall					# Print

		# increment & loop
		addi	$t2,	$t2,	1		# t2 = t2 + 1
		addi	$t0,	$t0,	4		# Next element address
		j	print				# loop
		
	endPrint:
		# Print Size
		li	$v0,	4			# Print string op.
		la	$a0,	seqSize			# a0 = ", Size : "
		syscall					# Print
		
		li	$v0,	1			# Print string op.
		move	$a0,	$t1			# a0 = size of the array
		syscall					# Print
		
		# Printing new line
		li	$v0,	4			# Print string op.
		la	$a0,	newLine			# a0 = t3 (current value)
		syscall					# Print
		
		jr	$ra				# return to return address
