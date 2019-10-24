# topurl

Get top-K URLs from a big URL file

## Problem

The existing 100GB URL file uses 1GB of available memory to filter out all the top 100 URLs and their occurrences.

## Solve

1. Read the original input file and partition the file into multiple small files.
2. Map each small file from the url to hash maps
3. Perform a reduce operation on the maps after mapping a file
4. Use min-heap to keep the most frequently appearing 100 URLs

Increase efficiency with multi-threaded parallel processing

## Make and Run

```bash
cmake CMakeList.txt -B build
build/topurl -f URL_FILE_PATH > top_urls.txt
```
