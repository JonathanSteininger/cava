#include <locale.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int print_raw_bar_out(int bars_count, int fd, int is_binary, int bit_format, int ascii_range,
                  char bar_delim, char frame_delim, int const f[]) {
    char *barCharecters[9] = {"▁","▂","▃","▄","▅","▆","▇","█"," "};
    int barCharByteSize = 4;
    int barCharByteOffset = 3;
    int barLayers = 8;
    ascii_range++;
    int height=(ascii_range % barLayers == 0 ? ascii_range/barLayers : ascii_range/barLayers + 1);
    ascii_range--;
    setlocale(LC_ALL, "en_US.UTF-8");
    char mainBuf[barCharByteSize * height * bars_count];
    int size_mainBuf = 0;
    for(int level = height -1; level >= 0; level--) {
        for (int i = 0; i < bars_count; i++) {
            int f_ranged = f[i];
            if (f_ranged > ascii_range)
                f_ranged = ascii_range;

            //gets the index of bar
            int index = f_ranged % barLayers;

            if ((level + 1) * barLayers <= f_ranged){
                index = barLayers - 1;
            }else if (level * barLayers >= f_ranged){
                index = barLayers;
            }

            snprintf(mainBuf + size_mainBuf, barCharByteSize, barCharecters[index]);
            size_mainBuf += barCharByteOffset;
        }
        if (level != 0){
            sprintf(mainBuf + size_mainBuf, ";");
            size_mainBuf += 1;
        }
    }
    sprintf(mainBuf + size_mainBuf, "\n");
    size_mainBuf += 1;
    write(fd, &mainBuf, size_mainBuf);
    
    return 0;
}
