// ============================================================================
// ============================================================================
// utils.h: 
// This is the "module" (component) that holds all sort of general purpose
// utilities functions and definitions.
// 
// ============================================================================
// ============================================================================
#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ==================
// Global definitions
// ==================
#define IN
#define OUT
#define INOUT

uint32_t utilsHash(IN const char* str);
void utilsPrintBytesAscii(IN const uint8_t* byteArr, IN const unsigned short numBytesToDisplay);
void utilsdisplayDnsName(IN const uint8_t* questionName);
void utilsGetCmdLineArgs(IN int argc, IN char** argv);

// ===============================
// Linked list related definitions
// ===============================

// Define the node structure that will hold IPv4/IPv6 address as a string
typedef struct node
{
    char* val;
    struct node* next;
} node;

// Function to allocate space and insert a string to the val field in a node
void set_node_value(IN struct node* n, IN const char* value);

// Function to free the space allocated and used by a node
void free_node(IN struct node* n);

// Function to insert a new node with a given value to the end of the list
void insert_end(IN struct node** head, IN const char* value);

// Function to remove a node from the list if it is found
void remove_node(struct node** head, const char* value);

// Function to find a node in the list according to a char* value
struct node* find_node(struct node* head, const char* value);

//void display_list(struct node* head);

/*
// Function to free the entire linked list
void free_list(struct node* head);
*/
