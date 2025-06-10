# Trabajo Práctico - Sistemas Operativos

### Integrantes:
- Revale Nicolás Ezequiel - 64227  
- Canzonieri Nicolas - 63501  
- Mangiaterra Francisco Tomas - 64544  

---

## Manual de Usuario

### Requerimientos

Tener instalados los siguientes programas:
- `make`
- `nasm`
- `docker`
- `qemu`

---

## Shell

### Comandos

- `help:` Imprime la lista de los comandos disponibles y su descripción.
- `clear:` Limpia la pantalla al ejecutarse, eliminando cualquier salida anterior.
- `registers:` Imprime los últimos registros cargados (se cargan con Ctrl+R).
- `divx0:` Simula la excepción de dividir por 0.
- `invalid:` Simula la excepción de código de operación inválida.
- `time:` Imprime la hora actual en Buenos Aires.
- `date:` Imprime la fecha actual en Buenos Aires.
- `zoom:` Varía la escala del texto: `<in>` la aumenta, `<out>` la disminuye.
- `agro:` Imprime el escudo de Club Atlético Agropecuario.
- `snake:` Comienza el juego de Snake.
- `testMem:` Test del memory manager `<tamano de memoria maxima>`.
- `mem:` Imprime el uso de memoria actual.
- `ps:` Imprime información sobre los procesos vivos al momento.
- `testProc:` Test del scheduler `<cantidad maxima de procesos>`.
- `testPrio:` Test de prioridades.
- `testSync:` Test de sincronización `<maxima cantidad de separaciones>` y `<uso de semaforos (0 o 1)>`.
- `loop:` Imprime su ID con un saludo cada cierta cantidad de segundos. `<PID>`
- `kill:` Mata un proceso por su PID. `<PID>`
- `nice:` Cambia la prioridad de un proceso dado su PID y nueva prioridad. `<PID> <prioridad>`
- `block:` Bloquea un proceso por su PID. `<PID>`
- `unblock:` Desbloquea un proceso por su PID. `<PID>`
- `cat:` Imprime el contenido de la entrada estándar.
- `wc:` Cuenta las líneas de la entrada estándar.
- `filter:` Filtra las vocales de la entrada estándar.
- `phylo:` Implementa el problema de los filósofos comensales.

---

### Características Adicionales

- **Foreground/Background Execution:** Agregando `&` al final de un comando se ejecuta en segundo plano.
- **Pipe:** Se pueden conectar dos aplicaciones mediante un pipe usando `|`. Por ejemplo: `ps | wc`.
- **Soporte para Teclas Especiales:**  
  - `Ctrl+D`: Señala fin de archivo en la entrada estándar.  
  - `Ctrl+C`: Termina el proceso en foreground.  
  - `Ctrl+R`: Guarda el estado de los registros para luego mostrarlos con `registers`.

---

### Comentarios Adicionales

- El script `compilador.sh` requiere como primer argumento el nombre de un contenedor existente. Si se desea usar el sistema "Buddy" como manejo de memoria, se debe agregar como segundo argumento `MM=BUDDY`.
- Se utilizó TGDB para el seguimiento de registros, stack y uso de memoria. Con run.sh -d se puede correr con tgdb.
- Se utilizó PVS-studio para detectar errores, vulnerabilidades y posibles problemas de calidad en codigo fuente.