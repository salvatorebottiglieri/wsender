#include "file.h"
#include "sep_log.h"
#include "sep_string.h"

#include <fcntl.h>
#include <unistd.h>
#include <string.h>


/**
 * Returns the index of the file extension in the given path.
 * If the path does not contain an extension, -1 is returned.
 * @param path The path to get the file extension from
 * @return The index of the file extension, or -1 if not found
 */
ssize_t get_file_extension_from(const char *path){
	ssize_t index = 0;

	while (path[index] != '\0'){
		if (path[index] == '.'){
			return index+1;
		}
		index++;
	}
	return -1;
}


/**
 * Opens a file at the given path and returns a new SFile object.
 * The returned SFile object contains the file descriptor, path, and file extension.
 * If any of the operations fail, an error message is printed and NULL is returned.
 * @param path The path to the file to open
 * @return A new SFile object containing information about the file, or NULL if the operation failed
 */
SFile* s_open(const char *path){
	SFile* file = malloc(sizeof(SFile));
	if (file == NULL){s_log(ERROR,"malloc  failed");return NULL;}

	file->file_descriptor = open(path, O_RDONLY);
	if (file->file_descriptor == -1){s_log(ERROR,"open  failed");free(file);return NULL;}


	file->path = new_s(path, strlen(path));
	if (file->path == NULL){s_log(ERROR,"malloc  failed"); close(file->file_descriptor);free(file);return NULL;}
        

	file->is_open = true;

	ssize_t index = get_file_extension_from(path);
	if (index == -1){s_log(ERROR,"get_file_extension_from  failed");close(file->file_descriptor);delete_s(file->path);free(file);return NULL;}

	file->extension = get_slice(file->path, index, strlen(path+index));

	return file;
}