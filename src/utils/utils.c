#include <stdio.h>

#include "include/utils.h"

uint32_t utilsHash(IN const char* str)
{
    uint32_t hash = 5381;
    int c;
    while ((c = *str++))
    {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
        
    return hash;
}

void utilsdisplayDnsName(IN const uint8_t* questionName)
{
    uint8_t len = questionName[0];  // First byte is the length of the first label
    int pos = 1;
    printf("len is:%u\n", len);

    while (len > 0 && pos < 255) { // DNS_QNAME_MAX_LEN
        for (int i = 0; i < len; i++) {
            printf("%c", questionName[pos + i]);
        }

        pos += len; // Move past this label
        len = questionName[pos]; // Next label length
        pos++;

        if (len > 0) {
            printf("."); // Print the dot between segments
        }
    }
    printf("\n");
}

void utilsPrintBytesAscii(IN const uint8_t* byteArr, IN const unsigned short numBytesToDisplay)
{
    for (unsigned short i = 0; i < numBytesToDisplay; ++i)
    {
        // Print each byte in hexadecimal format (2-digit uppercase hex)
        printf("%c ", byteArr[i]);
    }
}

void printBytes(IN const unsigned char* byteArr, IN const unsigned short numBytesToDisplay)
{
    const char funcName [] = "printBytes - ";
    printf("%s about to display %u bytes that starts at address:%p\n", funcName, numBytesToDisplay, (void*)byteArr);
    for (unsigned short i = 0; i < numBytesToDisplay; ++i)
    {
        // Print each byte in hexadecimal format (2-digit uppercase hex)
        printf("%02X ", byteArr[i]);
    }

    printf("\n");
}

void utilsGetCmdLineArgs(IN int argc, IN char** argv)
{
    const char funcName [] = "utilsGetCmdLineArgs - ";
    printf("%s got %d arguments from command line\n", funcName, argc);
    if (argc == 1)
    {
        // No arguments were passed (only the program name is present)
        printf("%s no arguments were passed\n", funcName);
    } 
    else
    {
        // Print all arguments, including the program name
        printf("Arguments:\n");
        for (int i = 0; i < argc; ++i)
        {
            printf("%s argv[%d]: %s\n", funcName, i, argv[i]);
        }
    }
}

// Function to allocate space and insert a string to the val field in a node
void set_node_value(IN struct node* n, IN const char* value)
{
    if (NULL == n || NULL == value)
    {
        return;
    }

    n->val = malloc(strlen(value) + 1);
    if (n->val) 
    {
        strcpy(n->val, value);
    }
}

// Function to free the space allocated and used by a node
void free_node(IN struct node* n)
{
    if (NULL == n)
    {
        return;
    }

    free(n->val);  // Free the allocated string
    n->val = NULL; // Set val to NULL for safety
    free(n);       // Free the node itself
}

// Function to insert a new node with a given value to the end of the list
void insert_end(IN struct node** head, IN const char* value)
{
    struct node* new_node = malloc(sizeof(struct node));
    if (!new_node) return;  // Check for allocation failure

    set_node_value(new_node, value);  // Set the value of the new node
    new_node->next = NULL;

    if (*head == NULL) {
        *head = new_node;  // If the list is empty, set new node as head
    } else {
        struct node* current = *head;
        while (current->next != NULL) {
            current = current->next;  // Traverse to the last node
        }
        current->next = new_node;  // Link the new node at the end
    }
}

// Function to remove a node from the list if it is found
void remove_node(struct node** head, const char* value)
{
    struct node* current = *head;
    struct node* prev = NULL;

    while (current != NULL) {
        if (current->val && strcmp(current->val, value) == 0) {  // Node found
            if (prev == NULL) {
                *head = current->next;  // Update head if removing the first node
            } else {
                prev->next = current->next;  // Unlink the node
            }
            free_node(current);  // Free the memory of the found node
            return;
        }
        prev = current;
        current = current->next;
    }
}

// Function to find a node in the list according to a char* value
struct node* find_node(struct node* head, const char* value)
{
    struct node* current = head;
    while (current != NULL) {
        if (current->val && strcmp(current->val, value) == 0) {
            return current;  // Node found
        }
        current = current->next;
    }
    return NULL;  // Node not found
}

/*
void display_list(struct node* head)
{
    struct node* current = head;
    while (current != NULL)
    {
        printf("%s--->", current->val);
        current = current->next;
    }

    printf("NULL\n");
}


// Function to free the entire linked list
void free_list(struct node* head)
{
    struct node* current = head;
    while (current != NULL) {
        struct node* temp = current;
        current = current->next;
        free_node(temp);  // Free each node
    }
}
*/