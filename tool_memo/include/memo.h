//
// Created by dell on 2023/6/19.
//

#ifndef TOOL_MEMO_MEMO_H
#define TOOL_MEMO_MEMO_H

#include <stdio.h>
#include <iostream>
#include <vector>
#include <list>
#include <unordered_map>

// 备忘录的基本功能,增，删，改
class Memo {
public:
    Memo() = default;
    auto AddMemo(std::string memo) -> bool ;
    /** 将一个备忘录移入已完成的列表中 */
    auto SetCompleted(std::string memo) -> bool ;
    /** 将一个备忘录移入未完成的列表中 */
    auto SetUnCompleted(std::string memo) -> bool ;
    /** 删除一个备忘录 */
    void DeleteMemo(std::string memo);



private:
    std::unordered_map<std::string, bool> completed_map_;
    std::list<std::string> completed_memo_;
    /** 使用unordered_map是为了更快的寻找到list中的位置，可以更快的进行更改 */
    std::unordered_map<std::string, std::list<std::string>::iterator> completed_memo_map_;
    std::list<std::string> uncompleted_momo_;
    std::unordered_map<std::string, std::list<std::string>::iterator> uncompleted_memo_map_;


};

#endif
