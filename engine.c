#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME_SIZE 32
#define BUFFER_SIZE 256

/**
 * @brief Allocate space for the filepath and copy the filepath to the file
 * pointer 
 * @param int argv pointer
 * @return char file pointer
 */
char *filepath_init(char **argv)
{
    if(argv[1] != NULL)
    {
        char *file = (char *)calloc(1, FILENAME_SIZE);
        
        if(file == NULL)
        {
            fprintf(stderr, "Could not allocate space for the file\n");
            exit(1);
        }

        strncpy(file, argv[1], FILENAME_SIZE - 1);
        file[FILENAME_SIZE - 1] = '\0';

        return file;
    }
    else
    {
        fprintf(stderr, "Invalid input provided, expencting filename.extension\n");
        exit(1);
    }
}

/**
 * @brief Check the given input provided and perform a callback to alloc and save 
 * the filepath 
 * @param int argc Stores the numbers of provided arguments
 * @param char **argv double pointer
 * @param char*(*on_check_completed)(char **filepath) Callback 
 * @return char pointer
 */
char *check_cmd_args(int argc, char **argv, char*(*on_check_completed)(char **filepath))
{
    if (argc == 1)
    {
        fprintf(stderr, "No extra argument provided, please enter the filepath.extension\n");
        exit(1);
    }
    else if (argc >= 3)
    {
        fprintf(stderr, "Too many argument provided, please enter the filepath.extension\n");
        exit(1);
    }
    else
    {
        return on_check_completed(argv);
    }
}

/**
 * @brief Open the filepath and process the stream by adding to the buffer 
 * the frequency of the characters found in the given filepath
 * @param char filepath pointer The name of the filepath
 * @return void
 */
void open_filepath(char *filepath)
{
    int val;
    int buffer[BUFFER_SIZE] = { 0 };
    FILE *file = fopen(filepath, "r");
    
    if(file == NULL)
    {
        fprintf(stderr, "No file found at the path: %s\n", filepath);
        exit(1);
    }
  
    while ((val = fgetc(file)) != EOF)
    {
        buffer[val]++;
    }
    
    for(int i = 0; i < BUFFER_SIZE; i++)
        if(buffer[i] > 0) printf("frequency index[%d] = %d = %c\n",i,buffer[i],i);
    
    fclose(file);
}

int main(int argc, char **argv)
{
    char *filepath = check_cmd_args(argc, argv, filepath_init);
    open_filepath(filepath);

    free(filepath);
    return 0;
}
