/*
page_id             uint32_t page_id           : [0..3]
record_count        uint32_t record_count      : [4..7]
payload_bytes       payload bytes              : [8..P-1] , P = page size
*/

#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<errno.h>
#include<inttypes.h>
#include<stdbool.h>
#include<stdint.h>
#include<string.h>
#include<sys/mman.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>

typedef struct {
    size_t os_page_size;
    size_t file_pages;
    size_t file_size;
}MmapConfig;

static void fatal(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

static size_t get_os_page_size(void)
{
    long page_size = sysconf(_SC_PAGE_SIZE);
    if(page_size < 0)
    {
        return 4096;
    }

    return (size_t)page_size;
}

//helper function to read 32-bit data at the pagebase
static inline uint32_t page_read_u32(const uint8_t *page_base, size_t offset)
{
    uint32_t data;
    memcpy(&data, page_base + offset, sizeof(data));
    return data;
}

//helper function to write 32-bit data at the pagebase
static inline void page_write_u32(uint8_t *page_base, size_t offset, uint32_t data)
{
    memcpy(page_base + offset, &data, sizeof(data));
}

static void init_page(uint8_t *page_base, size_t page_size, uint32_t page_id, uint32_t record_count)
{
    page_write_u32(page_base, 0, page_id);
    page_write_u32(page_base, 4, record_count);

    size_t payload_length = page_size - 8;
    memset(page_base + 8, 0, payload_length);

    char marker[128];
    int n = snprintf(marker, sizeof(marker), "Page %u : %u records", page_id, record_count);

    if(n < 0)
    {
        n = 0;
    }

    size_t to_copy = (size_t)n < payload_length ? (size_t)n : payload_length;
    memcpy(page_base + 8, marker, to_copy);
}

static void print_page_header(const uint8_t *page_base)
{
    uint32_t pid = page_read_u32(page_base, 0);
    uint32_t rc = page_read_u32(page_base, 4);

    printf(" page_id = %u, record_count = %u \n", pid, rc);
}

//map_shared
static uint8_t *map_file_shared(int fd, size_t length)
{
    void *address = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if(address == MAP_FAILED)
    {
        fatal("mmap(MAP_SHARED)");
    }

    return (uint8_t*)address;
}

//map_private (copy-on-write)
static uint8_t *map_file_private(int fd, size_t length)
{
    void *address = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);

    if(address == MAP_FAILED)
    {
        fatal("mmap(MAP_PRIVATE)");
    }

    return (uint8_t*)address;
}

int main(int argc, char **argv)
{
    const char *path = (argc >= 2) ? argv[1] : "mmap_demo.dat";

    MmapConfig cfg = {0};
    cfg.os_page_size = get_os_page_size();
    cfg.file_pages = 3;
    cfg.file_size = cfg.file_pages * cfg.os_page_size;

    printf("\nOS Page Size : %zu bytes \n", cfg.os_page_size);
    printf("Creating/truncating : %s (%zu bytes) \n", path, cfg.file_size);

    int fd = open(path, O_CREAT | O_TRUNC | O_RDWR, 0644);

    if (fd < 0)
    {
        fatal("open");
    }
    
    if(ftruncate(fd, (off_t)cfg.file_size) != 0)
    {
        fatal("ftruncate");
    }

    uint8_t *base = map_file_shared(fd, cfg.file_size);

    for (size_t iterator = 0; iterator < cfg.file_pages; ++iterator)
    {
        uint8_t *page = base + iterator * cfg.os_page_size;
        init_page(page, cfg.os_page_size, (uint32_t)iterator, (uint32_t)(iterator * 10 + 1));
    }

    if(msync(base, cfg.file_size, MS_SYNC) != 0)
    {
        fatal("msync(all)");
    }

    if(fsync(fd) != 0)
    {
        fatal("fsync");
    }

    printf("\n(After initialization) \nHeaders on the mapped memory : \n");
    for (size_t iterator = 0; iterator < cfg.file_pages; ++iterator)
    {
        uint8_t *page = base + iterator * cfg.os_page_size;
        printf(" page[%zu] :", iterator);
        print_page_header(page);
    }

    size_t target_index = 1;
    uint8_t *target_page = base + target_index * cfg.os_page_size;
    uint32_t old_rc = page_read_u32(target_page, 4);
    page_write_u32(target_page, 4, old_rc + 100);
    target_page[8] = '*';

    if(msync(target_page, cfg.os_page_size, MS_SYNC) != 0)
    {
        fatal("msycc(page)");
    }

    if(fsync(fd) != 0)
    {
        fatal("fsync(after page change)");
    }

    printf("\n(After memory modification + msync) \nUpdated headers :\n page[%zu] : ", target_index);
    print_page_header(target_page);

    uint8_t *priv = map_file_private(fd, cfg.file_size);
    uint8_t *page2 = priv + 2 * cfg.os_page_size;
    uint32_t rc2 = page_read_u32(page2, 4);
    page_write_u32(page2, 4, rc2 + 777);
    (void)msync(page2, cfg.os_page_size, MS_SYNC);

    if(munmap(priv, cfg.file_size) != 0)
    {
        fatal("munmap(private)");
    }

    if(munmap(base, cfg.file_size) != 0)
    {
        fatal("munmap(shared)");
    }

    if(close(fd) != 0)
    {
        fatal("close");
    }

    fd = open(path, O_RDONLY);

    if(fd < 0)
    {
        fatal("open(verfiy)");
    }

    printf("\n(For persistence verification) \nHeaders on the file : \n");

    for (size_t iterator = 0; iterator < cfg.file_pages; ++iterator)
    {
        uint8_t header[8];
        off_t off = (off_t)(iterator * cfg.os_page_size);
        ssize_t read_data = pread(fd, header, sizeof(header), off);

        if (read_data != (ssize_t)sizeof(header))
        {
            fatal("pread(header)");
        }

        uint32_t pid, rc;
        memcpy(&pid, header + 0, 4);
        memcpy(&rc, header + 4, 4);
        printf(" page[%zu] : page_id = %u, record_count = %u \n", iterator, pid, rc);
    }

    if(close(fd) != 0)
    {
        fatal("close(verify)");
    }

    printf("\n PS: \n");
    printf("* The changes on page[1] via MAP_SHARED are persistent. \n");
    printf("* The changes on page[1] via MAP_PRIVATE are not persistent.\n");

    return 0;
}