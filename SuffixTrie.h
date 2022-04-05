//
// Created by admin on 22-3-31 031.
//

#ifndef _SUFFIXTRIE_H
#define _SUFFIXTRIE_H
#include <string>
#include <set>
#include <queue>
#include <iostream>
using namespace std;

class SuffixTrie {
    struct SuffixNode{
        int64_t l=0;
        int64_t r=-1;   //-1表示右端点未定
        set<SuffixNode *> child;    //使用set存子节点，crud时更高效
        SuffixNode *suffixLink= nullptr;
        SuffixNode()=default;
        SuffixNode(int64_t l):l(l){}
        SuffixNode(int64_t l,int64_t r):l(l),r(r){}
    };
    bool existPrefixContainSuffix(int64_t right){
        SuffixNode *it= nullptr;
        if(activeEdge== nullptr) {
            for (const auto &item : activeNode->child)
                if (str[item->l] == str[right]) {
                    activeEdge = it = item;
                    break;
                }
            if(it== nullptr)    return 0;
        }
        if(str[activeEdge->l+activeLength]==str[activeEdge->r==-1?right:activeEdge->r]){
            ++activeLength;
            if(activeEdge->l+activeLength>(activeEdge->r==-1?right:activeEdge->r)){
                activeLength=0;
                activeNode=activeEdge;
                activeEdge= nullptr;
            }
            return 1;
        }else return 0;
    }
    void changeActive(int64_t left, int64_t right){
        //rule 1:
        if(activeNode==root){
            if(activeLength>0)  --activeLength;
            activeEdge= nullptr;
            if(activeLength)
                for (const auto &item : root->child)
                    if(str[item->l]==str[left]) {
                        activeEdge=item;
                        break;
                    }
            return ;
        }
        //rule 3:
        if(activeNode->suffixLink!= nullptr){
            activeNode=activeNode->suffixLink;
            for (const auto &item : activeNode->child)
                if (str[item->l] == str[right]) {
                    activeEdge = item;
                    break;
                }
            return ;
        }else{
            activeNode=root;
            activeLength=0;
            activeEdge= nullptr;
            for (const auto &item : activeNode->child)
                if (str[item->l] == str[right]) {
                    activeEdge = item;
                    break;
                }
            while (activeEdge!= nullptr and str[activeEdge->l+activeLength]==str[activeEdge->r==-1?right:activeEdge->r]){
                ++activeLength;
                if(activeEdge->l+activeLength>(activeEdge->r==-1?right:activeEdge->r)){
                    activeLength=0;
                    activeNode=activeEdge;
                    activeEdge= nullptr;
                }
            }
        }
    }
public:
    std::string str;
    SuffixNode *root= nullptr;
    int64_t remainder=0;
    SuffixNode *activeNode=root;
    SuffixNode *activeEdge= nullptr;
    int64_t activeLength=0;
    SuffixTrie(const std::string &str){ rebuild(str); }
    ~SuffixTrie(){}
    /**
     * 2022-04-03 11:59:22 GMT+8
     * @param s 要构建后缀树的字符串
     * @param last 结尾字符, 在对两个不同字符串查询公共部分时, 用来区分不同的字符串
     */
    void rebuild(const std::string &s, const char last='\0'){
        str=s;
        str.push_back(last);
        root=new SuffixNode(-1);//标致为根节点，不存放任何字符
        SuffixNode *previous= nullptr;
        int64_t left=0,right=0;
        remainder=activeLength=0;
        activeNode=root;
        activeEdge= nullptr;  //非空情况表示要对活动节点的哪个子节点进行分裂
        while (left<(int64_t)str.size()){
            ++remainder;
            if(!existPrefixContainSuffix(right)){
                previous= nullptr;
                while (remainder>0){
                    cout<<"str: "<<str.substr(left, right-left+1)<<'\n';
                    cout<<"activenode: "<<activeNode->l<<' '<<activeNode->r<<'\n';
                    if(activeEdge!= nullptr)
                        cout<<"activeedge: "<<activeEdge->l<<' '<<activeEdge->r<<'\n';
                    else cout<<"activeedge is null\n";
                    cout<<"activelen: "<<activeLength<<'\n';
                    check();
                    if(!activeLength)   //activeLength=0说明不需要分裂，直接给activeNode添加子节点
                        activeNode->child.insert(new SuffixNode(right));
                    else {  //下面是分裂节点的情况
                        auto temp=new SuffixNode(activeEdge->l,activeEdge->l+activeLength-1);
                        activeNode->child.erase(activeEdge);
                        activeNode->child.insert(temp);
                        temp->child.insert(activeEdge);
                        activeEdge->l=activeEdge->l+activeLength;
                        temp->child.insert(new SuffixNode(right));
                        activeEdge=temp;
                        if(previous== nullptr)  previous=temp;
                        else{
                            previous->suffixLink=temp;
                            previous=temp;
                        }
                    }
                    changeActive(++left,right);
                    --remainder;
                }
            }
            ++right;
        }
    }
    /**
     * 2022-03-31 18:07:04 GMT+8
     * @param t 查找的子串
     * @return 子串t对应的开始索引
     */
    int64_t findSubstring(std::string &t){

    }
    /**
     * 2022-03-31 18:07:06 GMT+8
     * @param t 统计的子串
     * @return 子串t的出现次数
     */
    int64_t statisticSubstring(string &t){
        int64_t times=0;

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
    /**
     * 2022-04-02 09:52:53 GMT+8
     * 检查构造的树的结构
     */
    void check(){
        queue<SuffixNode *> q;
        q.push(root);
//        cout<<111;
        while (!q.empty()){
            auto now=q.front();
//            cout<<222;
            if(now!= nullptr){
                cout<<"now: "<<now->l<<' '<<now->r<<" its children:\n";
                //                cout<<333;
                for (const auto &item : now->child){
                    cout<<item->l<<' '<<item->r<<'\n';
                    if(item!= nullptr)
                        q.push(item);
                }
            }
            q.pop();
        }
        cout<<"====\n";
    }
};

#endif //_SUFFIXTRIE_H
