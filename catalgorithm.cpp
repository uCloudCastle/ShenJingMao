#include "catalgorithm.h"
#include "helper.h"
#include <math.h>
#include <iostream>

CatAlgorithm::CatAlgorithm(QObject *parent) :
    QObject(parent), m_hardLv(Eazy), m_helper(new Helper)
{
}

int CatAlgorithm::getNextDirection(int catPos, QList<int> map)
{
    if ( catPos < -1 || catPos >= c_dimen*c_dimen || map.size() <= c_dimen/2 )
        Q_ASSERT("getNextDirection Parameter Error !");

    if ( m_hardLv == Eazy ) {
        return EazyMode(catPos, map);
    }
    else if ( m_hardLv == Normal ) {
        return NormalMode(catPos, map, DirLeftFirst);
    }
    else { // m_hardLv == Hard
        return HardMode(catPos, map);
    }
}

// 简单模式, 随机选择周围可走方向
int CatAlgorithm::EazyMode(int catPos, QList<int>& list)
{
    int oddCompensate;                // 奇数行补偿值
    if ( (int)(catPos / 9) % 2 == 0 )
        oddCompensate = 0;
    else
        oddCompensate = 1;

    QVector<int> directionArray;          // 方位数组,从左边顺时针绕圆,为[1-6]
    if ( list.contains(catPos - 1) == false )
        directionArray.push_back(1);
    if ( list.contains(catPos - 10 + oddCompensate) == false )
        directionArray.push_back(2);
    if ( list.contains(catPos - 9 + oddCompensate) == false )
        directionArray.push_back(3);
    if ( list.contains(catPos + 1) == false )
        directionArray.push_back(4);
    if ( list.contains(catPos + 9 + oddCompensate) == false )
        directionArray.push_back(5);
    if ( list.contains(catPos + 8 + oddCompensate) == false )
        directionArray.push_back(6);

    if ( directionArray.size() == 0 )             // 如果无路可走,返回 0
        return 0;

    int randomNum = rand() % directionArray.size();      // 否则在可走方向上随机返回
    return directionArray.at(randomNum);
}

// 普通模式, 使用最短路径算法与最大通路算法
int CatAlgorithm::NormalMode(int catPos, QList<int>& list, NormalDirMode dm)
{
    int map[c_dimen][c_dimen];
    m_helper.data()->initMap(map, list);

    int catRow = catPos / c_dimen;
    int catCol = catPos % c_dimen;
    QList<int> result = m_helper.data()->getMinFormNeighbor(map, catRow, catCol);

    if ( result.at(0) < c_obstacle )                                                           // next
    {
        if ( dm == DirLeftFirst )
            return result.at(1);
        else
        {
            int r = rand() % result.size();
            if ( r == 0 ) { ++r; }
            return result.at(r);
        }
    }
    else if ( (result.at(0) == c_obstacle) && ( result.size() == 7 ) )    // over
    {
        return 0;
    }
    else                                                                                                        // blockaded
    {
        return m_helper.data()->getMaxPathway(map, catRow, catCol);
    }
}

// 困难模式, 基于多叉树寻找必胜路径
// 必胜路径满足三个条件：
// 1. 当cat 到达某个点时，该点至少含有两条路径
// 2. 无论玩家如何操作，该路径总能保证条件 1 成立
// 3. 终点与两个可出边界相邻
int CatAlgorithm::HardMode(int catPos, QList<int>& list)
{
    PathStruct* ps = new PathStruct();
    m_helper.data()->initPathStruct(ps, catPos, list);
    int dir = m_helper.data()->findVictoryPath(ps, list);
    delete ps;

    if ( dir == nullInit )
        return NormalMode(catPos, list, DirRandom);
    else
        return dir;
}
