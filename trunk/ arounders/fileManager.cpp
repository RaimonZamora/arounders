#include "fileManager.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "fcntl.h"
#ifdef _WIN32
#include "io.h"
#endif

char *resourceFileName = "data.jrf";

void setResourceFile(char *p_resourceFileName) {
	resourceFileName = p_resourceFileName;
}

char *GetBufferFromResource(char *resourcename, int *filesize) 
{
	//Try to open the resource file in question
	int fd = open(resourceFileName, O_RDONLY);
	if (fd < 0) 
	{
		perror("Error opening resource file");
		exit(1);
	}
	
	//Make sure we're at the beginning of the file
	lseek(fd, 0, SEEK_SET);
	
	//Read the first INT, which will tell us how many files are in this resource
	int numfiles;
	int resultat = read(fd, &numfiles, sizeof(int));
	
	int final = eof(fd);

	//Get the pointers to the stored files
	int *filestart = (int *) malloc(sizeof(int) * numfiles);	// this is probably wrong in the zip
	resultat = read(fd, filestart, sizeof(int) * numfiles);
	
	//Loop through the files, looking for the file in question
	int filenamesize;
	char *buffer;
	int i;
	for(i=0;i<numfiles;i++) 
	{
		int result = 129;
		char *filename;
		//Seek to the location
		lseek(fd, filestart[i], SEEK_SET);
		//Get the filesize value
		read(fd, filesize, sizeof(int));
		//Get the size of the filename string
		read(fd, &filenamesize, sizeof(int));
		//Size the buffer and read the filename
		filename = (char *) malloc(filenamesize + 1);
		result = read(fd, filename, filenamesize);
		//Remember to terminate the string properly!
		filename[filenamesize] = '\0';
		//Compare to the string we're looking for
		if (strcmp(filename, resourcename) == 0) 
		{
			//Get the contents of the file
			buffer = (char *) malloc(*filesize);
			read(fd, buffer, *filesize);
			free(filename);
			break;
		}
		//Free the filename buffer
		free(filename);
	}
	
	//Release memory
	free(filestart);
	
	//Close the resource file!
	close(fd);
	
	//Did we find the file within the resource that we were looking for?
	if (buffer == NULL) 
	{
		printf("Unable to find '%s' in the resource file!\n", resourcename);
		exit(1);
	}
	
	//Return the buffer
	return buffer;
}
