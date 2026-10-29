#include <stdio.h>
#include "decode.h"
#include "types.h"
#include<string.h>
#include"common.h"
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    if(strstr(argv[2],".bmp")!=NULL)
    {
       decInfo->src_image_fname=argv[2];
    }
    else
    {
        return e_failure;
    }
    if(argv[3] != NULL)
    {
        //validate if .txt is the extn
           decInfo->secret_fname=argv[3];
    }
    else    
    {
        decInfo->secret_fname="default.txt"; 
    }
    return e_success;
}
Status open_files1(DecodeInfo *decInfo)
{
    decInfo->fptr_src_image=fopen(decInfo->src_image_fname,"r");
    if(decInfo->fptr_src_image == NULL)
    {
        perror("fopen");
        fprintf(stderr,"ERROR: Unable to open file %s\n",decInfo->src_image_fname);
        return e_failure;
    }
    decInfo->fptr_secret=fopen(decInfo->secret_fname,"w");
    if(decInfo->fptr_secret==NULL)
    {
        perror("fopen");
        fprintf(stderr,"ERROR: Unable to open file %s\n",decInfo->secret_fname);
        return e_failure;
    }
    return e_success;
}
Status decode_byte_to_lsb(char *data,char *image_buffer)
{
    *data=0;
    for(int i=0;i<8;i++)
    {
        *data = *data | ((image_buffer[i]&1)<<i);
    }
    return e_success;
}

Status decode_data_to_image(char *data,int size,DecodeInfo *decInfo)
{
    for(int i=0;i<size;i++)
    {
    fread(decInfo->image_data,8,1,decInfo->fptr_src_image);
    decode_byte_to_lsb(&data[i],decInfo->image_data);
    }
    data[size]='\0';
    return e_success;
}

Status decode_magic_string(char *magic_string, DecodeInfo *decInfo)
{
    decode_data_to_image(magic_string,strlen(MAGIC_STRING),decInfo);
    if(strcmp(magic_string,MAGIC_STRING)==0)
    return e_success;
   // return e_failure;
}

Status decode_size_to_lsb(int *size, DecodeInfo *decInfo)
{
    char str[32];

    *size = 0;

    fread(str, 32, 1, decInfo->fptr_src_image);

    for (int i = 0; i < 32; i++)
    {
        *size = *size | ((str[i] & 1) << i);
    }

    return e_success;
}

Status decode_secret_file_extn_size(int size,DecodeInfo *decInfo)
{
    decode_size_to_lsb(&size,decInfo);
    decInfo->extension_size=size;
    return e_success;
}

Status decode_secret_file_extn(char *file_extn,DecodeInfo *decInfo)
{
   decode_data_to_image(file_extn,decInfo->extension_size,decInfo);
   return e_success; 
}

Status decode_secret_file_size(long file_size, DecodeInfo *decInfo)
{
    int size;
    decode_size_to_lsb(&size,decInfo);
    decInfo->size_secret_file=size;
    return e_success;
}
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char ch;

    for(long i = 0; i < decInfo->size_secret_file; i++)
    {
        fread(decInfo->image_data, 8, 1, decInfo->fptr_src_image);

        decode_byte_to_lsb(&ch, decInfo->image_data);

        //printf("%c", ch);      // Print each decoded character

        fputc(ch, decInfo->fptr_secret);
    }

    //printf("\n");

    return e_success;
}

Status do_decoding(DecodeInfo *decInfo)
{
    fseek(decInfo->fptr_src_image,54,SEEK_SET);
    char magic[10];
    if(decode_magic_string(magic,decInfo)==e_success)
    {
        printf("Info: Decoded magic string successfully\n");
        if(decode_secret_file_extn_size(strlen(decInfo->extn_secret_file),decInfo)==e_success)
        {
            printf("Info: Decoded secret_file_extn_size successfully\n");
            if(decode_secret_file_extn(decInfo->extn_secret_file,decInfo) == e_success)
            {
                printf("Info: decoded secret file extn successfully\n");
                if(decode_secret_file_size(0,decInfo)==e_success)
                {
                    printf("Info: decoded secret fil size successfully\n");
                    if(decode_secret_file_data(decInfo)==e_success)
                    {
                        printf("Info: decode secret file data successfully\n");
                    }
                    else
                    {
                        printf("Failed to decode secret file daTA\n");
                        return e_success;
                    }
                }
                else
                {
                    printf("Failed to decode secret file size\n");
                    return e_failure;
                }
            }
            else
            {
                printf("Failed to decode secret file extn\n");
                return e_failure;
            }
        }
        else
        {
            printf("Failed to decode secret file extn size\n");
            return e_failure;
        }
    }
    else
    {
        printf("Failed to decode magic string\n");
        return e_failure;
    }
    return e_success;

}





