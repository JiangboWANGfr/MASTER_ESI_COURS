//#include<opencv2/imgproc/imgproc.hpp>
//#include<opencv2/highgui/highgui.hpp>
#include "img_mgmt.h"
#include "img_convolution.h"

char *g_progname;
static inline void usage(void) {
    fprintf(stderr, "USAGE: %s [filename [x y]]\n", g_progname);
    exit(0);
}


int 
main(int argc, char* argv[])
{
    uint32_t X        = 512, 
             Y        = 512;
    char*    filename = NULL;

    g_progname        = argv[0]; 
    printf("argc = %d\n", argc);
    printf("argv[0] = %s\n", argv[0]);
    switch(argc) {
    case 4:
            X        = strtoul(argv[3], NULL, 0);
            Y        = strtoul(argv[2], NULL, 0);
    case 2:
            filename = argv[1];
            break;
    case 1:
            filename = "/home/jetson/Desktop/TP_OpenMP/TP_ParallelProg/raw_images/lena_color.data";
            X        = 512;
            Y        = 512;
            break;
    default:
            usage();
    }

    printf("Processing %s...\n", filename);

    image_t img;
    img_load(filename, X, Y, &img);
    convolution(&img, 1/4.0);
    //convolution_seq(&img, 1/4.0);

    printf("done\n");


    img_write(filename, &img);

    free(img.data_in);
    free(img.data_out);

    return 0;
}
