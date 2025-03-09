/*
2. Write a multi-threaded application to process a really large file which has integer 
numbers stored in it.  Each thread should work on a separate section of the file and 
should find out unique numbers from that section and add it to the global unique 
number list. Each thread keeps adding the unique numbers from its section to the 
global list. Make sure the unique number should not repeat in the global list. 
At the end of processing the whole file, print the list of unique numbers from the 
global list. 
*/
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>
#include <set>

std::mutex mtx; // Mutex to protect shared resource
std::set<int> globalUniqueNumbers; // Shared set for storing unique numbers

// Function to process a section of the file
void processFileSection(const std::string& filename, size_t start, size_t end) {
    std::ifstream file(filename);
    if (!file) return;

    file.seekg(start);

    // Ensure we are at the beginning of a number
    if (start != 0) {
        char c;
        while (file.get(c) && isdigit(c)) {} // Skip partial number
    }

    std::set<int> localSet;
    int num;
    
    while (file.tellg() != -1 && file.tellg() < end && file >> num) {
        localSet.insert(num);
    }

    // Merge local unique numbers into the global set with thread safety
    std::lock_guard<std::mutex> lock(mtx);
    globalUniqueNumbers.insert(localSet.begin(), localSet.end());
}

// Function to process a large file using multiple threads
void processLargeFile(const std::string& filename, size_t numThreads) {
    std::ifstream file(filename, std::ios::ate); // Open file and move to end
    if (!file) {
        std::cerr << "Error opening file.\n";
        return;
    }
    
    size_t fileSize = file.tellg(); // Get file size
    size_t chunkSize = fileSize / numThreads;
    std::vector<std::thread> threads;

    // Create threads for parallel processing
    for (size_t i = 0; i < numThreads; ++i) {
        size_t start = i * chunkSize;
        size_t end = (i == numThreads - 1) ? fileSize : (i + 1) * chunkSize;
        threads.emplace_back(processFileSection, filename, start, end);
    }

    // Join threads
    for (auto& t : threads) {
        t.join();
    }

    // Output unique numbers
    for (int num : globalUniqueNumbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
}

int main() {
    processLargeFile("large_numbers.txt", 4);
    return 0;
}
