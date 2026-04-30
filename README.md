# Proyecto Docker — Contenedores con C y Python

Incluye dos contenedores funcionales: uno en **C** que genera imágenes JPG y uno en **Python** que genera gráficas matemáticas como PNG.

---

## Objetivos de aprendizaje

| # | Objetivo |
|---|----------|
| 1 | Explicar cómo funciona Docker y cómo se utiliza para crear y gestionar contenedores |
| 2 | Seguir instrucciones detalladas para construir un contenedor Docker con un programa en C |
| 3 | Documentar el proceso de creación y uso de contenedores en un `README.md` claro |
| 4 | Organizar y estructurar un repositorio Git de manera limpia y eficaz |
| 5 | Proponer, desarrollar y reportar un contenedor Docker adicional usando Git |


---

## Estructura del repositorio

```text
.
├── contenedor-c-jpg/
│   ├── main.c
│   └── Dockerfile
├── contenedor-python-plot/
│   ├── app.py
│   ├── requirements.txt
│   └── Dockerfile
├── .gitignore
└── README.md
```

---

## Contenedor 1 — Programa en C que genera JPG

> Contenedor principal requerido por la práctica.

### Descripción

Un programa escrito en C que utiliza la librería [libjpeg](http://libjpeg.sourceforge.net/) para generar una imagen JPG desde cero dentro de un contenedor Docker aislado.

### Estructura

```text
contenedor-c-jpg/
├── main.c          # Programa en C que genera la imagen
└── Dockerfile      # Imagen basada en gcc/debian con libjpeg
```

### Construcción de la imagen

```bash
cd contenedor-c-jpg
docker build -t generador-jpg-c .
```

### Ejecución del contenedor

```bash
# Linux / macOS
docker run --rm -v $(pwd):/output generador-jpg-c

# Windows PowerShell
docker run --rm -v ${PWD}:/output generador-jpg-c
```

La imagen `output.jpg` aparecerá en la carpeta actual al terminar.

### Justificación técnica

- **Compilación dentro del contenedor**: no se requiere instalar `gcc` ni `libjpeg` localmente.
- **Imagen efímera**: el flag `--rm` elimina el contenedor tras la ejecución, sin dejar residuos.
- **Volumen de salida**: el flag `-v` mapea la carpeta local para recuperar el archivo generado.

---

## Contenedor 2 — Generador de gráfica matemática en Python

> Contenedor adicional propuesto para la práctica.

### Descripción

Un script Python que utiliza `matplotlib` para generar una gráfica matemática (por ejemplo, una función seno) y la exporta como `grafica.png`.

### Estructura

```text
contenedor-python-plot/
├── app.py              # Script que genera la gráfica
├── requirements.txt    # Dependencias: matplotlib, numpy
└── Dockerfile          # Imagen python:3.9-slim
```

### Construcción de la imagen

```bash
cd contenedor-python-plot
docker build -t generador-grafica-python .
```

### Ejecución del contenedor

```bash
# Linux / macOS
docker run --rm -v $(pwd):/app generador-grafica-python

# Windows PowerShell
docker run --rm -v ${PWD}:/app generador-grafica-python
```

### Justificación técnica

| Decisión | Razón |
|----------|-------|
| `python:3.9-slim` | Reduce el tamaño de la imagen y la superficie de ataque versus la imagen completa |
| Flag `-v` (volumen) | Permite extraer `grafica.png` del contenedor efímero al sistema local |
| `requirements.txt` | Fija versiones de dependencias para reproducibilidad |
| Contenedor aislado | Evita conflictos con versiones locales de Python o matplotlib |

### Resultado esperado

Al ejecutar el contenedor se generará `grafica.png` en la carpeta desde la que se lanzó el comando.

---

## Conceptos clave de Docker

```
┌─────────────────────────────────────────────┐
│                  Dockerfile                 │
│   Define instrucciones para la imagen       │
└──────────────────┬──────────────────────────┘
                   │ docker build
                   ▼
┌─────────────────────────────────────────────┐
│                   Image                     │
│   Snapshot inmutable del entorno            │
└──────────────────┬──────────────────────────┘
                   │ docker run
                   ▼
┌─────────────────────────────────────────────┐
│                 Container                   │
│   Instancia en ejecución de la imagen       │
└─────────────────────────────────────────────┘
```

| Concepto | Descripción |
|----------|-------------|
| **Imagen** | Plantilla inmutable que define el entorno (SO, librerías, código) |
| **Contenedor** | Instancia en ejecución de una imagen; aislada del sistema host |
| **Dockerfile** | Archivo de texto con instrucciones para construir una imagen |
| **Volumen** | Mecanismo para persistir o compartir datos entre host y contenedor |
| **`docker build`** | Construye una imagen a partir de un Dockerfile |
| **`docker run`** | Crea y ejecuta un contenedor desde una imagen |

---

## Comandos de referencia rápida

```bash
# Ver imágenes locales
docker images

# Ver contenedores activos
docker ps

# Ver todos los contenedores (incluyendo detenidos)
docker ps -a

# Eliminar una imagen
docker rmi <nombre-imagen>

# Eliminar contenedores detenidos
docker container prune
```

---

## .gitignore recomendado

```gitignore
# Archivos generados por los contenedores
*.jpg
*.jpeg
*.png

# Archivos de sistema
.DS_Store
Thumbs.db

# Variables de entorno
.env
```

---
