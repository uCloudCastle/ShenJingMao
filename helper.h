#ifndef HELPER_H
#define HELPER_H

#include <QList>
#include <QHash>
#include "pathstruct.h"
const int c_dimen = 9;
const int c_obstacle = 100;

class Helper
{
public:
    Helper(){}

    /******************Normal Mode Function**************************/
    // 初始化 map,不可通行区域设置为 c_obstacle, 可通行区域设置为到边界的最短长度
    void initMap(int map[][c_dimen], QList<int>& list);

    // 返回最大通路前进方向
    int getMaxPathway(int map[][c_dimen], int row, int col);

    // 返回邻居中的最小值,第一个位置为最小值,之后为等于该最小值的方位
    QList<int> getMinFormNeighbor(int map[][c_dimen], int row, int col);

    // 返回该点可前进方向的数量
    int getAbleDir(int map[][c_dimen], int row, int col);

    /******************Hard Mode Function**************************/
    // 初始化 PathStruct, 困难模式所用数据结构
    bool initPathStruct(PathStruct* ps, int catPos, QList<int>& list);

    // 寻找必胜路径
    int findVictoryPath(PathStruct* ps, QList<int>& list);

private:
    // 打补丁, 优化数组
    // 对于超过100的数,判断其是否真的无解
    void makePatch(int map[][c_dimen], int row, int col);

    // 设置行数据,按周边最小值加 1
    void setRowData(int map[][c_dimen], int rowNum, int startNo, int endNo);

    // 设置列数据,按周边最小值加 1
    void setColData(int map[][c_dimen], int colNum, int startNo, int endNo);

    // 是否为边界
    bool isBorder(int pos);

    // 获取周围的 Pos 值
    QVector<int> getNeighbor(int nodePos);

    // 获取目标地址的方位(dirpos不在周围 6 个 pos 中返回 0)
    int getNeighborDirByPos(int sourPos, int dirPos);

    // 添加 node 节点的子节点, 不添加 hash 与 list 中的值
    void appendChildren(TreeNode* node, QHash<int, int>& hash, QList<int>& list, int floor);

    // 广度优先遍历
    QList<int> depthFirstSearch(TreeNode* root, QList<int>& list);
};

#endif // HELPER_H
