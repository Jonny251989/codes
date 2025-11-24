/*

Input: intervals = [[1,3],[2,6],[8,10],[15,18]]
Output: [[1,6],[8,10],[15,18]]
Explanation: Since intervals [1,3] and [2,6] overlap, merge them into [1,6].


Input: intervals = [[1,4],[4,5]]
Output: [[1,5]]
Explanation: Intervals [1,4] and [4,5] are considered overlapping.

*/


// Input: intervals = [[15,18], [1,3], [8,10], [2,6]]

#include <vector>
#include <iostream>
#include <algorithm>

// Input: intervals = [[1,3],[2,6],[8,10],[15,18]]

using namespace std;
std::vector<vector<int>> merge(std::vector<std::vector<int>>& intervals) {

    std::sort(intervals.begin(), intervals.end(), [](std::vector<int>& i1, std::vector<int>& i2){
        if (i1[0] < i2[0]) return true;
        else return false;
    });
    for(const auto val: intervals)
        std::cout<<val[0]<<","<<val[1]<<"\n";
    std::cout<<"\n";
    std::vector<std::vector<int>> new_intervals{};

    int i = 0;
    int begin = intervals[0][0];
    int end  = intervals[0][1];
    size_t length = intervals.size();

    // Input: intervals = [[1,3],[2,6],[4,10],[15,18]]

    for(size_t i = 1; i < length; ++i){
        if(end < intervals[i][1] && end > intervals[i][0]){
            end  = intervals[i][1];
            if(begin == new_intervals.back()[0])
                new_intervals.back()[1] = intervals[i][1];
            else
                new_intervals.push_back({begin, intervals[i][1]});
        }else{

            //new_intervals.push_back({intervals[i][0], intervals[i][1]});
            begin = intervals[i][0];
            end  = intervals[i][1];                       
        }
    }
    return new_intervals;
    // while(i < length){
    //     if(end < intervals[i][1] && end > intervals[i][0]){

    //     }
    //     else{
    //         begin = intervals[i][0];
    //         end  = intervals[i][1];           
    //     }

    // }

}

int main(){

    std::vector<std::vector<int>> m_v{{1,3}, {15,18}, {8,10}, {2,6}};

    auto new_intervals = merge(m_v);

    for(const auto val: new_intervals)
        std::cout<<val[0]<<","<<val[1]<<"\n";

    return 0;
}