#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types

/* 
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1024
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 10

typedef struct _DecodeInfo
{
    /* Source Image info */
    char *src_image_fname;
    int extension_size;
    char decoded_data;
    FILE *fptr_src_image;
    uint image_capacity;
    uint bits_per_pixel;
    char image_data[MAX_IMAGE_BUF_SIZE];

    /* Secret File Info */
    char *secret_fname;
    FILE *fptr_secret;
    char extn_secret_file[MAX_FILE_SUFFIX];
    char secret_data[MAX_SECRET_BUF_SIZE];
    long size_secret_file;
    /* Stego Image Info */
    char *stego_image_fname;
    FILE *fptr_stego_image;


} DecodeInfo;


/* Decoding function prototype */


/* Read and validate Encode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* get file pointers for i/p and o/p fils */
Status open_files1(DecodeInfo *decInfo);
/* perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

Status decode_magic_string(char *magic_string, DecodeInfo *decInfo);

Status decode_data_to_image(char *data,int size,DecodeInfo *decInfo);
/* decode a byte into lsb of image data array */
Status decode_byte_to_lsb(char *data,char *image_buffer);
Status decode_size_to_lsb(int *size,DecodeInfo *decInfo);

Status decode_secret_file_extn_size(int size,DecodeInfo *decInfo);

Status decode_secret_file_extn(char *file_extn,DecodeInfo *decInfo);

Status decode_secret_file_size(long file_size, DecodeInfo *decInfo);

Status decode_secret_file_data(DecodeInfo *decInfo);








#endif


