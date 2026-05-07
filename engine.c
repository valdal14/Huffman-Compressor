#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffengine.h"

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
 * @param int freq_buffer The main buffer 
 * @return void
 */
void open_filepath(char *filepath, int *freq_buffer)
{
    int val;
    FILE *file = fopen(filepath, "r");
    
    if(file == NULL)
    {
        fprintf(stderr, "No file found at the path: %s\n", filepath);
        exit(1);
    }
  
    while ((val = fgetc(file)) != EOF)
    {
        freq_buffer[val]++;
    }
    
    fclose(file);
}

/**
 * @brief Allocates HuffNodes for active characters and stores their pointers.
 * @param freq_buffer The populated frequency array.
 * @param nodes Array of pointers to HuffNodes (Double Pointer).
 * @return int The number of unique active nodes created.
 */
int build_node_array(int *freq_buffer, struct HuffNode **nodes)
{
    int unique_values = 0;

    for(int i = 0; i < BUFFER_SIZE; i++)
    {
        if(freq_buffer[i] > 0)
        {
            struct HuffNode *node = (struct HuffNode *)malloc(sizeof(struct HuffNode));
            if(node == NULL) exit(1);
            
            node->data = i;
            node->freq = freq_buffer[i];
            node->left = NULL;
            node->right = NULL;
            
            nodes[unique_values] = node;
            unique_values++;
        }
    }
    
    return unique_values;
}

/**
 * @brief Comparator function for qsort to sort HuffNodes in ascending order by frequency.
 * @param a Void pointer to the first HuffNode pointer.
 * @param b Void pointer to the second HuffNode pointer.
 * @return int Negative if a < b, 0 if equal, Positive if a > b.
 */
int compare_nodes(const void *a, const void *b)
{
    // Cast the void pointers to Double Pointers 
    // and dereference once to get the actual struct HuffNode pointers.
    struct HuffNode *nodeA = *(struct HuffNode **)a;
    struct HuffNode *nodeB = *(struct HuffNode **)b;

    // Return the difference in their frequencies
    return (nodeA->freq - nodeB->freq);
}

/**
 * @brief Prints out the sorted array 
 * @param struct HuffNode array of pointers
 * @param int size the size of the array
 * @return void
 */
void print_sorted_array(struct HuffNode **nodes, int size)
{
    printf("\n--- Sorted Nodes ---\n");
   
    for(int i = 0; i < size; i++)
        printf("Char: %c | Freq: %d\n", nodes[i]->data, nodes[i]->freq);
}

/**
 * @brief Factory method used to build the HuffNode tree
 * @param struct HuffNode nodes array pointer 
 * @param int size The size of the sorted array 
 * @return struct HuffNode pointer 
 */
struct HuffNode *build_huffman_tree(struct HuffNode **nodes, int size)
{
    while(size > 1)
    {
        struct HuffNode *root = (struct HuffNode *)calloc(1, sizeof(struct HuffNode));
        if(root == NULL) exit(1);
        root->data = '\0';
        root->left = nodes[0];
        root->right = nodes[1];
        root->freq = nodes[0]->freq + nodes[1]->freq;
        // Put the parent into the first slot.
        nodes[0] = root;
        // Shift: Close the gap at index 1.
        // We start at index 1, and copy the item from the right (i + 1) into the current slot (i).
        for (int i = 1; i < size - 1; i++)
            nodes[i] = nodes[i + 1];
        
        // Shrink the desk and Re-sort
        size--;
        qsort(nodes, size, sizeof(struct HuffNode *), compare_nodes);
    }

    return nodes[0];
}

int main(int argc, char **argv)
{
    int buffer[BUFFER_SIZE] = { 0 };
    struct HuffNode *nodes[BUFFER_SIZE];

    char *filepath = check_cmd_args(argc, argv, filepath_init);
    open_filepath(filepath, buffer);

    int unique_values = build_node_array(buffer, nodes);
   
    // Sort the array of pointers
    qsort(nodes, unique_values, sizeof(struct HuffNode *), compare_nodes);
    
    print_sorted_array(nodes, unique_values);

    struct HuffNode *root = build_huffman_tree(nodes, unique_values);
    printf("Root frequency = %d\n", root->freq);

    free(filepath);
    return 0;
}
