GLOBAL sys_write,
GLOBAL sys_read
GLOBAL sys_drawRectangle
GLOBAL sys_clearScreen
GLOBAL sys_getCoords
GLOBAL sys_undrawChar
GLOBAL sys_getScreenInfo
GLOBAL sys_getFontInfo
GLOBAL sys_getTime
GLOBAL sys_setFontScale
GLOBAL sys_getRegisters
GLOBAL sys_sleep
GLOBAL sys_playSound
GLOBAL sys_ticks
GLOBAL sys_malloc
GLOBAL sys_free
GLOBAL sys_memoryDump
GLOBAL sys_getPid
GLOBAL sys_killProcess
GLOBAL sys_showProcesses
GLOBAL sys_createProcess
GLOBAL sys_changePriority
GLOBAL sys_blockProcess
GLOBAL sys_unblockProcess
GLOBAL sys_yield
global sys_updatePriority

section .text

%macro syscall 1
    mov rax, %1
    int 80h
    ret
%endmacro

;sys_write(STDOUT_FD, "la-maquina $>",14,GREEN);

sys_read: syscall 0x00
sys_write: syscall 0x01
sys_drawRectangle: syscall 0x02
sys_clearScreen: syscall 0x03
sys_getCoords: syscall 0x04
sys_getScreenInfo: syscall 0x05
sys_getFontInfo: syscall 0x06
sys_getTime: syscall 0x07
sys_setFontScale: syscall 0x08
sys_getRegisters: syscall 0x09
sys_sleep: syscall 0x0A
sys_playSound: syscall 0x0B
sys_ticks: syscall 0x0E
sys_malloc: syscall 0x0F
sys_free: syscall 0x10
sys_memoryDump: syscall 0x11
sys_getPid: syscall 0x12
sys_killProcess: syscall 0x13
sys_showProcesses: syscall 0x14
sys_createProcess: syscall 0x15
sys_changePriority: syscall 0x16
sys_blockProcess: syscall 0x17
sys_unblockProcess: syscall 0x18
sys_yield: syscall 0x19
sys_updatePriority: syscall 0x1A