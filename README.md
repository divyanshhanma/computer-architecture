# computer-architecture
My CSN-221 project

- **Project details :**

- **Following are the set of instructions which will my simulator supports :**
    
    immediate
    
    - **`addi`** (Add Immediate): Opcode 0010011, funct3 000.
    - **`ori`** (OR Immediate): Opcode 0010011, funct3 110.
    - **`andi`** (AND Immediate): Opcode 0010011, funct3 111.
    - **`lw`** Opcode: 0000011 (I-type), funct3: 010
    
    register
    
    - **`add`** (Addition): Opcode 0110011, funct3 000, funct7 0000000.
    - **`sub`** (Subtraction): Opcode 0110011, funct3 000, funct7 0100000.
    - **`and`** (Bitwise AND): Opcode 0110011, funct3 111, funct7 0000000.
    - **`or`** (Bitwise OR): Opcode 0110011, funct3 110, funct7 0000000
    
    branch
    
    - **`beq`** (Branch if Equal):Opcode: 1100011 (B-type), funct3: 000
    - **`bne`** (Branch if Not Equal):Opcode: 1100011 (B-type), funct3: 001
    - **`blt`** (Branch if Less Than):Opcode: 1100011 (B-type), funct3: 100
    - **`bge`** (Branch if Greater Than or Equal):Opcode: 1100011 (B-type), funct3: 101  (not included)
    
    jump
    
    - **`jal`**Opcode: 1101111 (J-type)
    
    store
    
    - **`sw`** (Store Word):Opcode: 0100011 (S-type), funct3: 010
 
    **- Following are few example codes**
    
    **GCD and LCM :**
    
    1.  calculates gcd and lcm of two nos. stored in mem location 0x00000000 **{ mem[0] in my code }** and 0x00000004 **{ mem[1] in my code }** 
    2. gcd stored in mem location -- 0x00000008 **{ mem[2] in my code }**  and lcm stored in mem location -- 0x0000000c **{ mem[3] in my code }**
    
    ```matlab
    addi x5,x0,{number1}
    addi x4,x0,{number2}
    sw x4,4,x0
    sw x5,0,x0
    blt x4,x5,20
    blt x5,x4,4
    lw x1,4,x0
    lw x2,0,x0
    jal x4,12
    lw x1,0,x0
    lw x2,4,x0
    sub x1,x1,x2
    blt x2,x1,-4
    beq x1,x2,20
    add x4,x0,x1
    add x1,x0,x2
    add x2,x0,x4
    jal x5,-24
    sw x2,8,x0
    lw x1,4,x0
    lw x2,0,x0
    add x3,x0,x0
    add x3,x3,x2
    addi x1,x1,-1
    bne x1,x0,-8
    lw x1,8,x0
    add x5,x0,x0
    sub x3,x3,x1
    addi x5,x5,1
    bne x3,x0,-8
    sw x5,12,x0
    ```
    
    **Prime :** 
    
    1. checks if the number stored in memory location 0x00000004 **{ mem[1] in my code }** is prime or not…
    2. if its prime then stores 1 in memory location 0x00000004 **{ mem[1] in my code }** else it stores zero in the memory location 0x00000004 **{ mem[1] in my code }** 
    
    ```matlab
    addi x4,x0,{number}
    sw x4,4,x0
    addi x3,x0,1
    lw x1,4,x0
    add x2,x0,x1 
    add x5,x0,x1  
    addi x2,x2,-1
    beq x2,x3,28
    sub x5,x5,x2
    blt x0,x5,-4
    beq x5,x0,8
    blt x5,x0,-24
    sw x0,4,x0
    jal x1,8
    sw x3,4,x0
    ```
    
    **Factorial :**
    
    1. calculates factorial of the number stored in memory location 0x00000004 **{ mem[1] in my code }** and stores the result in location 0x00000008 **{ mem[2] in my code }** 
    
    ```matlab
    addi x4,x0,{number}
    sw x4,4,x0
    lw x1,4,x0
    add x5,x0,x1
    addi x1,x1,1 
    add x3,x0,x0
    addi x1,x1,-1 
    add x2,x0,x1 
    addi x2,x2,-1   
    beq x2,x0,24
    add x3,x3,x5
    addi x2,x2,-1  
    bne x2,x0,-8
    add x5,x0,x3
    beq x2,x0,-36
    sw x5,8,x0
    ```
