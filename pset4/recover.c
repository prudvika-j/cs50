#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover forensic-image\n");
        return 1;
    }
    // open forensic image
    FILE *inptr = fopen(argv[1], "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 2;
    }
    typedef uint8_t  BYTE; 
    BYTE *buffer = malloc(512);
    char filename[20];
    int count = 0;
    FILE *img;
    while(fread(buffer, 1, 512, inptr)==512)
    {
        
        if(buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if(count > 0)
            {
              fclose(img);  
            }
            sprintf(filename,"%03i.jpg", count);
            count++;
            img =  fopen(filename, "w");
            fwrite(buffer, 1, 512, img);
        }
        else
        {
            if(count > 0)
            {
                fwrite(buffer, 1, 512, img);
            }
        }
    }
        fclose(img);
        fclose(inptr);
        free(buffer);
        return 0;
}