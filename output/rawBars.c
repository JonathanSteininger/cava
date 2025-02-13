#include <stdio.h>
#include <unistd.h>

int print_raw_bar_out(int bars_count, int fd, int ascii_range,
                  char layer_delim, char frame_delim, int const f[]) {
    //charecters used to draw
    char *barCharecters[9] = {" ","▁","▂","▃","▄","▅","▆","▇","█"};
    //byte data to draw no hidden charecters
    int barCharByteSize = 4;
    int barCharByteOffset = 3;
    //amount of visible bars
    int barLayers = 8;

    //gets amoun of lines needed depending on ascii_range value
    int height = ascii_range % barLayers == 0 ? ascii_range/barLayers : ascii_range/barLayers + 1;

    //buffer to write frame into
    char mainBuf[barCharByteSize * height * bars_count + height + 1];
    int size_mainBuf = 0;
    
    //each line of bars
    for(int level = height -1; level >= 0; level--) {
        //each bar
        for (int i = 0; i < bars_count; i++) {
            int f_ranged = f[i];

            //gets the index for what bar to draw
            int index = f_ranged % barLayers;
            if ((level + 1) * barLayers <= f_ranged){
                index = barLayers;
            }else if (level * barLayers >= f_ranged){
                index = 0;
            }
            //appends char to buffer
            snprintf(mainBuf + size_mainBuf, barCharByteSize, "%s", barCharecters[index]);
            size_mainBuf += barCharByteOffset;
        }
        //appends bar line delimiter
        if (level != 0){
            sprintf(mainBuf + size_mainBuf, "%c", layer_delim);
            size_mainBuf += 1;
        }
    }
    //append end of framw delimiter
    sprintf(mainBuf + size_mainBuf, "%c", frame_delim);
    size_mainBuf += 1;
    //writes to rawOutput path
    write(fd, &mainBuf, size_mainBuf);
    return 0;
}
