
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize factor infile outfile\n");
        return 1;
    }
    

    //remember factor
    int n = atoi(argv[1]);
    
    if(n < 0 || n > 100)
    {
        fprintf(stderr, " Factor must be a positive integer less than or equal to 100\n");
        return 1;
    }
    
    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // updating header information
    bi.biWidth = bi.biWidth*n;
    bi.biHeight = bi.biHeight*n; 
    
    // determine the outfile padding
    int paddingo = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
        
    bi.biSizeImage = (bi.biWidth * abs(bi.biHeight) * sizeof(RGBTRIPLE)) + (paddingo * abs(bi.biHeight));
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    
    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // determine padding for scanlines
    int paddingi = (4 - ((bi.biWidth/n) * sizeof(RGBTRIPLE)) % 4) % 4;

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight/n); i < biHeight; i++)
    {
        RGBTRIPLE triples[(bi.biWidth/n)];
        // iterate over pixels in scanline
        for (int j = 0; j < (bi.biWidth/n); j++)
        {
            // temporary storage
            RGBTRIPLE triple;
            
            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
            
            triples[j].rgbtBlue = triple.rgbtBlue;
            triples[j].rgbtGreen = triple.rgbtGreen;
            triples[j].rgbtRed = triple.rgbtRed;
            
        }
        
        for(int h = 0; h < n; h++)
        {
        
            for(int j = 0; j < (bi.biWidth/n); j++)
            {
                for( int k = 0; k < n; k++)
                {
                    // write RGB triple to outfile
                    fwrite(&triples[j], sizeof(RGBTRIPLE), 1, outptr);
                }
            }
            // write the padding to outfile
            for (int l = 0; l < paddingo; l++)
            {
            fputc(0x00, outptr);
            }
        }
        
        // skip over padding, if any
        fseek(inptr, paddingi, SEEK_CUR);
        
    }
    

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
