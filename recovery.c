#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 2)
    {
        printf("Usage: %s <forensic_image>\n", argv[0]);
        return 1;
    }

    // Open the forensic image
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Error opening %s for reading\n", argv[1]);
        return 1;
    }

    // Initialize variables
    unsigned char buffer[512];
    int jpg_count = 0;
    FILE *output = NULL;
    char filename[8];

    // Read the input file block by block
    while (fread(buffer, 1, 512, input) == 512)
    {
        // Check for JPEG signature
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            // Close previous output file
            if (output != NULL)
                fclose(output);

            // Create a new output file
            sprintf(filename, "%03i.jpg", jpg_count);
            output = fopen(filename, "w");
            if (output == NULL)
            {
                printf("Error creating %s\n", filename);
                fclose(input);
                return 1;
            }

            jpg_count++;
        }

        // Write buffer to output file
        if (output != NULL)
            fwrite(buffer, 1, 512, output);
    }

    // Clean up
    if (output != NULL)
        fclose(output);
    fclose(input);

    return 0;
}
