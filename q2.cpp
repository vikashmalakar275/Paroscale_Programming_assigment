#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>
#include <set>

std::mutex mtx;
std::set<int> globalUniqueNumbers;

void processFileSection(const std::string& filename, size_t start, size_t end) {
    std::ifstream file(filename);
    if (!file) return;
    file.seekg(start);
    std::set<int> localSet;
    int num;
    while (file.tellg() < end && file >> num) {
        localSet.insert(num);
    }
    std::lock_guard<std::mutex> lock(mtx);
    globalUniqueNumbers.insert(localSet.begin(), localSet.end());
}

void processLargeFile(const std::string& filename, size_t numThreads) {
    std::ifstream file(filename, std::ios::ate);
    size_t fileSize = file.tellg();
    size_t chunkSize = fileSize / numThreads;
    std::vector<std::thread> threads;
    
    for (size_t i = 0; i < numThreads; ++i) {
        size_t start = i * chunkSize;
        size_t end = (i == numThreads - 1) ? fileSize : (i + 1) * chunkSize;
        threads.emplace_back(processFileSection, filename, start, end);
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    for (int num : globalUniqueNumbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
}

int main() {
    processLargeFile("large_numbers.txt", 4);
    return 0;
}
