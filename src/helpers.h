//---------------------------------------------
// NAME: Ognian Baruh
// CLASS: XI A
// NUMBER: 21
// PROBLEM: #1
// FILE NAME: helpers.h
// FILE PURPOSE:
// Declarations and descriptions for all helper functions
//---------------------------------------------

#ifndef HELPERS_H
#define HELPERS_H

#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>

// Default value for current directory
#define CURRENT_DIRECTORY "."

// Default value for parent directory
#define PARENT_DIRECTORY ".."

// Struct for flags
// a -> is -A present
// l -> is -l present
// r -> is -R present
struct flags_t {
	bool a;
	bool l;
	bool r;
};

//----------------------------------------------
// FUNCTION: init_flags()
// Allocates memory for flags
// 
// PARAMETERS:
// none
// 
// RETURNS:
// flags (struct flags_t*) -> flags with all values set to false
//----------------------------------------------
struct flags_t* init_flags();

//----------------------------------------------
// FUNCTION: get_flags()
// Reads argument values and searches for flags using getopt()
// 
// PARAMETERS:
// argc (int) -> argument count
// argv (char* const*) -> argument values
// flags (struct flags_t*) -> pointer to the flags struct
// 
// RETURNS:
// optind (int) -> index to the first nonoption argument value
//----------------------------------------------
int get_flags(int argc, char* const argv[], struct flags_t* flags);

//----------------------------------------------
// FUNCTION: get_type_of_file()
// Gets file type from mode_t
// 
// PARAMETERS:
// mode (mode_t) -> contains the type of the file
// 
// RETURNS:
// char (char) -> character, representing the type of the file
//----------------------------------------------
char get_type_of_file(mode_t mode);

//----------------------------------------------
// FUNCTION: print_permissions()
// Gets permissions from mode_t and prints them on the screen
// 
// PARAMETERS:
// mode (mode_t) -> contains the permissions for the file
// 
// RETURNS:
// none
//----------------------------------------------
void print_permissions(mode_t mode);

//----------------------------------------------
// FUNCTION: get_owner_name()
// Gets owner name by uid (user id)
// 
// PARAMETERS:
// uid (uid_t) -> the user id
// 
// RETURNS:
// name (char*) -> the name of the owner
//----------------------------------------------
char* get_owner_name(uid_t uid);

//----------------------------------------------
// FUNCTION: get_group_name()
// Gets group name by gid (group id)
// 
// PARAMETERS:
// gid (gid_t) -> the group id
// 
// RETURNS:
// name (char*) -> the name of the group
//----------------------------------------------
char* get_group_name(gid_t gid);

//----------------------------------------------
// FUNCTION: is_current_directory()
// Checks if file is the current directory
// 
// PARAMETERS:
// file (char*) -> the name of the file
// 
// RETURNS:
// is (bool) -> true/false
//----------------------------------------------
bool is_current_directory(char* file);

//----------------------------------------------
// FUNCTION: is_parent_directory()
// Checks if file is the parent directory 
//
// PARAMETERS:
// file (char*) -> the name of the file
// 
// RETURNS:
// is (bool) -> true/false
//----------------------------------------------
bool is_parent_directory(char* file);

//----------------------------------------------
// FUNCTION: is_hidden_file()
// Checks if the file is hidden
// 
// PARAMETERS:
// file (char*) -> the name of the file
// 
// RETURNS:
// is (bool) -> true/false
//----------------------------------------------
bool is_hidden_file(char* file);

//----------------------------------------------
// FUNCTION: print_partial_file_info()
// Prints type and name of file (when there is no -l)
// 
// PARAMETERS:
// filename (char*) -> the name of the file
// 
// RETURNS:
// none
//----------------------------------------------
void print_partial_file_info(char* filename);

//----------------------------------------------
// FUNCTION: print_full_file_info()
// Prints full information of file (when there is -l)
// 
// PARAMETERS:
// path_to_file (char*) -> full path to file, in order to use stat()
// filename (char *) -> the name of the file
// 
// RETURNS:
// none
//----------------------------------------------
void print_full_file_info(char* path_to_file, char* filename);

//----------------------------------------------
// FUNCTION: get_file_block_count()
// Gets block count from the stat of a file
// 
// PARAMETERS:
// path_to_file (char*) -> full path to file, in order to use stat()
// 
// RETURNS:
// block_count (int) -> number of blocks for the file
//----------------------------------------------
int get_file_block_count(char* path_to_file);

//----------------------------------------------
// FUNCTION: get_total_block_count()
// Gets block count for all the files in a directory
// 
// PARAMETERS:
// directory (char*) -> full path to directory
// flags (struct flags_t*) -> flags from the command
// 
// RETURNS:
// block_count (int) -> number of blocks for all the files in the directory
//----------------------------------------------
int get_total_block_count(char* directory, struct flags_t* flags);

//----------------------------------------------
// FUNCTION: print_directory_error_message()
// Prints error message for directory errors 
// 
// PARAMETERS:
// directory (char*) -> the name of the directory
// 
// RETURNS:
// none
//----------------------------------------------
void print_directory_error_message(char* directory);

//----------------------------------------------
// FUNCTION: print_file_error_message()
// Prints error message for file errors 
// 
// PARAMETERS:
// file (char*) -> the name of the file
// 
// RETURNS:
// none
//----------------------------------------------
void print_file_error_message(char* file);

//----------------------------------------------
// FUNCTION: has_subdirectories
// Check if a directory has subdirectories 
// 
// PARAMETERS:
// directory (char*) -> the name of the directory
// 
// RETURNS:
// has (bool) -> true/false
bool has_subdirectories(char* directory, struct flags_t* flags);

#endif