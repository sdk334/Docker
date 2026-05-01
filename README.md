# Documentación de Docker

**Alumno:** Jonathan Uriel Paredes Martinez
**Carrera:** Ingeniería en Sistemas Computacionales — ESCOM IPN

---

## Tabla de Contenidos

1. [¿Qué es Docker?](#1-qué-es-docker)
2. [Tabla de análisis por componente](#2-tabla-de-análisis-por-componente)
3. [Reporte de Resultados: Contenedor en C que genera PPM](#3-reporte-de-resultados-contenedor-en-c-que-genera-ppm)
4. [Contenedor Python — Generador de Gráficas Matemáticas](#4-contenedor-python--generador-de-gráficas-matemáticas)
5. [Instrucciones de Construcción y Ejecución](#5-instrucciones-de-construcción-y-ejecución)
6. [Explicación del Aislamiento y la Automatización](#6-explicación-del-aislamiento-y-la-automatización)
7. [Requisitos del Sistema](#7-requisitos-del-sistema)

---

## 1. ¿Qué es Docker?

**Docker** es una plataforma de virtualización a nivel de sistema operativo que permite empaquetar una aplicación junto con todas sus dependencias (librerías, compiladores, variables de entorno) en una unidad estandarizada llamada **contenedor**. A diferencia de una máquina virtual, un contenedor no incluye un sistema operativo completo; comparte el kernel del host y solo encapsula lo estrictamente necesario para ejecutar la aplicación.

El flujo básico de Docker es el siguiente: el desarrollador escribe un `Dockerfile` que describe el entorno deseado; a partir de ese archivo, `docker build` construye una **imagen** inmutable; y `docker run` instancia esa imagen en un **contenedor** en ejecución. Esto garantiza que el programa se comporte de forma idéntica en cualquier máquina que tenga Docker instalado, eliminando el problema clásico de "en mi máquina sí funciona".

---

## 2. Tabla de análisis por componente

| Elemento | Código / Instrucción | Explicación técnica |
| :--- | :--- | :--- |
| **`FROM`** | `FROM gcc:latest` | Define la imagen base sobre la que se construye la nueva imagen. Toda imagen hereda el sistema de archivos y las herramientas de su base. Elegir la imagen correcta es crucial para minimizar el tamaño final. |
| **`RUN`** | `RUN apt-get install -y libpng-dev` | Ejecuta un comando de shell durante la fase de construcción de la imagen. Cada instrucción `RUN` genera una nueva capa en la imagen; por eso es buena práctica encadenar comandos con `&&` para reducir el número de capas. |
| **`COPY`** | `COPY main.c /app/main.c` | Copia archivos del sistema de archivos del host al sistema de archivos de la imagen. Se ejecuta en tiempo de build, no en tiempo de ejecución. |
| **`WORKDIR`** | `WORKDIR /app` | Establece el directorio de trabajo para las instrucciones `RUN`, `CMD`, `COPY` y `ADD` que le sigan. Equivale a ejecutar `cd /app` antes de cada comando. |
| **`CMD`** | `CMD ["./generador"]` | Define el comando por defecto que se ejecutará cuando se inicie el contenedor. Solo puede haber un `CMD` por Dockerfile; si se especifica, puede ser sobreescrito al llamar `docker run`. |
| **`docker build`** | `docker build -t nombre .` | Lee el `Dockerfile` del directorio indicado (`.`) y construye la imagen, asignándole el nombre y etiqueta especificados con `-t`. Las capas intermedias se cachean para acelerar builds posteriores. |
| **`docker run`** | `docker run --rm -v $(pwd):/output nombre` | Crea e inicia un contenedor a partir de la imagen. El flag `--rm` elimina el contenedor al terminar. El flag `-v` monta un volumen para compartir datos entre host y contenedor. |
| **`--rm`** | Flag de `docker run` | Instruye a Docker para que elimine automáticamente el contenedor y su sistema de archivos al detenerse. Evita la acumulación de contenedores detenidos que ocupan espacio en disco. |
| **`-v` (volumen)** | `-v $(pwd):/output` | Monta un directorio del host (`$(pwd)`) dentro del contenedor en la ruta `/output`. Los archivos escritos en esa ruta dentro del contenedor aparecen directamente en el host. |
| **`.dockerignore`** | `.dockerignore` | Equivalente al `.gitignore` para Docker. Lista los archivos y directorios que `docker build` debe ignorar al copiar el contexto de construcción, reduciendo el tiempo de build y el tamaño de la imagen. |

---

## 3. Reporte de Resultados: Contenedor en C que genera PPM

Durante la reproducción del ejercicio, se construyó y ejecutó el contenedor en C siguiendo las instrucciones de la práctica. A continuación se describe la secuencia completa de operaciones.

### Paso 1: Construcción de la imagen

```bash
docker build -t generador-ppm-c .
```

- **Explicación:** Docker lee el `Dockerfile` del directorio actual y ejecuta cada instrucción en orden, generando capas intermedias cacheadas. La imagen resultante queda registrada localmente bajo el nombre `generador-ppm-c`.
- **Capa base (`FROM gcc:latest`):** Provee el compilador `gcc` y el entorno Debian necesario para compilar el programa. El formato PPM (Portable Pixmap) es texto plano y no requiere librerías externas; `main.c` solo depende de la biblioteca estándar de C (`stdio.h`).
- **Compilación dentro del build (`RUN gcc`):** El código fuente `main.c` se compila durante la construcción de la imagen, de modo que el contenedor final ya contiene el binario listo para ejecutarse.

### Paso 2: Ejecución del contenedor

```bash
docker run --rm -v $(pwd):/output generador-ppm-c
```

- **Explicación:** Docker instancia la imagen `generador-ppm-c` como un contenedor efímero. El programa en C se ejecuta dentro del entorno aislado del contenedor y escribe el archivo `captura.ppm` en el directorio de trabajo, que está montado sobre el directorio actual del host mediante el volumen `-v`.

### Acerca del formato PPM

El programa genera una imagen en formato **PPM (Portable Pixmap)**, que es uno de los formatos gráficos más simples que existen. Un archivo PPM es texto plano estructurado así:

```
P3
200 100
255
R G B  R G B  R G B ...
```

- `P3` indica que los valores de color son texto ASCII (en contraposición a `P6`, que usa binario).
- Cada pixel se describe con tres enteros entre 0 y 255 (rojo, verde, azul).
- `main.c` genera un gradiente: el canal rojo aumenta de izquierda a derecha, el verde de arriba hacia abajo, y el azul se fija en 128.

La ventaja del PPM es que no requiere ninguna librería de compresión ni codificación: el programa lo produce con llamadas directas a `fprintf`, lo que hace que el contenedor no necesite instalar ninguna dependencia más allá de `gcc`.

### Verificación del aislamiento

Para confirmar que el programa no requiere ninguna dependencia local, se verificó que la compilación y ejecución funcionan correctamente dentro del contenedor sin instalar `gcc` en el sistema host. El archivo `captura.ppm` fue generado correctamente, demostrando que el entorno de compilación está completamente encapsulado dentro de la imagen.

### Instrucciones de ejecución del ejemplo

Para validar el funcionamiento del contenedor en C:

1. Construir la imagen:
```bash
docker build -t generador-ppm-c .
```
2. Ejecutar el contenedor y recuperar el archivo:
```bash
docker run --rm -v $(pwd):/output generador-ppm-c
```
3. Eliminar la imagen local al terminar:
```bash
docker rmi generador-ppm-c
```

### Flujo de ejecución del contenedor

```
docker build
  │
  ├─► Leer Dockerfile instrucción por instrucción
  │     ├─ FROM  → descargar imagen base gcc:latest (si no está en caché)
  │     ├─ COPY  → copiar main.c al sistema de archivos de la imagen
  │     └─ RUN   → compilar main.c → produce el binario 'generador'
  │
  └─► Imagen 'generador-ppm-c' lista en el registro local

docker run --rm -v $(pwd):/output generador-ppm-c
  │
  ├─► Docker instancia la imagen como contenedor aislado
  ├─► Monta $(pwd) del host en /output dentro del contenedor
  ├─► Ejecuta CMD → ./generador
  │     └─ El programa escribe captura.ppm en el directorio de trabajo
  └─► Contenedor se detiene → --rm lo elimina automáticamente
        └─ captura.ppm queda disponible en $(pwd) del host
```

**Resultado del análisis:** El contenedor compila y ejecuta el programa en C de forma completamente aislada, escribe `captura.ppm` a través del volumen montado y desaparece sin dejar rastros en el sistema. El único artefacto persistente es `captura.ppm` en el directorio del host.

---

## 4 Contenedor Python — Generador de Gráficas Matemáticas

### Descripción de Archivos

```
contenedor-python-plot/
├── Dockerfile          ← Define el entorno Python y la ejecución del script
├── requirements.txt    ← Fija las versiones de las dependencias Python
└── app.py              ← Script que genera la gráfica matemática
```

---

### Explicación del Código Fuente

#### `requirements.txt` — Fijación de dependencias

```text
matplotlib==3.7.1
numpy==1.24.3
```

**¿Por qué se usa un `requirements.txt`?**

Fijar versiones exactas de las dependencias garantiza **reproducibilidad**: cualquier persona que construya la imagen en cualquier momento obtendrá exactamente las mismas versiones de `matplotlib` y `numpy`. Sin este archivo, `pip install matplotlib` podría instalar versiones distintas en diferentes momentos, introduciendo comportamientos inesperados o incompatibilidades. En el `Dockerfile` se instala con `pip install -r requirements.txt`, lo que convierte este archivo en la fuente de verdad sobre las dependencias del proyecto.

---

#### `app.py` — El generador de gráficas

```python
import matplotlib.pyplot as plt
import numpy as np

# Generar datos para la gráfica
x = np.linspace(0, 10, 100)
y = np.sin(x)

plt.figure(figsize=(10, 6))
plt.plot(x, y, label='Seno(x)', color='blue')
plt.title('Gráfica generada en un contenedor Docker')
plt.xlabel('Eje X')
plt.ylabel('Eje Y')
plt.legend()
plt.grid(True)

# Guardar la imagen en el directorio de trabajo
plt.savefig('grafica.png')
print("Imagen 'grafica.png' generada exitosamente.")

```

---

### El Dockerfile del Proyecto

El Dockerfile es el corazón del contenedor. Define el entorno de ejecución de forma precisa y reproducible.

```dockerfile
# ═══════════════════════════════════════════════════════════════
# Dockerfile — Contenedor Python: Generador de Gráficas
# Ingeniería en Sistemas Computacionales, ESCOM IPN
# ═══════════════════════════════════════════════════════════════

# ──────────────────────────────────────────────
# SECCIÓN 1: Imagen base
# ──────────────────────────────────────────────

# python:3.9-slim es una variante reducida de la imagen oficial de Python.
# Contiene solo lo mínimo para ejecutar Python, sin herramientas de desarrollo
# ni documentación, lo que reduce el tamaño final de ~900 MB a ~125 MB.
FROM python:3.9-slim


# ──────────────────────────────────────────────
# SECCIÓN 2: Directorio de trabajo
# ──────────────────────────────────────────────

# Todos los comandos RUN, COPY y CMD siguientes se ejecutan
# relativos a este directorio dentro del contenedor.
# Si el directorio no existe, Docker lo crea automáticamente.
WORKDIR /app


# ──────────────────────────────────────────────
# SECCIÓN 3: Instalación de dependencias
# ──────────────────────────────────────────────

# Copiar SOLO el requirements.txt primero (antes que el resto del código).
# Esto aprovecha el sistema de caché de capas de Docker:
# si requirements.txt no cambia entre builds, Docker reutiliza
# la capa de pip install en lugar de reinstalar todo.
COPY requirements.txt .

RUN pip install --no-cache-dir -r requirements.txt


# ──────────────────────────────────────────────
# SECCIÓN 4: Copiar el código fuente
# ──────────────────────────────────────────────

# Se copia después de instalar dependencias para no invalidar
# la caché de pip install cada vez que el código cambia.
COPY app.py .


# ──────────────────────────────────────────────
# SECCIÓN 5: Comando de ejecución
# ──────────────────────────────────────────────

# Define el proceso principal del contenedor.
# Se usa la forma de lista JSON (exec form) en lugar de la forma de string
# (shell form) para que Python reciba las señales del sistema operativo
# directamente (como SIGTERM al detener el contenedor con docker stop).
CMD ["python", "app.py"]
```

#### Desglose de las decisiones de diseño del Dockerfile

**Orden de instrucciones y caché de capas**

Docker construye imágenes en capas; cada instrucción (`FROM`, `RUN`, `COPY`) genera una capa nueva. Una vez que una capa es construida, Docker la cachea. En la siguiente ejecución de `docker build`, si los archivos de entrada de una instrucción no han cambiado, Docker reutiliza la capa cacheada en lugar de reconstruirla.

Por eso el Dockerfile copia `requirements.txt` e instala las dependencias **antes** de copiar `app.py`. El código cambia con frecuencia durante el desarrollo, pero las dependencias no. Si se copiara todo el código primero, cualquier modificación a `app.py` invalidaría la caché de pip install, forzando una reinstalación completa de `matplotlib` y `numpy` en cada build.

**`--no-cache-dir` en pip**

La flag `--no-cache-dir` evita que `pip` almacene los archivos de instalación descargados dentro de la imagen. En tiempo de ejecución ese caché no sirve de nada, y eliminarlo reduce el tamaño de la imagen final.

**Exec form vs Shell form en `CMD`**

| Forma | Sintaxis | Proceso principal |
| :--- | :--- | :--- |
| Shell form | `CMD python app.py` | `/bin/sh -c "python app.py"` (PID 1 es el shell) |
| Exec form | `CMD ["python", "app.py"]` | `python app.py` directamente (PID 1 es Python) |

En la exec form, Python recibe directamente las señales del sistema operativo (como `SIGTERM`), lo que permite un apagado limpio del contenedor con `docker stop`.

---

## 5. Instrucciones de Construcción y Ejecución

### Requisitos previos

En sistemas Debian/Ubuntu:

```bash
sudo apt update && sudo apt install docker.io
sudo systemctl start docker
```

### Clonar el repositorio

```bash
git clone https://github.com/sdk334/Docker.git
cd Docker/contenedor-python-plot
```

### Construir la imagen

```bash
docker build -t generador-grafica-python .
```

Docker ejecutará las siguientes capas en orden:

1. Descarga la imagen base `python:3.9-slim`
2. Establece `/app` como directorio de trabajo
3. Copia `requirements.txt` e instala `matplotlib` y `numpy`
4. Copia `app.py` al sistema de archivos de la imagen

La salida en la terminal es similar a:

```
Step 1/5 : FROM python:3.9-slim
 ---> a1b2c3d4e5f6
Step 2/5 : WORKDIR /app
 ---> Running in 7f8e9a0b1c2d
Step 3/5 : COPY requirements.txt .
 ---> 3c4d5e6f7a8b
Step 4/5 : RUN pip install --no-cache-dir -r requirements.txt
 ---> Running in 9b0c1d2e3f4a
Successfully installed matplotlib-3.7.1 numpy-1.24.3
Step 5/5 : CMD ["python", "app.py"]
 ---> 5e6f7a8b9c0d
Successfully built 5e6f7a8b9c0d
Successfully tagged generador-grafica-python:latest
```

### Ejecutar el contenedor

```bash
# Linux / macOS
docker run --rm -v $(pwd):/app generador-grafica-python

# Windows PowerShell
docker run --rm -v ${PWD}:/app generador-grafica-python
```

Salida esperada:

```
Gráfica generada: /app/grafica.png
```

Al terminar, `grafica.png` aparecerá en el directorio desde el que se ejecutó el comando.

### Limpiar los recursos generados

```bash
# Eliminar la imagen local
docker rmi generador-grafica-python

# Eliminar contenedores detenidos (si no se usó --rm)
docker container prune
```

---

## 6. Explicación del Aislamiento y la Automatización

El comportamiento del contenedor está determinado por tres mecanismos que trabajan en conjunto.

### 6.1 Aislamiento de dependencias por capas de imagen

Cada instrucción `RUN` en el Dockerfile crea una capa independiente. Las capas son de solo lectura y se apilan para formar el sistema de archivos final de la imagen. Cuando se instala `matplotlib` con `RUN pip install`, esa instalación queda congelada en su propia capa y no puede interactuar con ni verse afectada por el entorno Python del host.

```
Capa 5 (CMD)          → define el proceso de inicio
Capa 4 (COPY app.py)  → agrega el script al sistema de archivos
Capa 3 (RUN pip)      → instala matplotlib y numpy
Capa 2 (COPY req.txt) → agrega el archivo de dependencias
Capa 1 (FROM slim)    → sistema de archivos base de Python 3.9
```

**Ejemplo práctico:** Si el host tiene Python 3.11 con una versión incompatible de `matplotlib`, el contenedor sigue ejecutándose con Python 3.9 y `matplotlib==3.7.1` sin ningún conflicto, porque opera sobre su propio sistema de archivos aislado.

### 6.2 Caché de capas para builds incrementales

El sistema de caché de Docker funciona de forma análoga a la compilación incremental de `make`. Compara el estado de los archivos de entrada de cada instrucción con la capa cacheada correspondiente:

```
¿requirements.txt cambió desde el último build?
    └─ No → Reutilizar capa de pip install (ahorra varios minutos)
    └─ Sí → Reinstalar dependencias y reconstruir capas siguientes

¿app.py cambió desde el último build?
    └─ No → Reutilizar capa de COPY app.py
    └─ Sí → Copiar nueva versión (operación instantánea)
```

**Resultado:** Durante el desarrollo activo, donde `app.py` cambia con frecuencia pero `requirements.txt` no, los builds posteriores al primero son casi instantáneos porque todas las capas costosas (descarga de imagen base, instalación de pip) se sirven desde la caché.

### 6.3 Volúmenes para extracción de artefactos

El contenedor es efímero: al detenerse, su sistema de archivos desaparece. El flag `-v $(pwd):/app` crea un puente entre el sistema de archivos del host y el del contenedor, permitiendo que el archivo `grafica.png` escrito por `app.py` persista en el host después de que el contenedor se elimine.


Si el nuevo proyecto requiriera exportar múltiples archivos (por ejemplo, gráficas para distintos rangos de frecuencia), bastaría con modificar `app.py` para escribir todos los archivos en `/app`; el volumen los haría disponibles en el host automáticamente, sin cambiar el Dockerfile ni el comando `docker run`.

---

## 7. Requisitos del Sistema

| Requisito | Versión mínima recomendada |
| :--- | :--- |
| Sistema Operativo | Linux, macOS o Windows con WSL2 |
| Docker Engine | 20.10 o superior |
| Conexión a internet | Requerida solo durante `docker build` (descarga de imágenes base y paquetes) |

Para verificar la instalación de docker:

```bash
docker --version
docker info
```

---
