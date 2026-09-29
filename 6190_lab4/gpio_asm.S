.section .text      # indicates a code segment
.align 2            # specifies how data should be arranged in memory
//.globl pinSetup   # makes pinSetup able to be used outside of this file
.globl pinWrite     # makes pinWrite able to be used outside of this file
.globl pinRead      # makes pinRead able to be used outside of this file
.globl setPixel     # makes setPixel able to be used outside of this file
.globl eraseBuffer  # makes eraseBuffer able to be used outside of this file

# C declaration: int pinRead(int pin_num)
# ARGUMENTS a0: pin_num
# RETURNS bit read from GPIO pin
pinRead:
    lui a2, 0x60004
    addi a2, a2, 0x3C # GPIO_IN_ADDR 

    # YOUR CODE HERE...

    jalr x0, 0(ra)


# C declaration: void pinWrite(int pin_num, int value)
# ARGUMENTS a0: pin_num, a1: value
# RETURNS: Nothing
pinWrite:
    lui a2, 0x60004
    addi a2, a2, 0x4  # GPIO_OUT_ADDR

    # YOUR CODE HERE...

    jalr x0, 0(ra)


# C declaration: void pinSetup(int pin_num, int mode)
# ARGUMENTS a0: pin_num, a1: mode
# RETURNS: Nothing
pinSetup:
    lui a2, 0x60009
    addi a2, a2, 0x4  # IO_MUX_GPIOn_ADDR

    lui a3, 0x60004
    addi a3, a3, 0x20 # GPIO_ENABLE_ADDR 

    # YOUR CODE HERE...

    jalr x0, 0(ra)


# C declaration: void setPixel(uint32_t* screen_buffer_addr, uint8_t x, uint8_t y, uint8_t val);
# ARGUMENTS a0: screen_buffer base address, a1: x, a2: y, a3: val
# RETURNS: Nothing
setPixel:
    # YOUR CODE HERE...

    jalr x0, 0(ra)


# C declaration: void eraseBuffer(uint32_t* screen_buffer_addr)
# ARGUMENTS a0: screen_buffer base address
# RETURNS: Nothing

eraseBuffer:
    addi a1, x0, 8      # upper bound on for loop
    addi a2, x0, 0      # "i" for for loop
looping:
    slli a3, a2, 2      # calculate 4*i
    add a4, a0, a3      # get address of array element by adding base address + 4*i
    sw zero, 0(a4)      # write 0 to memory address
    addi a2, a2, 1      # increment i
    bne a2, a1, looping # continue looping if i < 8
    jalr x0, 0(ra)      # return from eraseBuffer