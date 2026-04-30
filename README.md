# gr-fica

## Contenedor Python para generar una gráfica

Este proyecto incluye un contenedor Docker ubicado en [contenedor-python-plot](contenedor-python-plot) que genera una gráfica matemática sencilla y la guarda como `grafica.png`.

### Estructura

```text
/contenedor-python-plot
├── app.py
├── requirements.txt
└── Dockerfile
```

### Construcción de la imagen

Desde la raíz de la carpeta [contenedor-python-plot](contenedor-python-plot), ejecute:

```bash
docker build -t generador-grafica-python .
```

### Ejecución del contenedor

Para recuperar la imagen generada en su máquina host, utilice un volumen:

```bash
docker run --rm -v $(pwd):/app generador-grafica-python
```

En Windows PowerShell, puede usar:

```powershell
docker run --rm -v ${PWD}:/app generador-grafica-python
```

### Justificación técnica

- Aislamiento: el contenedor incluye todas las librerías necesarias para evitar conflictos con las versiones locales de Python.
- Persistencia de datos: el uso del flag `-v` permite extraer `grafica.png` desde el contenedor efímero hacia el sistema de archivos local.
- Optimización: se utiliza la imagen `python:3.9-slim` para reducir el tamaño de la imagen y la superficie de ataque.

### Resultado esperado

Al ejecutar el contenedor, se generará el archivo `grafica.png` en la carpeta desde la que se lanzó el comando.