#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <errno.h>

char *slurp_file(const char *file_path) {
	FILE *fp = fopen(file_path, "rb");
	if (fp == NULL) {
		fprintf(stderr, "ERROR Opening file for reading. FILE: %s. ERROR: %s.\n", file_path, strerror(errno));	
		return NULL;
	}

	size_t file_size = 0;
	fseek(fp, 0L, SEEK_END);
	file_size = ftell(fp);
	rewind(fp);

	char *file_mem_buffer = (char*)malloc(file_size + 1);
	if (file_mem_buffer == NULL) {
		fprintf(stderr, "ERROR allocating buffer to read file. FILE: %s. ERROR: %s.\n", file_path, strerror(errno));
		fclose(fp);
		return NULL;
	}
	file_mem_buffer[file_size] = 0;

	if (fread(file_mem_buffer, file_size, 1, fp) != 1) {
		fprintf(stderr, "ERROR reading file. FILE: %s. ERROR: %s.\n", file_path, strerror(errno));
		fclose(fp);
		free(file_mem_buffer);
		return NULL;
	}

	fclose(fp);
	return file_mem_buffer;
}

char *get_puzzle_input(const char *file_path) {
	if (file_path == NULL) {
		return strdup("1abc2\npqr3stu8vwx\na1b2c3d4e5f\ntreb7uchet");
	} else {
		return slurp_file(file_path);
	}
}

int main(int argc, char **argv) {
	char *input_file_path = NULL;
	if (argc == 2) {
		input_file_path = argv[1];
	}

	char *puzzle_input = get_puzzle_input(input_file_path);
	assert(puzzle_input != NULL);

	char *line = strtok(puzzle_input, "\n");
	size_t total = 0;

	while (line) {
		char digit_a[16] = { 0 };
		size_t a = 0;

		char digit_b[16] = { 0 };
		size_t b = 0;

		/* For Multi-Digit numbers, whoops!
		for (size_t i = 0; i < strlen(line); i++) {
			if (isdigit(line[i])) {
				b = 0;
				memset(digit_b, 0, 16);

				int push_a = (a == 0);

				for (size_t j = i; j < strlen(line); j++) {
					if (isdigit(line[j])) {
						if (push_a)
							digit_a[a++] = line[j];

					} else {
						i = j;
						break;
					}
				}
			}
		}
		*/

		for (size_t i = 0; i < strlen(line); i++) {
			if (isdigit(line[i])) {
				if (a == 0) {
					digit_a[0] = line[i];
					a++;
				}

				digit_b[0] = line[i];
				b = 1;
			}
		}

		char digit_merged[16] = { 0 };
		size_t dm = 0;

		for (size_t i = 0; i < a; i++) {
			digit_merged[dm++] = digit_a[i];
		}
		for (size_t i = 0; i < b; i++) {
			digit_merged[dm++] = digit_b[i];
		}
		
		int merged_digit_num = atoi(digit_merged);
		printf("Line: '%s'. Merged Digit Char: %s, Num: %d\n", line, digit_merged, merged_digit_num);
		total += merged_digit_num;

		line = strtok(NULL, "\n");
	}

	printf("Total: %zu\n", total);	
	free(puzzle_input);

	return 0;
}

