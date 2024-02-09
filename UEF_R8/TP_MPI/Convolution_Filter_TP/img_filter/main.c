// #include<opencv2/imgproc/imgproc.hpp>
// #include<opencv2/highgui/highgui.hpp>
#include "img_mgmt.h"
#include "img_convolution.h"
#include <mpi.h>

char *g_progname;
static inline void usage(void)
{
    fprintf(stderr, "USAGE: %s [filename [x y]]\n", g_progname);
    exit(0);
}

int main(int argc, char *argv[])
{
    uint32_t X = 512,
             Y = 512;
    char *filename = NULL;

    g_progname = argv[0];
    switch (argc)
    {
    case 4:
        X = strtoul(argv[3], NULL, 0);
        Y = strtoul(argv[2], NULL, 0);
    case 2:
        filename = argv[1];
        break;
    case 1:
        filename = "/home/ensea/MPI_TP/Convolution_Filter_TP/raw_images/img02.data";
        X = 1800;
        Y = 1000;
        break;
    default:
        usage();
    }

    int rank, size;
    // must ALWAYS be called to run an MPI program
    MPI_Init(&argc, &argv);
    // get process rank id
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // get total number of processes in communicator
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (rank == 0)
    {
        printf("Processing %s on %d processors...\n", filename, size);
    }
    image_t img;
    uint32_t local_width = X / size;
    uint32_t start_row = rank * local_width;
    uint32_t end_row = start_row + local_width;
    int send_count = local_width * Y;
    printf("rank %d: start_row = %d, end_row = %d\n", rank, start_row, end_row);
    if (rank == 0)
    {
        // load image from file just in the master process
        img_load(filename, X, Y, &img);
        img.width = local_width;
        img.height = Y;
        for (int i = 1; i < size; i++)
        {
            pixel_t *send_data = img.data_in + i * send_count * sizeof(pixel_t);
            // send part of the image to each process
            MPI_Send(send_data, send_count * sizeof(pixel_t), MPI_CHAR, i, 0, MPI_COMM_WORLD);
        }
    }
    else
    {
        // allocate memory for the image in each process
        img.data_in = (pixel_t *)malloc(send_count * sizeof(pixel_t));
        img.data_out = (pixel_t *)malloc(send_count * sizeof(pixel_t));
        img.width = local_width;
        img.height = Y;
        memset(img.data_out, 0UL, send_count);
        // receive part of the image from the master process
        MPI_Recv(img.data_in, send_count * sizeof(pixel_t), MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    // perform the convolution in each process for part of the image loading from the master process
    convolution(&img, 1 / 4.0, start_row, end_row);
    if (rank == 0)
    {
        for (int i = 1; i < size; i++)
        {
            pixel_t *recv_data = img.data_out + i * send_count * sizeof(pixel_t);
            // receive the result from each process[img.dataout]
            MPI_Recv(recv_data, send_count, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }
    else
    {
        // send the result to the master process[img.dataout]
        MPI_Send(img.data_out, send_count, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }
    if (rank == 0)
    {
        printf("Writing %s\n", filename);
        // write the result to file just in the master process
        img_write(filename, &img);
        printf("Convolution with MPI done\n");
    }
    free(img.data_in);
    free(img.data_out);

    MPI_Finalize();
    return 0;
}
