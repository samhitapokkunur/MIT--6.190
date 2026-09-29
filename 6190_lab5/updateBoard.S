.section .text     
.align 2   
.globl updateBoard

# updateBoard
# ARGUMENTS a0: screen buffer (current board), a1: temporary output buffer (for new board)
# RETURNS: Nothing
updateBoard:
    
    # TO-DO: Translate the C implementation of updateBoard to assembly. 
    # Make sure to follow RISC-V calling convention!

    ret

# getPixel
# ARGUMENTS a0: screen_buffer, a1: x, a2: y
# RETURNS: 1 if cell occupied, 0 otherwise
getPixel:
    # your code here
    slli a2, a2, 2  # a2: 4 * y for address 
    add a2, a2, a0  # a2: screen_buffer + 4*y 
    lw a3, 0(a2)    # a3: screen_buffer[y]
    srl a3, a3, a1  # a3: screen_buffer[y] >>_l x
    andi a0, a3, 1  # a0: a3 & 1
    ret

# checkNeighbors
# ARGUMENTS a0: game_board, a1: x index, a2: y index
# RETURNS: total occupied cells in the eight surrounding cells of (x,y) (game board wraps in x and y)
checkNeighbors: 
    # TODO: Determine left, right, up, and down indices for neighbors.

    call tallyNeighbors # tallyNeighbors(game_board, x, y, left, right, up, down)

    # TODO: Return result of tallyNeighbors

    ret

# tallyNeighbors
# ARGUMENTS a0: game_board, a1: current x index, a2: current y index, a3: left neighbor index, 
# a4: right neighbor index, a5: up neighbor index, a6: down neighbor index 
# RETURNS: total occupied cells in the eight surrounding cells of current (x,y)
tallyNeighbors:
    # TODO: This procedure is functionally correct, but doesn't follow RISC-V calling convention.
    # Make this procedure follow calling convention. You may only add instructions that: 
    # 1. Increment/decrement the stack pointer
    # 2. Put elements on the stack
    # 3. Take elements off the stack.
    
    mv s0, a0 # s0: game_board
    mv s1, a1 # a1: x
    mv s2, a2 # a2: y
    li s3, 0  # s3: tally
 
    mv a1, a4
    mv a2, a5
    call getPixel # getPixel(game_board, x-1, y-1) 
    add s3, s3, a0

    mv a0, s0
    mv a1, a4
    mv a2, s2
    call getPixel # getPixel(game_board, x-1, y)
    add s3, s3, a0

    mv a0, s0
    mv a1, a4
    mv a2, a6
    call getPixel # getPixel(game_board, x-1, y+1)
    add s3, s3, a0

    mv a0, s0
    mv a1, s1
    mv a2, a5
    call getPixel # getPixel(game_board, x, y-1)
    add s3, s3, a0

    mv a0, s0
    mv a1, s1
    mv a2, a6
    call getPixel # getPixel(game_board, x, y+1)
    add s3, s3, a0

    mv a0, s0
    mv a1, a3   
    mv a2, a5
    call getPixel # getPixel(game_board, x+1, y-1)
    add s3, s3, a0

    mv a0, s0
    mv a1, a3     
    mv a2, s2
    call getPixel # getPixel(game_board, x+1, y)
    add s3, s3, a0

    mv a0, s0
    mv a1, a3   
    mv a2, a6
    call getPixel # getPixel(game_board, x+1, y+1)
    add s3, s3, a0

    mv a0, s3

    ret