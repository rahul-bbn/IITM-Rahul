#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdbool.h>
#include <time.h>
#include "./include/bufferpool.h"
#include "./include/lrucache.h"

// Cached File
typedef struct cached_file {
    char filename[256];
    buffer_t *buffer;
    size_t file_size;
    time_t last_accessed;
    struct cached_file *next;
}cached_file_t;

typedef struct cache_stats {
    int hits;
    int misses;
    int evictions;
    size_t total_bytes_cached;
} cache_stats_t;

typedef struct file_cache {
    buffer_pool_t *buffer_pool;
    cached_file_t *files[CACHE_SIZE];
    int cached_files_count;
    cache_stats_t stats;
}file_cache_t;

//To create file cache
file_cache_t* create_file_cache() {
    file_cache_t *cache = malloc(sizeof(file_cache_t));
    if(!cache) return NULL;

    cache->buffer_pool = create_buffer_pool(POOL_SIZE);
    if(!cache->buffer_pool)
    {
        free(cache);
        return NULL;
    }  
    
    cache->cached_files_count = 0;

    for (int iterator = 0; iterator < CACHE_SIZE; iterator++)
    {
        cache->files[iterator] = NULL;
    }

    return cache;
} 

//To read the cached file
char* read_file_cached(file_cache_t *cache, const char *filename)
{
    unsigned int index = hash(filename);

    cached_file_t *cached = cache->files[index];

    while(cached)
    {
        if(strcmp(cached->filename, filename) == 0)
        {
            cached->last_accessed = time(NULL);
            printf("File cache HIT for %s \n", filename);
            return cached->buffer->data;

        }

        cached = cached->next;
    }

    printf("File cache MISS for %s \n", filename);

    buffer_t *buffer = acquire_buffer(cache->buffer_pool);
    if(!buffer)
    {
        printf("No buffers are available");
        return NULL;
    }

    int fd = open(filename, O_RDONLY);
    if(fd == -1)
    {
        release_buffer(cache->buffer_pool, buffer);
        return NULL;
    }

    size_t bytes_read = read(fd, buffer->data, BUFFER_SIZE - 1);
    close(fd);

    if(bytes_read == -1)
    {
        release_buffer(cache->buffer_pool, buffer);
        return NULL;
    }

    buffer->data[bytes_read] = '\0';


//To add file as a new cache entry
cached_file_t *new_cached = malloc(sizeof(cached_file_t));

if(!new_cached)
{
    release_buffer(cache->buffer_pool, buffer);
    return NULL;
}

strncpy(new_cached->filename, filename, sizeof(new_cached->filename) - 1);
new_cached->filename[sizeof(new_cached->filename) - 1] = '\0';

new_cached->buffer = buffer;
new_cached->file_size = bytes_read;
new_cached->last_accessed = time(NULL);
new_cached->next = cache->files[index];

cache->files[index] = new_cached;
cache->cached_files_count++;

printf("Cached file %s of size %zu bytes \n", filename, bytes_read);

return buffer->data;
}

// Remove a specific file from cache
void remove_cached_file(file_cache_t *cache, const char *filename) {
    unsigned int index = hash(filename);
    cached_file_t *current = cache->files[index];
    cached_file_t *prev = NULL;
    
    while (current) {
        if (strcmp(current->filename, filename) == 0) {
            // Remove from linked list
            if (prev) {
                prev->next = current->next;
            } else {
                cache->files[index] = current->next;
            }
            
            // Release buffer back to pool
            release_buffer(cache->buffer_pool, current->buffer);
            
            // Free the cache entry
            free(current);
            cache->cached_files_count--;
            
            printf("Removed file from cache: %s\n", filename);
            return;
        }
        prev = current;
        current = current->next;
    }
}

// LRU eviction - remove least recently used files
void evict_lru_files(file_cache_t *cache, int max_files) {
    if (cache->cached_files_count <= max_files) return;
    
    // Collect all cached files with their access times
    typedef struct {
        cached_file_t *file;
        time_t last_accessed;
    } file_access_t;
    
    file_access_t *files = malloc(sizeof(file_access_t) * cache->cached_files_count);
    int file_count = 0;
    
    // Gather all files
    for (int i = 0; i < CACHE_SIZE; i++) {
        cached_file_t *current = cache->files[i];
        while (current) {
            files[file_count].file = current;
            files[file_count].last_accessed = current->last_accessed;
            file_count++;
            current = current->next;
        }
    }
    
    // Sort by access time (bubble sort for simplicity)
    for (int i = 0; i < file_count - 1; i++) {
        for (int j = 0; j < file_count - i - 1; j++) {
            if (files[j].last_accessed > files[j + 1].last_accessed) {
                file_access_t temp = files[j];
                files[j] = files[j + 1];
                files[j + 1] = temp;
            }
        }
    }
    
    // Remove oldest files
    int files_to_remove = cache->cached_files_count - max_files;
    for (int i = 0; i < files_to_remove; i++) {
        remove_cached_file(cache, files[i].file->filename);
    }
    
    free(files);
    printf("Evicted %d LRU files\n", files_to_remove);
}


// Write data to a cached file
int write_file_cached(file_cache_t *cache, const char *filename, 
                     const char *data, size_t data_size) {
    if (data_size >= BUFFER_SIZE) {
        printf("Data too large for buffer: %zu bytes\n", data_size);
        return -1;
    }
    
    unsigned int index = hash(filename);
    cached_file_t *cached = cache->files[index];
    
    // Check if file is already cached
    while (cached) {
        if (strcmp(cached->filename, filename) == 0) {
            // Update existing cache entry
            memcpy(cached->buffer->data, data, data_size);
            cached->buffer->data[data_size] = '\0';
            cached->file_size = data_size;
            cached->last_accessed = time(NULL);
            
            printf("Updated cached file: %s\n", filename);
            return 0;
        }
        cached = cached->next;
    }
    
    // File not in cache - create new entry
    buffer_t *buffer = acquire_buffer(cache->buffer_pool);
    if (!buffer) {
        printf("No buffers available for write!\n");
        return -1;
    }
    
    memcpy(buffer->data, data, data_size);
    buffer->data[data_size] = '\0';
    
    cached_file_t *new_cached = malloc(sizeof(cached_file_t));
    if (!new_cached) {
        release_buffer(cache->buffer_pool, buffer);
        return -1;
    }
    
    strncpy(new_cached->filename, filename, sizeof(new_cached->filename) - 1);
    new_cached->filename[sizeof(new_cached->filename) - 1] = '\0';
    new_cached->buffer = buffer;
    new_cached->file_size = data_size;
    new_cached->last_accessed = time(NULL);
    new_cached->next = cache->files[index];
    
    cache->files[index] = new_cached;
    cache->cached_files_count++;
    
    printf("Cached new file for write: %s (%zu bytes)\n", filename, data_size);
    return 0;
}

// Flush cached data to disk
int flush_cached_file(file_cache_t *cache, const char *filename) {
    unsigned int index = hash(filename);
    cached_file_t *cached = cache->files[index];
    
    while (cached) {
        if (strcmp(cached->filename, filename) == 0) {
            // Write to disk
            int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd == -1) {
                perror("Failed to open file for writing");
                return -1;
            }
            
            ssize_t bytes_written = write(fd, cached->buffer->data, cached->file_size);
            close(fd);
            
            if (bytes_written != (ssize_t)cached->file_size) {
                printf("Partial write: %zd/%zu bytes\n", bytes_written, cached->file_size);
                return -1;
            }
            
            printf("Flushed cached file to disk: %s (%zu bytes)\n", 
                   filename, cached->file_size);
            return 0;
        }
        cached = cached->next;
    }
    
    printf("File not found in cache: %s\n", filename);
    return -1;
}

// Get cache statistics
void print_cache_stats(file_cache_t *cache) {
    printf("\n=== File Cache Statistics ===\n");
    printf("Cached files: %d\n", cache->cached_files_count);
    printf("Cache hits: %d\n", cache->stats.hits);
    printf("Cache misses: %d\n", cache->stats.misses);
    printf("Total evictions: %d\n", cache->stats.evictions);
    
    if (cache->stats.hits + cache->stats.misses > 0) {
        double hit_rate = (double)cache->stats.hits / 
                         (cache->stats.hits + cache->stats.misses) * 100.0;
        printf("Hit rate: %.2f%%\n", hit_rate);
    }
    
    printf("Available buffers: %d/%d\n", 
           cache->buffer_pool->free_count, cache->buffer_pool->total_buffers);
    printf("===============================\n\n");
}

// List all cached files
void list_cached_files(file_cache_t *cache) {
    printf("\n=== Cached Files ===\n");
    time_t now = time(NULL);
    
    for (int i = 0; i < CACHE_SIZE; i++) {
        cached_file_t *current = cache->files[i];
        while (current) {
            double age = difftime(now, current->last_accessed);
            printf("File: %-30s Size: %6zu bytes Age: %.0f seconds\n",
                   current->filename, current->file_size, age);
            current = current->next;
        }
    }
    printf("===================\n\n");
}

// Clean up old cache entries
void cleanup_old_entries(file_cache_t *cache, int max_age_seconds) {
    time_t now = time(NULL);
    
    for (int i = 0; i < CACHE_SIZE; i++) {
        cached_file_t *current = cache->files[i];
        cached_file_t *prev = NULL;
        
        while (current) {
            if (difftime(now, current->last_accessed) > max_age_seconds) {
                cached_file_t *to_remove = current;
                
                // Update linked list
                if (prev) {
                    prev->next = current->next;
                } else {
                    cache->files[i] = current->next;
                }
                
                current = current->next;
                
                // Clean up
                release_buffer(cache->buffer_pool, to_remove->buffer);
                printf("Aged out file: %s\n", to_remove->filename);
                free(to_remove);
                cache->cached_files_count--;
                cache->stats.evictions++;
            } else {
                prev = current;
                current = current->next;
            }
        }
    }
}

// Prefetch files into cache
void prefetch_files(file_cache_t *cache, const char **filenames, int count) {
    for (int i = 0; i < count; i++) {
        // Check if already cached
        unsigned int index = hash(filenames[i]);
        cached_file_t *cached = cache->files[index];
        bool already_cached = false;
        
        while (cached) {
            if (strcmp(cached->filename, filenames[i]) == 0) {
                already_cached = true;
                break;
            }
            cached = cached->next;
        }
        
        if (!already_cached) {
            printf("Prefetching: %s\n", filenames[i]);
            read_file_cached(cache, filenames[i]);
        }
    }
}

// Flush all cached files to disk
void flush_all_cached_files(file_cache_t *cache) {
    printf("Flushing all cached files to disk...\n");
    
    for (int i = 0; i < CACHE_SIZE; i++) {
        cached_file_t *current = cache->files[i];
        while (current) {
            flush_cached_file(cache, current->filename);
            current = current->next;
        }
    }
}

// Destroy the entire file cache
void destroy_file_cache(file_cache_t *cache) {
    if (!cache) return;
    
    // Optional: flush all files before destroying
    flush_all_cached_files(cache);
    
    // Free all cached file entries
    for (int i = 0; i < CACHE_SIZE; i++) {
        cached_file_t *current = cache->files[i];
        while (current) {
            cached_file_t *next = current->next;
            release_buffer(cache->buffer_pool, current->buffer);
            free(current);
            current = next;
        }
    }
    
    // Destroy buffer pool
    destroy_buffer_pool(cache->buffer_pool);
    
    // Free cache structure
    free(cache);
    
    printf("File cache destroyed\n");
}


int main() {
    // Create file cache
    file_cache_t *cache = create_file_cache();
    if (!cache) {
        printf("Failed to create file cache\n");
        return 1;
    }
    
    // Read some files
    char *content1 = read_file_cached(cache, "test1.txt");
    char *content2 = read_file_cached(cache, "test2.txt");
    char *content1_again = read_file_cached(cache, "test1.txt");  // Should be cache hit
    
    // Write a new file
    write_file_cached(cache, "output.txt", "Hello, cached world!", 21);
    
    // Show cache stats
    print_cache_stats(cache);
    list_cached_files(cache);
    
    // Simulate cache management
    evict_lru_files(cache, 2);  // Keep only 2 most recent files
    cleanup_old_entries(cache, 300);  // Remove files older than 5 minutes
    
    // Final cleanup
    destroy_file_cache(cache);
    
    return 0;
}



