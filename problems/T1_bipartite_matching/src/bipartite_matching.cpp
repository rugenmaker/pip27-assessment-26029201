#include "bipartite_matching.hpp"

#include <memory>
#include <vector>

// (原有注释)这是可编译的起始模板。请在此处实现带权二分图最大权匹配算法。

// 新添加<limits>用于初始化best_weight
#include <limits>
// 新添加<functional>用于实现dfs自我调用,AI现教现学的
#include <functional>

struct BipartiteMatcher::Impl
{
    Impl(std::uint32_t left_count, std::uint32_t right_count, const std::vector<Edge> &edges)
        : left_count(left_count), right_count(right_count), edges(edges) {}

    std::uint32_t left_count;
    std::uint32_t right_count;
    std::vector<Edge> edges;
};

BipartiteMatcher::BipartiteMatcher(std::uint32_t left_count, std::uint32_t right_count,
                                   const std::vector<Edge> &edges)
    : impl_(std::make_unique<Impl>(left_count, right_count, edges)) {}

BipartiteMatcher::~BipartiteMatcher() = default;

std::vector<std::int32_t> BipartiteMatcher::maximumWeightMatching() const
{
    // (原有注释)下述区域实现算法
    if (impl_->edges.empty())
    {
        // 没有边
        return std::vector<std::int32_t>(impl_->left_count, -1);
    }
    else if (impl_->edges.size() == 1)
    {
        std::vector<std::int32_t> onlyanswer(impl_->left_count, -1);
        const Edge &e = impl_->edges[0];
        onlyanswer[e.left] = static_cast<std::int32_t>(e.right);
        // 只有一条边
        return onlyanswer;
    }
    else
    {
        std::vector<std::int32_t> answer(impl_->left_count, -1);
        std::vector<std::int32_t> bestanswer(impl_->left_count, -1);
        std::vector<bool> used(impl_->right_count, false);
        std::int64_t best_weight = std::numeric_limits<std::int64_t>::lowest();

        // 定义Lambda,直接使用外部变量
        std::function<void(std::uint32_t, std::int64_t, bool)> dfs;
        dfs = [&](std::uint32_t i, std::int64_t current_weight, bool selected)
        {
            // 所有左顶点均已处理
            if (i == impl_->left_count)
            {
                if (selected && current_weight > best_weight)
                {
                    best_weight = current_weight;
                    bestanswer = answer;
                }
                return;
            }
            // 策略一:当前左顶点不匹配
            answer[i] = -1;
            // 旧层递归
            dfs(i + 1, current_weight, selected);
            // 策略二:尝试当前左顶点的每一条边
            for (const Edge &e : impl_->edges)
            {
                if (e.left != i)
                    continue;
                if (used[e.right])
                    continue;
                used[e.right] = true;
                answer[i] = static_cast<std::int32_t>(e.right);
                // 新层递归
                dfs(i + 1, current_weight + e.weight, true);
                // 回溯
                answer[i] = -1;
                used[e.right] = false;
            }
        }; // Lambda是赋值语句，需要以;结尾

        dfs(0, 0, false);
        return bestanswer;
    }
}