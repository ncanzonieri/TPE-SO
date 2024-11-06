# Trabajo Practico - Arquitectura de Computadoras

### Integrantes:
- Juan Francisco Palermo - 63014
- Canzonieri Nicolas - 63501
- Mangiaterra Francisco Tomas - 64544 

--
## Manual de Usuario
### Requerimientos
Tener instalado los siguientes programas:
- ```make```
- ```make```
- ```nasm```
- ```docker```
- ```qemu```

---
### Shell
### Comandos 

- ```Registers: ```Imprime en la pantalla todos los registros con sus respectivos valores al momento de ejecutar el comando. 
- ```help: ```Al ejecutar este comando, se despliega una lista con todos los comandos de la shell y una breve descripción de cada uno.
- ```snake:```Inicia una versión implementada del clásico juego Snake en nuestro sistema operativo.
- ```ZoomIn:```: Aumenta la escala de las palabras en la pantalla.  
- ```ZoomOut:```Disminuye la escala de las palabras en la pantalla. 
- ```time```Imprime en pantalla el horario actual de Buenos Aires, Argentina, con su respectiva hora, minutos y segundos al momento de ejecutar el comando.
- ```date:```Imprime en pantalla el día actual de Buenos Aires, Argentina, con su respectivo día, mes y año.
- ```Agro:```Muestra el escudo de Agropecuario Carlos Casares, equipo de fútbol de la Primera Nacional
- ```clear:``` Limpia la pantalla al ejecutarse, eliminando cualquier salida anterior.
- ```divx0:```Lanza una excepción cuando un programa intenta dividir un número entre cero.
- ```invalid:``` Genera intencionalmente una excepción de "código de operación inválido".

--

### Comentarios adicionales 

- Recordatorio: en ```Registers``` se deben guardar los valores de los registros con ```Crtl + r``` antes de ejectuarla.

- Existen tres niveles de escala: la mínima es 1, la intermedia es 2, y la máxima es 3. Se aumenta o se disminuye a de a uno.


- Usamos el TGDB para hacer seguimiento de registros, stack y memoria.