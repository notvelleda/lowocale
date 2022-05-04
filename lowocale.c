#include <stdio.h>
#include <stdint.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdbool.h>

uint8_t *buf;

// MO header structure
typedef struct _MOHeader {
	uint32_t magic;
	uint32_t revision;
	uint32_t numStrings;
	uint32_t origTableOffset;
	uint32_t transTableOffset;
	uint32_t hashTableSize;
	uint32_t hashTableOffset;
} MOHeader;

// MO magic number
#define MO_MAGIC 0x950412de

// string entry in MO file
typedef struct _StringEntry {
	uint32_t length;
	char *ptr;
} StringEntry;

// get string entry from MO file
StringEntry getString(uint32_t offset, uint32_t index) {
	uint32_t offset2 = *((uint32_t *) (buf + offset + index * 8 + 4));
	StringEntry str = {
		*((uint32_t *) (buf + offset + index * 8)), (char *) (buf + offset2)
	};
	return str;
}

// uwuify string. yes i know this code sucks lmao
void uwuify(char *str) {
	while (*(str ++) != 0) {
		switch (*str) {
			case 'l':
			case 'r':
				*str = 'w';
				break;
			case 'L':
			case 'R':
				*str = 'W';
				break;
			case '%':
				// ignore formatting specifiers
				while (*(str ++) != 0) {
					switch (*str) {
						case '%':
						case 'c':
						case 'd':
						case 'e':
						case 'E':
						case 'f':
						case 'g':
						case 'G':
						case 'i':
						case 'n':
						case 'o':
						case 'p':
						case 's':
						case 'u':
						case 'x':
						case 'X':
							goto break_out;
						// special case- l can be standalone or have things following
						case 'l':
							switch (*(str + 1)) {
								case 'd':
								case 'i':
								case 'l':
								case 'u':
									goto break_out;
							}
					}
				}
				// shush this is a perfectly valid use of goto
				break_out:
				break;
		}
	}
}

int main(int argc, char **argv) {
	if (argc != 3) {
		fprintf(stderr, "usage: %s <input> <output>\n", argv[0]);
		return 1;
	}

	char *filename = argv[1];
	char *outputFilename = argv[2];
	struct stat info;
	FILE *f;
	
	printf("%s -> %s\n", filename, outputFilename);

	// get info about file
	if (stat(filename, &info) != 0) {
		fprintf(stderr, "error stat()ing file\n");
		return 1;
	}

	// open file
	f = fopen(filename, "rb");

	if (!f) {
		fprintf(stderr, "error opening file\n");
		return 1;
	}

	// allocate memory to store file contents
	buf = malloc(info.st_size);

	// read then close file
	fread(buf, info.st_size, 1, f);
	fclose(f);

	// get the file header- we can just use a pointer to the start of the buffer as that's where the header is
	MOHeader *header = (MOHeader *) buf;

	// check if magic number is correct
	if (header->magic != MO_MAGIC) {
		fprintf(stderr, "wrong magic number\n");
		return 1;
	}

	// check if revision is correct
	if (header->revision != 0 && header->revision != 1) {
		fprintf(stderr, "wrong revision! we need revision 0 or 1\n");
		return 1;
	}

	printf("%d strings\n", header->numStrings);

	// iterate over all strings
	for (uint32_t i = 0; i < header->numStrings; i ++) {
		// get string entries
		StringEntry str = getString(header->origTableOffset, i);
		StringEntry strTrans = getString(header->transTableOffset, i);

		// uwuify translated string
		uwuify(strTrans.ptr);
		
		printf("%d: (len %d) \"%s\", translated \"%s\"\n", i, str.length, str.ptr, strTrans.ptr);
	}

	// write uwuified MO file
	f = fopen(outputFilename, "wb");

	if (!f) {
		fprintf(stderr, "error opening file\n");
		return 1;
	}

	fwrite(buf, info.st_size, 1, f);
	fclose(f);
	
	return 0;
}
