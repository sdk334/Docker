# Utilizar una imagen oficial de Python ligera
FROM python:3.9-slim

# Establecer el directorio de trabajo dentro del contenedor
WORKDIR /app

# Instalar dependencias del sistema necesarias para Matplotlib
RUN apt-get update && apt-get install -y \
    libpng-dev \
    && rm -rf /var/lib/apt/lists/*

# Copiar el archivo de requerimientos e instalar dependencias de Python
COPY requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt

# Copiar el script al contenedor
COPY app.py .

# Comando por defecto para ejecutar la aplicación
CMD ["python", "app.py"]
