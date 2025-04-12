//---------------------------------------------
// NAME: Ognian Baruh
// CLASS: XI A
// NUMBER: 21
// PROBLEM: #1
// FILE NAME: helpers.c
// FILE PURPOSE:
// Definition of all functions declared in helpers.h
//---------------------------------------------

#include "helpers.h"

struct flags_t* init_flags() {
	struct flags_t* flags = malloc(sizeof(struct flags_t));
	flags -> a = false;
	flags -> l = false;
	flags -> r = false;

	return flags;
}

int get_flags(int argc, char* const argv[], struct flags_t* flags) {
	char ch;
	while ((ch = getopt(argc, argv, "AlR")) != -1) {
		switch (ch) {
			case 'A':
				flags -> a = true;
				break;
			case 'l':
				flags -> l = true;
				break;
			case 'R':
				flags -> r = true;
				break;
			case '?':
			default:
				break;
		}
	}

	return optind;
}

char get_type_of_file(mode_t mode) {
	if(S_ISREG(mode)) {
		return '-';
	}
	else if(S_ISDIR(mode)) {
		return 'd';
	}
	else if(S_ISCHR(mode)) {
		return 'c';
	}
	else if(S_ISBLK(mode)) {
		return 'b';
	}
	else if(S_ISFIFO(mode)) {
		return 'p';
	}
	else if(S_ISLNK(mode)) {
		return 'l';
	}
	else {
		return 's';
	}
}

void print_permissions(mode_t mode) {
	printf("%c", (mode & S_IRUSR) ? 'r' : '-');
	printf("%c", (mode & S_IWUSR) ? 'w' : '-');
	printf("%c", (mode & S_IXUSR) ? 'x' : '-');
	printf("%c", (mode & S_IRGRP) ? 'r' : '-');
	printf("%c", (mode & S_IWGRP) ? 'w' : '-');
	printf("%c", (mode & S_IXGRP) ? 'x' : '-');
	printf("%c", (mode & S_IROTH) ? 'r' : '-');
	printf("%c", (mode & S_IWOTH) ? 'w' : '-');
	printf("%c", (mode & S_IXOTH) ? 'x' : '-');
}

char* get_owner_name(uid_t uid) {
	// Define struct
	struct passwd *pwd;

	// Get info about owner by user id
	pwd = getpwuid(uid);

	// Return name of owner
	return pwd -> pw_name;
}

char* get_group_name(gid_t gid) {
	// Define struct 
	struct group *grp;

	// Get info about group by group id
	grp = getgrgid(gid);

	// Return name of group
	return grp -> gr_name;
}

bool is_current_directory(char* file) {
	return !strcmp(file, CURRENT_DIRECTORY);
}

bool is_parent_directory(char* file) {
	return !strcmp(file, PARENT_DIRECTORY);
}

bool is_hidden_file(char* file) {
	return file[0] == '.';
}

void print_partial_file_info(char* filename) {
	// Print file name
	printf("%s\n", filename);
}

void print_full_file_info(char* path_to_file, char* filename) {
	// Define stats struct
	struct stat st;

	// Get file stats
	stat(path_to_file, &st);

	// Print type of file
	printf("%c", get_type_of_file(st.st_mode));

	// Print permissions
	print_permissions(st.st_mode);

	// Print hard links
	printf(" %ld", st.st_nlink);

	// Print owner name
	printf(" %s", get_owner_name(st.st_uid));

	// Print group name
	printf(" %s", get_group_name(st.st_gid));

	// Print file size
	printf(" %ld ", st.st_size);

	// Get last modification time in struct
	struct tm *mtime;
	mtime = localtime(&st.st_mtime);

	// Time string
	char time_string[30];

	// Get current time
	time_t now;
	time(&now);

	// Get last modification time to time_t;
	time_t lmtime;
	lmtime = mktime(mtime);

	// Check if file is older than six months
	// 6 months, 30 days, 24 hours, 60 minutes, 60 seconds
	if(difftime(now, lmtime) < (6 * 30 * 24 * 60 * 60)) {
		// Get time struct of older file
		strftime(time_string, 30, "%b %e %H:%M", mtime);

		// %b -> abbreviated month name in format 'mmm'
		// %e -> date without '0' in front of one-digit days
		// %H -> hours from 00 to 24
		// %M -> minutes from 00 to 59
	}
	else {
		// Get time struct to string
		strftime(time_string, 30, "%b %e %Y", mtime);

		// %b -> abbreviated month name in format 'mmm'
		// %e -> date without '0' in front of one-digit days
		// %Y -> year with century in format 'yyyy'
	}

	// Print time string
	printf("%s ", time_string);

	// Print file name
	printf("%s", filename);

	// Print a new line at the end
	printf("\n");
}

int get_file_block_count(char* path_to_file) {
	// Stats struct
	struct stat st;

	// Make sure that file exists, error is handled in process_file
	if(stat(path_to_file, &st) == 0) {
		return st.st_blocks / 2;
	}

	// File doesn't exist, so 0 blocks
	return 0;
}

int get_total_block_count(char* directory, struct flags_t* flags) {
	// Block count
	int block_count = 0;

	// Path to file for get_file_block_count
	char* path_to_file = malloc(100 * sizeof(char));

	// Open directory
	DIR *dir = opendir(directory);

	// Check for error, it is handled in process_directory
	if(dir == NULL) {
		return -1;
	}

	// Create entry for each file in directory
	struct dirent *entry;

	// Go through directory
	while((entry = readdir(dir))) {
		// Check if file's block count should be checked
		if(
			(!flags -> a && is_hidden_file(entry -> d_name)) ||		// Hidden file without -A
			is_current_directory(entry -> d_name) ||				// Current directory
			is_parent_directory(entry -> d_name) 					// Parent directory
		) {
			continue;
		}

		// Add directory to path_to_file
		strcpy(path_to_file, directory);

		// Add "/" if needed
		if(path_to_file[strlen(path_to_file) - 1] != '/') {
			strcat(path_to_file, "/");
		}

		// Add file name
		strcat(path_to_file, entry -> d_name);

		// Get file block count
		block_count += get_file_block_count(path_to_file);
	}

	// Close directory
	closedir(dir);

	// Free allocated memory
	free(path_to_file);

	// Return block count
	return block_count;
}

void print_directory_error_message(char* directory) {
	// String for error message
	char* message = malloc(100 * sizeof(char));

	// Add function name
	strcpy(message, "ls: ");

	// Add error description
	strcat(message, "cannot open directory ");

	// Add directory name
	strcat(message, directory);

	// Add "/" if needed
	if(message[strlen(message) - 1] != '/') {
		strcat(message, "/");
	}

	// Print error message to screen
	perror(message);

	// Free allocated memory
	free(message);

	return;
}

void print_file_error_message(char* file) {
	// String for error message
	char* message = malloc(100 * sizeof(char));

	// Add function name
	strcpy(message, "ls: ");

	// Add error description
	strcat(message, "cannot access ");

	// Add directory name
	strcat(message, file);

	// Print error message to screen
	perror(message);

	// Free allocated memory
	free(message);

	return;
}

bool has_subdirectories(char* directory, struct flags_t* flags) {
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

		return false;
	}

	while((entry = readdir(dir))) {
		// Check if file's type should be checked
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

		// Define stats struct
		struct stat st;

		// Get stats for file
		stat(path_to_file, &st);

		// Check if file is directory and return true
		if(get_type_of_file(st.st_mode) == 'd') {
			return true;
		}
	}

	// Close directory
	closedir(dir);

	// Free allocated memory
	free(path_to_file);

	// There are no subdirectories so return false
	return false;
}