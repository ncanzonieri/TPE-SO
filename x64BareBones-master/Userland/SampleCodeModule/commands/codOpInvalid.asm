section .text
GLOBAL invalidOperation

invalidOperation:
    ud2                 ; Instrucción reservada para generar una excepción de "código de operación inválido"
