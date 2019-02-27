#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Wrong # of inputs.");
        return 1;
    }

    char *infile = argv[1]; //get photo
    char filename[8];
    unsigned char buffer[512];
    int photoIndex = 0;

    // open input file
    FILE *raw_file = fopen(infile, "r");

    if (raw_file == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    FILE *img = NULL;

    while (fread(buffer, 512, 1, raw_file) == 1)
    {
        //if found new jpeg
        if ((buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0))
        {
            //if image already exits close file
            if (img != NULL)
            {
                fclose(img);
            }

            //make new file and open it
            sprintf(filename, "%03i.jpg", photoIndex);
            img = fopen(filename, "w");
            photoIndex++;
        }

        //if image has been opened start writing to file
        if (img != NULL)
        {
            fwrite(buffer, sizeof(buffer), 1, img);
        }
    }

    //close last image
    if (img != NULL)
    {
        fclose(img);
    }
    fclose(raw_file);
}