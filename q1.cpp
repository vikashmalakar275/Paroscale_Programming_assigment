/*
1.  Write a library which maintains a Least Recently Used (LRU) cache for storing 
information about files being opened by the applications. Information stored in the 
cache could be : File path, timestamp at which it was opened, metadata information 
of the file e.g size, timestamp, inode(Information returned by stat() system call) 
The LRU cache should have a mechanism to remove the entries which have been 
stored in the cache for a really long time.  
Implement library calls to create LRU, add entry to LRU, search entry in LRU with 
filepath as key, remove items from LRU 
*/

#include <iostream>
#include <unordered_map>
#include <list>
#include <ctime>

// LRU Cache Implementation
class LRUCache {
private:
    struct FileInfo {
        std::string filePath;
        time_t timestamp;
        size_t size;
        ino_t inode;
    };

    size_t capacity;
    std::list<FileInfo> cacheList;
    std::unordered_map<std::string, std::list<FileInfo>::iterator> cacheMap;
    
public:
    LRUCache(size_t cap) : capacity(cap) {}

    void addEntry(const std::string& filePath, time_t timestamp, size_t size, ino_t inode) {
        if (cacheMap.find(filePath) != cacheMap.end()) {
            cacheList.erase(cacheMap[filePath]);
        }
        cacheList.push_front({filePath, timestamp, size, inode});
        cacheMap[filePath] = cacheList.begin();
        if (cacheList.size() > capacity) {
            cacheMap.erase(cacheList.back().filePath);
            cacheList.pop_back();
        }
    }
    
    bool searchEntry(const std::string& filePath) {
        return cacheMap.find(filePath) != cacheMap.end();
    }
    
    void removeOldest() {
        if (!cacheList.empty()) {
            cacheMap.erase(cacheList.back().filePath);
            cacheList.pop_back();
        }
    }
};

int main() {
    LRUCache cache(5);
    cache.addEntry("../file1", time(nullptr), 1024, 12345);
    cache.addEntry("../file2", time(nullptr), 2048, 12346);
    
    std::cout << "File1 in cache: " << cache.searchEntry("../file1") << std::endl;
    cache.removeOldest();
    
    std::cout << "File1 in cache after removal: " << cache.searchEntry("../file1") << std::endl;
    return 0;
}