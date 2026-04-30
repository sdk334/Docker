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
