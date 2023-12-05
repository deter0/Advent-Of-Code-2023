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
		return strdup("two1nine\neightwothree\nabcone2threexyz\nxtwone3four\n4nineeightseven2\nzoneight234\n7pqrstsixteen");
	} else {
		return slurp_file(file_path);
	}
}

const char *spelt_numbers[] = {
	"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"
};
const size_t spelt_numbers_count = sizeof(spelt_numbers)/sizeof(spelt_numbers[0]);

struct spelt_search_result {
	int value;
	int index;
};

struct spelt_search_result search_spelt_numbers(const char *string, int backwards) {
	struct spelt_search_result results = { 0 };

	for (int i = backwards ? strlen(string) : 0; backwards ? i >= 0 : i < strlen(string); backwards ? i-- : i++) { 
		for (size_t j = 0; j < spelt_numbers_count; j++) {
			const char *spelt_number = spelt_numbers[j];
			int broken = 0;
			for (size_t k = 0; k < strlen(spelt_number); k++) {
				if ((i + k) > strlen(string)) {
					broken = 1;
					break;
				};
				if (spelt_number[k] != string[i + k]) {
					broken = 1;
					break;
				}	
			}
			if (!broken) {
				printf("\tFound Spelt Number: %s. Line: '%s'. \n", spelt_number, string);

				results.index = i;
				results.value = j + 1;
				return results;
			}
		}
	}
	
	return results;
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
		char digit_a;
		int a_set = 0;
		int ai = 1e8;

		char digit_b;
		int bi = -1e8;

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
				if (a_set == 0) {
					digit_a = line[i];
					ai = i;
					a_set ++;
				}

				digit_b = line[i];
				bi = i;
			}
		}

		struct spelt_search_result forwards = search_spelt_numbers(line, 0);
		if (forwards.value > 0 && forwards.index < ai) {
			// Since we are only dealing with one digit numbers
			digit_a = '0' + forwards.value;
		}
		struct spelt_search_result backwards = search_spelt_numbers(line, 1);
		if (backwards.value > 0 && backwards.index > bi) {
			digit_b = '0' + backwards.value;
		}
		printf("\tlast_num_index: %d. spet_index: %d. spelt_index > bi: %d\n", bi, backwards.index, backwards.index > bi);

		char digit_merged[16] = { 0 };
		
		digit_merged[0] = digit_a;
		digit_merged[1] = digit_b;
	
		int merged_digit_num = atoi(digit_merged);
		printf("Line: '%s'. Merged Digit Char: %s, Num: %d\n\n", line, digit_merged, merged_digit_num);
		total += merged_digit_num;

		line = strtok(NULL, "\n");
	}

	printf("Total: %zu\n", total);	
	free(puzzle_input);

	return 0;
}

