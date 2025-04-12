//---------------------------------------------
// NAME: Ognian Baruh
// CLASS: XI A
// NUMBER: 21
// PROBLEM: #1
// FILE NAME: ls.c
// FILE PURPOSE:
// Implementation of 'ls' in C.
// Supported flags: -l, -A and -R
// Supported files: all kind of files and combination of different files
//---------------------------------------------

#include "helpers.h"

// ls() is declared here since process_directory() calls ls() and ls() calls process_directory()
void ls(char* path_to_file, struct flags_t* flags);

//----------------------------------------------
// FUNCTION: process_file()
// Executes ls on a file, checking for -A and -l flags
// 
// PARAMETERS:
// path_to_file (char*) -> full path to file
// filename (char*) -> name of the file
// flags (struct flags_t*) -> flags of the command
// 
// RETURNS:
// none
//----------------------------------------------
void process_file(char* path_to_file, char* filename, struct flags_t* flags) {
	// Stats struct
	struct stat st;

	// Check if file exists
	if(stat(path_to_file, &st) == 0) {
		
		// Check if file should be listed
		if(
			(!flags -> a && is_hidden_file(filename)) ||	// Hidden file without -A
			is_current_directory(filename) ||				// Current directory
			is_parent_directory(filename) 					// Parent directory
		) {
			return;
		}

		// Check for -l
		if(flags -> l) {
			print_full_file_info(path_to_file, filename);
		}

		// Basic case
		else {
			print_partial_file_info(filename);
		}
	}
	// File doesn't exist
	else {
		// Print error
		print_file_error_message(filename);
	}
}

//----------------------------------------------
// FUNCTION: process_directory()
// Processes directory, handles -R and -A flags
// 
// PARAMETERS:
// directory (char*) -> name of the directory
// flags (struct flags_t*) -> flags of the command
// 
// RETURNS:
// none
//----------------------------------------------
void process_directory(char* directory, struct flags_t* flags) {
	// Check for -l
	if(flags -> l) {
		// Get block count
		int block_count = get_total_block_count(directory, flags);

		// -1 is the error code
		if(block_count == -1) {
			// Print error message
			print_directory_error_message(directory);

			return;
		}

		// Print total block count
		printf("total %d\n", block_count);
	}

	// Define subdirectories, used only if -R flag is true
	char **subdirectories;
	int subdirectories_index = 0;
	int number_of_allocated_subdirectories = 10;

	if(flags -> r) {
		// Allocate memory for subdirectories
		subdirectories = malloc(10 * sizeof(char*));
		for(int i = 0; i < 10; i ++) {
			subdirectories[i] = malloc(100 * sizeof(char*));
		}
	}

	// String for path to file
	char *path_to_file = malloc(100 * sizeof(char));

	// Entry for each file in directory
	struct dirent *entry;

	// Open directory
	DIR *dir = opendir(directory);

	// Check for possible error
	if(dir == NULL) {
		// Print error message
		print_directory_error_message(directory);

		return;
	}

	while((entry = readdir(dir))) {
		// Check if file should be processed
		if(
			(!flags -> a && is_hidden_file(entry -> d_name)) ||		// Hidden file without -A
			is_current_directory(entry -> d_name) ||				// Current directory
			is_parent_directory(entry -> d_name) 					// Parent directory
		) {
			continue;
		}

		// Copy directory to file
		strcpy(path_to_file, directory);

		// Add "/" if needed
		if(path_to_file[strlen(path_to_file) - 1] != '/') {
			strcat(path_to_file, "/");
		}

		// Add file name to file
		strcat(path_to_file, entry -> d_name);

		// Process file
		process_file(path_to_file, entry -> d_name, flags);

		// Define stat struct
		struct stat st;

		// Get stats of file
		stat(path_to_file, &st);

		// Check if file is a directory and if -R flag is true
		if(get_type_of_file(st.st_mode) == 'd' && flags -> r) {
			// Copy file to subdirectories array
			strcpy(subdirectories[subdirectories_index], path_to_file);
			subdirectories_index ++;

			// Check if there is need for more memory
			if(subdirectories_index + 1 == number_of_allocated_subdirectories) {
				number_of_allocated_subdirectories += 10;

				// Allocate more memory
				subdirectories = realloc(subdirectories, number_of_allocated_subdirectories * sizeof(char*));
				for(int i = subdirectories_index + 1; i < number_of_allocated_subdirectories; i ++) {
					subdirectories[i] = malloc(100 * sizeof(char*));
				}
			}

		}
	}

	// Close directory
	closedir(dir);

	// Call ls for subdirectories if -R flag is true
	if(flags -> r) {
		for(int i = 0; i < subdirectories_index; i++) {
			printf("\n%s:\n", subdirectories[i]);
			ls(subdirectories[i], flags);
		}
	}

	// Free allocated memory
	free(path_to_file);

	// Free allocated memory for subdirectories
	if(flags -> r) {
		for(int i = 0; i < number_of_allocated_subdirectories; i++) {
			free(subdirectories[i]);
		}
		free(subdirectories);
	}
}

//----------------------------------------------
// FUNCTION: ls()
// Checks if the file is file or directory and calls process_file/process_directory
// 
// PARAMETERS:
// path_to_file (char*) -> full path to file
// flags (struct flags_t*) -> flags in the command
// 
// RETURNS:
// none
//----------------------------------------------
void ls(char* path_to_file, struct flags_t* flags) {
	// Define stat struct
	struct stat st;

	// Define file name
	char* filename = malloc(100 * sizeof(char));

	// Get stats for the file
	stat(path_to_file, &st);
	
	// Check if file is a directory
	if(get_type_of_file(st.st_mode) == 'd') {
		process_directory(path_to_file, flags);
	}
	// File
	else {
		// Check if file is path
		if(strchr(path_to_file, '/')) {
			// Split path at last '/' to get file name
			filename = strrchr(path_to_file, '/') + 1;
		}
		else {
			// Path is file name
			strcpy(filename, path_to_file);
		}

		process_file(path_to_file, filename, flags);
	}

	// Free allocated memory
	free(filename);
}

int main(int argc, char *argv[]) {
	// Initialize flags
	struct flags_t *flags = init_flags();

	// Get flags and optind
	optind = get_flags(argc, argv, flags);

	// Go to first nonoption argument
	argv += optind;

	// Remove options from argument block_count
	argc -= optind;

	// Flag for printed ls info
	int has_printed_ls_info = 0;
	// When has_printed_ls_info is 0, I don't print a new line before the listed directory

	// ls on current directory
	if(argc == 0) {
		// If -R flag and directory has subdirectories, I should print its name
		if(flags -> r) {
			printf("%s:\n", CURRENT_DIRECTORY);
		}
		ls(CURRENT_DIRECTORY, flags);
	}
	// ls on the only argument
	else if(argc == 1) {
		// Define stats struct
		struct stat st;

		// Get stats
		stat(argv[0], &st);

		// Check if file is directory
		if(get_type_of_file(st.st_mode) == 'd') {
			// If -R flag and directory has subdirectories, I should print its name
			if(has_subdirectories(argv[0], flags) && flags -> r) {
				printf("%s:\n", argv[0]);
			}
		}

		// Call ls
		ls(argv[0], flags);
	}
	// ls on multiple arguments
	else {
		// Print all files
		for(int i = 0; i < argc; i++) {
			// Define stats struct
			struct stat st;

			// Get stats
			stat(argv[i], &st);

			// Check if file is not a directory
			if(get_type_of_file(st.st_mode) != 'd') {
				// Call ls
				ls(argv[i], flags);
				has_printed_ls_info = 1;
			}
		}

		// Print all directories
		for(int i = 0; i < argc; i++) {
			// Define stats struct
			struct stat st;

			// Get stats
			stat(argv[i], &st);

			// Check if file is a directory
			if(get_type_of_file(st.st_mode) == 'd') {
				// Check if has to print new line
				if(has_printed_ls_info == 1) {
					printf("\n");
				}

				// Set has_printed_ls_info to 1
				has_printed_ls_info = 1;

				// Print header for directory
				printf("%s:\n", argv[i]);

				// Call ls
				ls(argv[i], flags);
			}
		}
	}

	// Free allocated memory
	free(flags);

	return 0;
}