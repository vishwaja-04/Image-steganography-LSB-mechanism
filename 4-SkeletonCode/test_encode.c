#include <stdio.h>
#include "encode.h"
#include "types.h"
#include<string.h>
#include "decode.h"
int main(int argc,char *argv[])
{
    EncodeInfo E1;
    DecodeInfo D1;
    //validate cla
    if(argc>=2)
    {
        int res=check_operation_type(argv);
        if(res==e_encode)
        {
            printf("encoding is selected\n");
            if(argc < 4)
            {
                printf("Insufficient arguments for encoding\n");
                return 0;
            }
            if(read_and_validate_encode_args(argv, &E1)==e_success)
            {
                printf("Info: Read and validate encode args is success\n");
                if(open_files(&E1)==e_success)
                {
                    printf("Info: files are opened successfully\n");
                    if(do_encoding(&E1) == e_success)
                    {
                        printf("Info: encoding is success\n");
                    }
                    else
                    {
                        printf("Failed to encode\n");
                        return 0;
                    }
                }
                else
                {
                    printf("Info: open files are failured\n");
                    return 0;
                }
            }
            else
            {
                printf("Info: Read and validate encode args is failure\n");
                return 0;

            }
        }
        else if(res==e_decode)
        {
            printf("decoding is selected\n");
            if(argc < 4)
            {
                printf("Insufficient arguments for decoding\n");
                return 0;
            }
            //decoding code
            if(read_and_validate_decode_args(argv, &D1)==e_success)
            {
                printf("Info: Read and validate decode args is success\n");
                if(open_files1(&D1)==e_success)
                {
                    printf("Info : Files are opened successfully\n");
                    if(do_decoding(&D1) == e_success)
                    {
                        printf("Info: decoding is success\n");
                    }
                    else
                    {
                        printf("Failed to decode\n");
                        return 0;
                    }
                }
                else
                {
                    printf("Open files is failure\n");
                    return 0;
                }
            }
            else
            {
                printf("Info: Read and validate decode args is failure\n");
                return 0;
            }

        }
        else
        {
            printf("Invalid option\n");
            printf("For encoding: ./a.out -e beautiful.bmp secret.txt [stegno.bmp]\n");
            printf("For decoding: ./a.out -d stegno.bmp default.txt\n");
        }

       
    }
    else
    {
        printf("No operation selected\n");
        printf("For encoding: ./a.out -e beautiful.bmp secret.txt [stegno.bmp]\n");
        printf("For decoding: ./a.out -d stegno.bmp [output.txt]\n");
    }


    return 0;
}
OperationType check_operation_type(char *argv[])
{
   if(strcmp(argv[1],"-e")==0)
       return e_encode;
    else if(strcmp(argv[1],"-d")==0)
        return e_decode;
    else
       return e_unsupported;


}

