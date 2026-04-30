#include <stdio.h>

int main(void) {
    const int width = 200;
    const int height = 100;
    FILE *f = fopen("captura.ppm", "w");
    if (!f) {
        perror("fopen");
        return 1;
    }

    fprintf(f, "P3\n%d %d\n255\n", width, height);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int r = (x * 255) / (width - 1);
            int g = (y * 255) / (height - 1);
            int b = 128;
            fprintf(f, "%d %d %d ", r, g, b);
        }
        fprintf(f, "\n");
    }

    fclose(f);
    printf("Archivo 'captura.ppm' generado correctamente.\n");
    return 0;
}
