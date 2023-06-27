//
// Created by dell on 2023/6/19.
//

#include "../include/memo.h"

auto Memo::AddMemo(std::string memo) -> bool {
    if (completed_map_.find(memo) != completed_map_.end())
    {
        return false;
    }
    uncompleted_momo_.push_front(memo);
    uncompleted_memo_map_[memo] = uncompleted_momo_.begin();
    completed_map_[memo] = false;
    return true;
}

auto Memo::SetCompleted(std::string memo) -> bool {
    if (completed_map_.find(memo) == completed_map_.end())
    {
        return false;
    }

    if (completed_map_[memo])
    {
        return true;
    }

    auto memo_iterator = uncompleted_memo_map_[memo];
    uncompleted_momo_.erase(memo_iterator);
    uncompleted_memo_map_.erase(memo);

    completed_memo_.push_front(memo);
    completed_memo_map_[memo] = completed_memo_.begin();
    completed_map_[memo] = true;
    return true;
}

auto Memo::SetUnCompleted(std::string memo) -> bool {
    if (completed_map_.find(memo) == completed_map_.end())
    {
        return false;
    }

    if (!completed_map_[memo])
    {
        return true;
    }

    if (uncompleted_memo_map_.find(memo) != uncompleted_memo_map_.end())
    {
        completed_map_[memo] = false;
        return true;
    }

    // 将completed_memo中的移入到uncompleted_memo中
    auto memo_iterator = completed_memo_map_[memo];
    completed_memo_.erase(memo_iterator);
    completed_memo_map_.erase(memo);

    uncompleted_momo_.push_front(memo);
    uncompleted_memo_map_[memo] = uncompleted_momo_.begin();
    completed_map_[memo] = true;
    return true;
}

void Memo::DeleteMemo(std::string memo) {
    if (completed_map_.find(memo) == completed_map_.end())
    {
        return ;
    }

    auto del = completed_map_[memo];
    if (del)
    {
        auto memo_iterator = completed_memo_map_[memo];
        completed_memo_.erase(memo_iterator);
        completed_memo_map_.erase(memo);
    } else
    {
        auto memo_iterator = uncompleted_memo_map_[memo];
        uncompleted_momo_.erase(memo_iterator);
        uncompleted_memo_map_.erase(memo);
    }
    completed_map_.erase(memo);
}


