-------- PROJECT GENERATOR --------
PROJECT NAME :	nixeTubeIR
PROJECT DIRECTORY :	C:\WorkSpace\nixeTubeIR\nixeTubeIR
CPU SERIES :	RX600
CPU TYPE :	RX62T
TOOLCHAIN NAME :	Renesas RX Standard Toolchain
TOOLCHAIN VERSION :	1.0.1.0
GENERATION FILES :
    C:\WorkSpace\nixeTubeIR\nixeTubeIR\dbsct.c
        Setting of B,R Section
    C:\WorkSpace\nixeTubeIR\nixeTubeIR\typedefine.h
        Aliases of Integer Type
    C:\WorkSpace\nixeTubeIR\nixeTubeIR\sbrk.c
        Program of sbrk
    C:\WorkSpace\nixeTubeIR\nixeTubeIR\iodefine.h
        Definition of I/O Register
    C:\WorkSpace\nixeTubeIR\nixeTubeIR\intprg.c
        Interrupt Program
    C:\WorkSpace\nixeTubeIR\nixeTubeIR\vecttbl.c
        Initialize of Vector Table
    C:\WorkSpace\nixeTubeIR\nixeTubeIR\vect.h
        Definition of Vector
    C:\WorkSpace\nixeTubeIR\nixeTubeIR\resetprg.c
        Reset Program
    C:\WorkSpace\nixeTubeIR\nixeTubeIR\nixeTubeIR.c
        Main Program
    C:\WorkSpace\nixeTubeIR\nixeTubeIR\sbrk.h
        Header file of sbrk file
    C:\WorkSpace\nixeTubeIR\nixeTubeIR\stacksct.h
        Setting of Stack area
START ADDRESS OF SECTION :
 H'1000	B_1,R_1,B_2,R_2,B,R,SU,SI
 H'FFFF8000	PResetPRG
 H'FFFF8100	C_1,C_2,C,C$*,D*,P,PIntPRG,W*
 H'FFFFFFD0	FIXEDVECT

* When the user program is executed,
* the interrupt mask has been masked.
* 
* Program start 0xFFFF8000.
* RAM start 0x1000.

SELECT TARGET :
    RX600 Segger J-Link
DATE & TIME : 2019/6/12 ¤W¤È 02:00:08
