#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void scan_pages(uint8_t* data, size_t num_pages) {
    size_t zero_pages = 0;
    size_t nonzero_pages = 0;

    sleep(3);

    printf("Scanning pages...\n");

    for (size_t i = 0; i != num_pages; i++) {
        int value = (int) data[i * 4096];
        if (value == 0) {
            zero_pages++;
        } else {
            nonzero_pages++;
        }
    }

    printf("Pages with first byte zero: %zu\n", zero_pages);
    printf("Pages with first byte nonzero: %zu\n", nonzero_pages);
}

int main(int argc, char** argv) {
    if (argc != 2 && argc != 3) {
        printf("Usage: %s num_pages [file_to_mmap]\n", argv[0]);
        return EXIT_FAILURE;
    }

    size_t num_pages = strtoull(argv[1], NULL, 0);
    if (errno != 0) {
        printf("num_pages is invalid\n");
        return EXIT_FAILURE;
    }

    int fd = -1;
    uint8_t* data = NULL;

    if (argc == 3) {
        fd = open(argv[2], O_RDWR);
        if (fd == -1) {
            perror("open");
            return EXIT_FAILURE;
        }

        data = mmap(NULL, 4096 * num_pages, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (data == MAP_FAILED) {
            perror("mmap");
            return EXIT_FAILURE;
        }
    } else {
        data = mmap(NULL, 4096 * num_pages, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (data == MAP_FAILED) {
            perror("mmap");
            return EXIT_FAILURE;
        }
    }

    scan_pages(data, num_pages);

    data[0] = 162;
    printf("data[0] <- 162\n");
    scan_pages(data, num_pages);

    memset(data, 0xa2, num_pages * 4096);
    printf("memset to 0xa2\n");
    scan_pages(data, num_pages);

    return EXIT_SUCCESS;
}