//
// Created by admin on 22-3-31 031.
//

#ifndef _SUFFIXTRIE_H
#define _SUFFIXTRIE_H
#include <string>
#include <vector>
using namespace std;

class SuffixTrie {
    struct SuffixNode{
        uint64_t l=0;
        uint64_t r=0;
        vector<SuffixNode*> child;
        SuffixNode *SuffixLink= nullptr;
        SuffixNode()=default;
    };
public:
    std::string str;
    SuffixNode root;
    SuffixTrie()=default;
    void rebuild(const std::string &s){
        str=s;
        str.push_back('\0');

    }
    SuffixTrie(const std::string &str){ rebuild(str); }
    /**
     * 2022-03-31 18:07:04 GMT+8
     * @param t 查找的子串
     * @return 子串t对应的开始索引
     */
    uint64_t findSubstring(std::string &t){

    }
    /**
     * 2022-03-31 18:07:06 GMT+8
     * @param t 统计的子串
     * @return 子串t的出现次数
     */
    uint64_t statisticSubstring(string &t){
        uint64_t times=0;

        return times;
    }
    /**
     * 2022-03-31 18:07:25 GMT+8
     * @return 重复出现的最长子串
     */
    string findMostRepeatSubstring(){


    }
    /**
     * 2022-03-31 18:07:44 GMT+8
     * @param q 第一个串
     * @param r 第二个串
     * @return q和r的公共最长子串
     */
    static string findLongestCommon(string &q, string &r){


    }
};


#endif //_SUFFIXTRIE_H
