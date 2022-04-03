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
        int64_t r=0;
        set<SuffixNode *> child;    //使用set存子节点，crud时更高效
        SuffixNode *SuffixLink= nullptr;
        SuffixNode()=default;
        SuffixNode(int64_t l):l(l){}
        SuffixNode(int64_t l,int64_t r):l(l),r(r){}
    };
    bool existPrefixContainSuffix(int64_t left, int64_t right){
        bool flag=0;
        queue<SuffixNode *> q;
        q.push(activeNode);
        while(!q.empty()){
            auto now=q.front();
            activeLength=0;
            if(now!= nullptr)
                for (const auto &item : now->child){
                    while (item->l+activeLength<=right and str[item->l+activeLength]==str[left])
                        activeLength++;
                    --activeLength;
                    if(item->l+activeLength<right)  return 1;
                    if(activeLength){
                        activeNode=item;
                        q.push(item);
                        flag=1;
                        break;
                    }
                }
            q.pop();
        }
        return flag;
    }
    void changeActive(int64_t left, int64_t right){
        if(!activeLength)   return ;
        //rule 1:
        if(activeNode==root){
            --activeLength;
            for (const auto &item : root->child)
                if(str[item->l]==str[left])
                    activeEdge=item;
            return ;
        }
        //rule 3:
        if(activeNode->SuffixLink!= nullptr){
            activeNode=activeNode->SuffixLink;
            return ;
        }else{
            activeNode=root;
            existPrefixContainSuffix(left, right);
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
        root=new SuffixNode(-1,-1);//标致为根节点，不存放任何字符
        SuffixNode *previous= nullptr;
        int64_t left=0,right=0;
        remainder=activeLength=0;
        activeNode=root;
        activeEdge= nullptr;  //非空情况表示要对活动节点的哪个子节点进行分裂
        while (right!=str.size()-1 and left!=str.size()-1){
            //下面的最外层if-else可以优化结构
            if(existPrefixContainSuffix(left, right)){
                //我们仅记录, 并不进行实际操作
                ++remainder;
            }else{  //这里一定有remainder>0吗?
                previous= nullptr;
                ++remainder;
                while (remainder>0){
                    if(activeLength==0){
                        //这个情况下的活动点的设置要单独分开
                        //activeLength=0说明不需要分裂，直接给activeNode添加子节点
                        activeNode->child.insert(new SuffixNode(right));
                    }else{
                        //对活动节点的某个子节点(activeEdge)进行分裂
                        activeNode->child.erase(activeEdge);
                        //下面这个节点不会是叶子节点了，所以左右端点都可以定下来了
                        // rule 2 and 4:
                        if(previous!= nullptr)  {
                            previous->SuffixLink=new SuffixNode(activeEdge->l,activeEdge->l-activeLength+1);
                            previous=previous->SuffixLink;
                        }else previous=new SuffixNode(activeEdge->l,activeEdge->l-activeLength+1);
                        activeNode->child.insert(previous);
                        //下面的两个新插入的节点后续还会是叶子节点，所以只能定下左端点
                        activeEdge->child.insert(new SuffixNode(activeEdge->l-activeLength+2));
                        activeEdge->child.insert(new SuffixNode(right));
                        //activeLength, activeEdge, activeNode都需要更新, 交给下面的函数进行更新
                        changeActive(++left, right);
                    }
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
            if(now!= nullptr)
//                cout<<333;
                for (const auto &item : now->child){
                    if(item!= nullptr){
//                        cout<<444;
                        q.push(now);
                        cout<<str.substr(now->l, now->r-now->l+1)<<' ';
                    }
                }
            q.pop();
        }
    }
};

#endif //_SUFFIXTRIE_H
