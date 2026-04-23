#ifndef BPT_MEMORYRIVER_HPP
#define BPT_MEMORYRIVER_HPP

#include <fstream>

using std::string;
using std::fstream;
using std::ifstream;
using std::ofstream;

template<class T, int info_len = 2>
class MemoryRiver {
private:
    fstream file;
    string file_name;
    int sizeofT = sizeof(T);
public:
    MemoryRiver() = default;

    MemoryRiver(const string& file_name) : file_name(file_name) {}

    void initialise(string FN = "") {
        if (FN != "") file_name = FN;
        file.open(file_name, std::ios::out);
        int tmp = 0;
        for (int i = 0; i < info_len; ++i)
            file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.close();
    }

    // 读出第n个int的值赋给tmp，1_base
    void get_info(int &tmp, int n) {
        if (n > info_len) return;
        file.open(file_name, std::ios::in);
        file.seekg((n - 1) * sizeof(int));
        file.read(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.close();
    }

    // 将tmp写入第n个int的位置，1_base
    void write_info(int tmp, int n) {
        if (n > info_len) return;
        file.open(file_name, std::ios::in | std::ios::out);
        file.seekp((n - 1) * sizeof(int));
        file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.close();
    }
    
    // 在文件合适位置写入类对象t，并返回写入的位置索引index
    int write(T &t) {
        // Check if there's a free block available (stored in the first info slot)
        int free_head = 0;
        get_info(free_head, 1);
        
        int index;
        if (free_head != 0) {
            // Reuse a free block
            index = free_head;
            
            // Read the next free block pointer
            int next_free = 0;
            file.open(file_name, std::ios::in);
            file.seekg(index);
            file.read(reinterpret_cast<char *>(&next_free), sizeof(int));
            file.close();
            
            // Update the free list head
            write_info(next_free, 1);
            
            // Write the object at this position
            file.open(file_name, std::ios::in | std::ios::out);
            file.seekp(index);
            file.write(reinterpret_cast<char *>(&t), sizeofT);
            file.close();
        } else {
            // No free blocks, append to end of file
            file.open(file_name, std::ios::in | std::ios::out);
            file.seekp(0, std::ios::end);
            index = file.tellp();
            file.write(reinterpret_cast<char *>(&t), sizeofT);
            file.close();
        }
        
        return index;
    }
    
    // 用t的值更新位置索引index对应的对象
    void update(T &t, const int index) {
        file.open(file_name, std::ios::in | std::ios::out);
        file.seekp(index);
        file.write(reinterpret_cast<char *>(&t), sizeofT);
        file.close();
    }
    
    // 读出位置索引index对应的T对象的值并赋值给t
    void read(T &t, const int index) {
        file.open(file_name, std::ios::in);
        file.seekg(index);
        file.read(reinterpret_cast<char *>(&t), sizeofT);
        file.close();
    }
    
    // 删除位置索引index对应的对象(涉及空间回收)
    void Delete(int index) {
        // Get current free list head
        int free_head = 0;
        get_info(free_head, 1);
        
        // Write the old free_head as the next pointer at the deleted position
        file.open(file_name, std::ios::in | std::ios::out);
        file.seekp(index);
        file.write(reinterpret_cast<char *>(&free_head), sizeof(int));
        file.close();
        
        // Update the free list head to point to this deleted block
        write_info(index, 1);
    }
};


#endif //BPT_MEMORYRIVER_HPP
