FROM gcc:12

# Directorio de trabajo
WORKDIR /app

# Copiar el código fuente y compilar
COPY main.c .
RUN gcc main.c -o main

# Instalar ImageMagick para convertir PPM -> PNG/JPG
RUN apt-get update && apt-get install -y --no-install-recommends \
    imagemagick \
    && rm -rf /var/lib/apt/lists/*

# Crear carpeta de evidencia
RUN mkdir -p evidencia

# Ejecutar: genera captura.ppm y la convierte a PNG dentro de /app/evidencia
CMD ["/bin/sh", "-lc", "./main && convert captura.ppm evidencia/captura.png && echo 'captura.png generada en /app/evidencia/'"]
