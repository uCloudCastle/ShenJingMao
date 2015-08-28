#include "helper.h"
#include <iostream>
#include <QQueue>

bool Helper::initPathStruct(PathStruct* ps, int catPos, QList<int>& list)
{
    if ( ps == NULL || list.size() == 0 )
        return false;

    ps->m_Node = new TreeNode(catPos, 0);
    ps->m_Hash.insert(catPos, 0);
    int floor = 1;

    QQueue<TreeNode*> que;
    que.enqueue(ps->m_Node);
    que.enqueue(new TreeNode());

    while( !que.empty() )
    {
        TreeNode* node = que.dequeue();
        if ( que.size() == 0 )
        {
            delete node;
            break;
        }

        if ( node->val == nullInit )                // this floor is over
        {
            ++floor;
            for ( int i = 0; i < que.size(); ++i )
            {
                ps->m_Hash.insert(que.at(i)->val, floor);
            }
            que.enqueue(new TreeNode());
            delete node;
        }
        else
        {
            appendChildren(node, ps->m_Hash, list, floor);
            for ( int i = 0; i < node->childList.size(); ++i )
            {
                que.enqueue(node->childList.at(i));
            }
        }
    }

//    que.enqueue(ps->m_Node);
//    while( !que.empty() )
//    {
//        TreeNode* node = que.dequeue();
//        std::cout << "parent is : " << node->val << " " << node->depth << ";";
//        std::cout << "child is : ";
//        for ( int i = 0; i < node->childList.size(); ++i )
//        {
//            que.enqueue(node->childList.at(i));
//            std::cout << node->childList.at(i)->val << " " << node->depth << ";";
//        }
//        std::cout << std::endl;
//    }
//    qDebug() << ps->m_Hash;
    return true;
}

int Helper::findVictoryPath(PathStruct* ps, QList<int>& list)
{
    // 先检查周围是否存在边界
    QVector<TreeNode*> children = ps->m_Node->childList;
    for ( int i = 0; i < children.size(); ++i )
    {
        if ( isBorder( children.at(i)->val ) )
            return getNeighborDirByPos(ps->m_Node->val, children.at(i)->val);
    }

    QList<int> result = depthFirstSearch(ps->m_Node, list);
    if ( result.last() == nullInit )
        return nullInit;
    else
        return getNeighborDirByPos(ps->m_Node->val, result.at(1));
}

QList<int> Helper::depthFirstSearch(TreeNode* root, QList<int>& list)
{
    QList<int> path;
    if ( root == NULL )
        return path;

    QVector<TreeNode*> around = root->childList;
    int count = 0;                                                       // 邻居中非障碍边界的个数
    for ( int i = 0; i < around.size(); ++i )                // 删除边界值
    {
        if ( isBorder(around.at(i)->val) )
        {
            around.remove(i);
            ++count;
            --i;
        }
    }

    if ( count == 0 )                                                      // 不包含边界
    {                                                                               // 则支路数需大于层数才继续下探
        if ( around.size() > root->depth )
        {
            path.append(root->val);
            QList< QList<int> > temp;
            for( int i = 0; i < around.size(); ++i )
            {
                temp.append(depthFirstSearch(around.at(i), list));
            }

            int minLen = nullInit;
            int pos = nullInit;
            for ( int i = 0; i < temp.size(); ++i )
            {
                if ( (temp.at(i).last() != nullInit) && (minLen > temp.at(i).size()) )
                {
                    minLen = temp.at(i).size();
                    pos = i;
                }
            }
            if ( pos != nullInit )                                             // 底层找到, 向上返回最短有效路径
            {
                path.append(temp.at(pos));
            }
            else
            {
               path.append(nullInit);                                     // 底层未找到, 该路径失败
            }
        }
        else
        {
            path.append(nullInit);
        }
    }
    else if ( count == 1 )                                               // 包含一个边界
    {                                                                                // 若还包含其他支路则继续下探
        if ( around.size() >= 1 )
        {
            path.append(root->val);
            QList< QList<int> > temp;
            for( int i = 0; i < around.size(); ++i )
            {
                temp.append(depthFirstSearch(around.at(i), list));
            }

            int minLen = nullInit;
            int pos = nullInit;
            for ( int i = 0; i < temp.size(); ++i )
            {
                if ( (temp.at(i).last() != nullInit) && (minLen > temp.at(i).size()) )
                {
                    minLen = temp.at(i).size();
                    pos = i;
                }
            }
            if ( pos != nullInit )                                             // 底层找到, 向上返回最短有效路径
            {
                path.append(temp.at(pos));
            }
            else
            {
               path.append(nullInit);                                     // 底层未找到, 该路径失败
            }
        }
        else
        {
            path.append(nullInit);
        }
    }
    else                                                                            // count >= 2, 该路径可行, 迭代返回
    {
        path.append(root->val);
    }
    return path;
}

void Helper::appendChildren(TreeNode* node, QHash<int, int>& hash, QList<int>& list, int floor)
{
    QVector<int> around = getNeighbor(node->val);
    if (around.size() < 6)
        return;

    for ( int i = 0; i < 6; ++i )
    {
        if ( list.contains(around.at(i)) == false && hash.contains(around.at(i)) == false )
        {
            node->childList.append(new TreeNode(around.at(i), floor));
        }
    }
}

bool Helper::isBorder(int pos)
{
    if ( pos < c_dimen || pos > c_dimen*(c_dimen-1) || pos % c_dimen == 0 || ( pos + 1 ) % c_dimen == 0 )
    {
        return true;
    }
    return false;
}

QVector<int> Helper::getNeighbor(int nodePos)
{
    QVector<int> around;
    if ( isBorder(nodePos) )
    {
        return around;
    }

    int oddCompensate;                // 奇数行补偿值
    if ( (int)(nodePos / c_dimen) % 2 == 0 )
        oddCompensate = 0;
    else
        oddCompensate = 1;

    around << nodePos - 1 << nodePos - 10 + oddCompensate << nodePos - 9 + oddCompensate
                    << nodePos + 1 << nodePos + 9 + oddCompensate << nodePos + 8 + oddCompensate;
    return around;
}

int Helper::getNeighborDirByPos(int sourPos, int dirPos)
{
    QVector<int> around = getNeighbor(sourPos);
    for ( int i = 0; i < around.size(); ++i )
    {
        if ( around.at(i) == dirPos )
            return i+1;
    }
    return nullInit;
}

void Helper::initMap(int map[][c_dimen], QList<int>& list)
{
    for ( int i = 0; i < c_dimen; ++i )
    {
        for ( int j = 0; j < c_dimen; ++j )
        {
            map[i][j] = nullInit;
        }
    }

    // 将障碍值设置为 c_obstacle
    for ( int i = 0; i < list.size(); ++i )
    {
        int row = list.at(i) / c_dimen;
        int col = list.at(i) % c_dimen;
        map[row][col] = c_obstacle;
    }

    for ( int cycle = 0; cycle < (c_dimen >> 1); ++cycle )
    {
        int endPos = c_dimen - cycle - 1;
        setRowData(map, cycle, cycle, endPos);
        setColData(map, cycle, cycle+1, endPos);
        setRowData(map, endPos, cycle+1, endPos);
        setColData(map, endPos, cycle+1, endPos-1);
    }
    if ( ( c_dimen ^ (c_dimen - 1) ) == 1 ) {
        int centerPos = c_dimen >> 1;
        setRowData(map, centerPos, centerPos , centerPos);
    }

    // 添加补丁,避免逆向出路被误判
    // 注意 makePatch 的边界
    int len = c_dimen - 1;
    for ( int i = 1; i < len; ++i )
    {
        for ( int j = 1; j < len; ++j )
        {
            makePatch(map, i, j);
        }
    }
}

void Helper::setRowData(int map[][c_dimen], int rowNum, int startNo, int endNo)
{
    if ( map == NULL || *map == NULL )
        return;

    if ( rowNum == 0 || rowNum == c_dimen-1 )
    {
        for ( int i = startNo; i <= endNo; ++i )
        {
            if ( map[rowNum][i] == nullInit )
                map[rowNum][i] = 0;
        }
    }
    else
    {
        for ( int i = startNo; i <= endNo; ++i )
        {
            if ( map[rowNum][i] == nullInit ) {
                QList<int> minNeib = getMinFormNeighbor(map, rowNum, i);
                map[rowNum][i] = minNeib.at(0) + 1;
            }
        }
    }
}

void Helper::setColData(int map[][c_dimen], int colNum, int startNo, int endNo)
{
    if ( map == NULL || *map == NULL )
        return;

    if ( colNum == 0 || colNum == c_dimen-1 )
    {
        for ( int i = startNo; i <= endNo; ++i )
        {
            if ( map[i][colNum] == nullInit )
                map[i][colNum] = 0;
        }
    }
    else
    {
        for ( int i = startNo; i <= endNo; ++i )
        {
            if ( map[i][colNum] == nullInit ) {
                QList<int> minNeib = getMinFormNeighbor(map, i, colNum);
                map[i][colNum] = minNeib.at(0) + 1;
            }
        }
    }
}

void Helper::makePatch(int map[][c_dimen], int row, int col)
{
    // 迭代函数, 为了效率不再判断边界
    // 最外围一圈数据不应该进入计算
    // 注意不要优化 c_obstacle
    if ( map[row][col] == c_obstacle )
        return;

    int min = getMinFormNeighbor(map, row, col).at(0) + 1;
    if ( map[row][col] == min ) {
        return;
    }
    else
    {
        map[row][col] = min;
        if ( ( row ^ (row - 1) ) == 1 )
        {
            makePatch(map, row, col-1);
            makePatch(map, row-1, col);
            makePatch(map, row-1, col+1);
            makePatch(map, row, col+1);
            makePatch(map, row+1, col+1);
            makePatch(map, row+1, col);
        }
        else
        {
            makePatch(map, row, col-1);
            makePatch(map, row-1, col-1);
            makePatch(map, row-1, col);
            makePatch(map, row, col+1);
            makePatch(map, row+1, col);
            makePatch(map, row+1, col-1);
        }
    }
}

QList<int> Helper::getMinFormNeighbor(int map[][c_dimen], int row, int col)
{
    QList<int> neighbor;
    if ( ( row ^ (row - 1) ) == 1 )
    {
        neighbor << map[row][col-1]  << map[row-1][col]
                << map[row-1][col+1]  << map[row][col+1]
                << map[row+1][col+1] << map[row+1][col];
    }
    else
    {
        neighbor << map[row][col-1]  << map[row-1][col-1]
                << map[row-1][col]  << map[row][col+1]
                << map[row+1][col] << map[row+1][col-1];
    }

    // 找到邻居中的最小值
    QList<int> retList;
    int min = nullInit;
    for ( int i = 0; i < neighbor.size(); ++i )
    {
        if ( min > neighbor.at(i) )
            min = neighbor.at(i);
    }
    retList.append(min);

    // 返回最小邻居的位置
    for ( int pos = 0; pos < neighbor.size(); ++pos )
    {
        if ( neighbor.at(pos) == min )
            retList.append(pos+1);
    }
    return retList;
}

// 最大通路,寻找邻居中路径最宽的一个
int Helper::getMaxPathway(int map[][c_dimen], int row, int col)
{
    int max = 0;
    int dir = 0;
    QList<int> rows;
    QList<int> cols;
    if ( ( row ^ (row - 1) ) == 1 )
    {
        rows << row << row-1 << row-1 << row << row+1 << row+1;
        cols << col-1 << col << col+1 << col+1 << col+1 << col;
    }
    else
    {
        rows << row << row-1 << row-1 << row << row+1 << row+1;
        cols << col-1 << col-1 << col << col+1 << col << col-1;
    }

    // 找到值超过 c_obstacle 的邻居
    // 返回空闲路径最多的邻居的方位
    for ( int i = 0; i < rows.size(); ++i )
    {
        if ( ( map[rows.at(i)][cols.at(i)] > c_obstacle ) && ( max < getAbleDir(map, rows.at(i), cols.at(i) ) ))
        {
            max = getAbleDir(map, rows.at(i), cols.at(i));
            dir = i + 1;
        }
    }
    return dir;
}

int Helper::getAbleDir(int map[][c_dimen], int row, int col)
{
    QList<int> neighbor;
    if ( ( row ^ (row - 1) ) == 1 )
    {
        neighbor << map[row][col-1]  << map[row-1][col]
                << map[row-1][col+1]  << map[row][col+1]
                << map[row+1][col+1] << map[row+1][col];
    }
    else
    {
        neighbor << map[row][col-1]  << map[row-1][col-1]
                << map[row-1][col]  << map[row][col+1]
                << map[row+1][col] << map[row+1][col-1];
    }

    int retVal = 0;
    for ( int i = 0; i < neighbor.size(); ++i )
    {
        if ( neighbor.at(i) > c_obstacle )
            ++retVal;
    }
    return retVal;
}
