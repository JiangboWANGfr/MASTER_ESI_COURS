#include "img_mgmt.h"

void 
img_load(const char* filename, const size_t width, const size_t height, image_t* img)
{
    FILE* fsrc = fopen(filename, "r");
    if (!fsrc) 
        FATAL("fopen");

    fseek(fsrc, 0, SEEK_END);
    size_t sz = ftell(fsrc);
    fseek(fsrc, 0, SEEK_SET);

    img->data_in  = XMALLOC(sz);
    img->data_out = XMALLOC(sz);
    img->width    = width;
    img->height   = height;

    memset(img->data_in,  0UL, sz);
    memset(img->data_out, 0UL, sz);

    size_t n_bytes = fread(img->data_in, 1, sz, fsrc);
    printf("Read %lu. Expected %lu bytes\n", n_bytes, sz);
    if (n_bytes < sz) {
        fprintf(stderr, "Read fewer bytes than expected: %lu/%lu\n", n_bytes, sz);
    }

    if (feof(fsrc)) {
        printf("Finished reading %s\n", filename);
        clearerr(fsrc);
    }

    fclose(fsrc);
}

void 
img_write(char* filein, image_t* img)
{
    char fileout[101] = {0};
    strncpy(fileout,filein,sizeof(fileout)-1);
    size_t next_dot = 0;
    for (next_dot = 0; fileout[next_dot] != '.' && fileout[next_dot] != '\0'; ++next_dot) 
        ; // nothing

    snprintf(fileout + next_dot, sizeof(fileout) - next_dot, "%s", "_output.data");

    FILE* fdst = fopen(fileout, "w");
    if (!fdst) FATAL("fopen");


    size_t sz = img->width * img->height * sizeof(pixel_t);

    size_t n_bytes = fwrite(img->data_out, 1, sz, fdst);
    if (n_bytes < sz) {
        fprintf(stderr, "Wrote fewer bytes than expected: %lu/%lu\n", n_bytes, sz);
    }

    fclose(fdst);
}
