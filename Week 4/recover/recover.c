#include <stdio.h>
#include <stdlib.h>

#define BLOCK 512

int main(int argc, char *argv[])
{
    // Check input validity
    if (argc != 2)
    {
        printf("USAGE: ./recover IMAGE\n");
        return 1;
    }

    // Open file to read
    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        printf("File not valid\n");
        return 1;
    }

    int img_count = 0;
    unsigned char buffer[BLOCK];
    FILE *img = NULL;
    char filename[8]; // No need to allocate dynamically

    while (fread(buffer, sizeof(char), BLOCK, file) == BLOCK) // Read full blocks
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            if (img != NULL)
            {
                fclose(img); // Close the previous file
            }
            sprintf(filename, "%03i.jpg", img_count);
            img = fopen(filename, "w");
            if (img == NULL)
            {
                fclose(file);
                return 1;
            }
            img_count++;
        }
        if (img != NULL)
        {
            fwrite(buffer, sizeof(char), BLOCK, img);
        }
    }

    if (img != NULL)
    {
        fclose(img); // Close the last image file
    }
    fclose(file);

    return 0;
}
