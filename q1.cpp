#include <iostream>
#include <unordered_map>
#include <list>
#include <ctime>

// LRU Cache Implementation
class LRUCache {
private:
    // Struct to store file information
    struct FileInfo {
        std::string filePath; // File path
        time_t timestamp;     // Time when the file was opened
        size_t size;          // File size
        ino_t inode;          // Inode number
    };

    size_t capacity; // Maximum capacity of the cache
    std::list<FileInfo> cacheList; // List to maintain the order of file accesses (most recent at front)
    std::unordered_map<std::string, std::list<FileInfo>::iterator> cacheMap; // Hash map for quick lookup
    
public:
    // Constructor to initialize the cache with a given capacity
    LRUCache(size_t cap) : capacity(cap) {}

    // Function to add an entry to the cache
    void addEntry(const std::string& filePath, time_t timestamp, size_t size, ino_t inode) {
        // If the file is already in cache, remove it from the list
        if (cacheMap.find(filePath) != cacheMap.end()) {
            cacheList.erase(cacheMap[filePath]);
        }
        // Add the new entry at the front (most recently used)
        cacheList.push_front({filePath, timestamp, size, inode});
        cacheMap[filePath] = cacheList.begin();
        
        // If the cache exceeds its capacity, remove the least recently used item
        if (cacheList.size() > capacity) {
            cacheMap.erase(cacheList.back().filePath);
            cacheList.pop_back(); // Remove oldest entry (least recently used)
        }
    }
    
    // Function to search for an entry in the cache
    bool searchEntry(const std::string& filePath) {
        return cacheMap.find(filePath) != cacheMap.end(); // Returns true if file exists in cache
    }
    
    // Function to remove the oldest (least recently used) entry from the cache
    void removeOldest() {
        if (!cacheList.empty()) {
            cacheMap.erase(cacheList.back().filePath);
            cacheList.pop_back();
        }
    }
};

int main() {
    // Create an LRU cache with a capacity of 5 entries
    LRUCache cache(5);
    
    // Add entries to the cache
    cache.addEntry("../file1", time(nullptr), 1024, 12345);
    cache.addEntry("../file2", time(nullptr), 2048, 12346);
    
    // Check if file1 is present in the cache
    std::cout << "File1 in cache: " << cache.searchEntry("../file1") << std::endl;
    
    // Remove the oldest entry (Least Recently Used)
    cache.removeOldest();
    
    // Check if file1 is still in cache after removal
    std::cout << "File1 in cache after removal: " << cache.searchEntry("../file1") << std::endl;
    
    return 0;
}