//
//  main.c
//  UyvyToNv16
//
//  Created by Hank Lee on 9/26/15.
//  Copyright (c) 2015 Hank Lee. All rights reserved.
//

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "UyvyToNv16.h"


#define MAX_WIDTH   3840
#define MAX_HEIGHT  2160

static uint8_t img       [MAX_WIDTH * MAX_HEIGHT * 2];
static uint8_t y_dst     [MAX_WIDTH * MAX_HEIGHT];
static uint8_t u_et_v_dst[MAX_WIDTH * MAX_HEIGHT];


int main(int argc, const char * argv[]) {
    int fd_rd;
    int fd_wr;

    size_t rd_sz;
    
    uint32_t width;
    uint32_t height;
    uint32_t wxh;

    char *cp;
    char output_file_name[256];

    
    if (argc < 4)
    {
        fprintf(stderr, "useage: %s [input_file] [width] [height]\n", argv[0]);
        
        return -1;
    }
    
    
    rd_sz   = 0;
    width   = 0;
    height  = 0;
    wxh     = 0;
    cp      = NULL;
    memset(output_file_name, 0, sizeof(output_file_name));
    
    // get input file name from comand line
    fd_rd = open(argv[1], O_RDONLY);
    if (fd_rd < 0)
    {
        perror(argv[1]);
        exit(EXIT_FAILURE);
    }
    
    // specify output file name
    cp = strchr(argv[1], '.');
    strncpy(output_file_name, argv[1], cp - argv[1]);
    strcat(output_file_name, "_nv16");
    strcat(output_file_name, cp);
    
    fd_wr = open(output_file_name, O_WRONLY | O_CREAT, S_IRUSR);
    
    width   = atoi(argv[2]);
    height  = atoi(argv[3]);
    
    wxh = width * height;
 
    fprintf(stderr, "Processing: ");
    
    while (1)
    {
        rd_sz = read(fd_rd, img, wxh * 2);
        
        if (rd_sz == wxh * 2)
        {
            unpack
            (
                wxh,
                (uint32_t *) y_dst,
                (uint32_t *) u_et_v_dst,
                img
            );
            
            write(fd_wr, y_dst, wxh);
            write(fd_wr, u_et_v_dst, wxh);
        }
        else
        {
            break;
        }
        fputc('.', stderr);
        fflush(stderr);
    }
    
    close(fd_rd);
    close(fd_wr);
    
    fprintf(stderr, "Done\n");
    fprintf(stderr, "Output file: %s\n", output_file_name);
    
    return 0;
}
