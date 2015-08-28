#ifndef PATHSTRUCT_H
#define PATHSTRUCT_H

#include <QVector>
#include <QHash>
#include <QDebug>
const int nullInit = 999;

struct TreeNode {
    int val;
    int depth;
    QVector<TreeNode*> childList;

    TreeNode(int v = nullInit, int d = nullInit) :
    val(v), depth(d){}

    ~TreeNode()
    {
        TreeNode* n;
        foreach(n, childList)
            delete n;
    }
};

struct PathStruct {
    QHash<int, int> m_Hash;
    TreeNode *m_Node;

    PathStruct() { m_Node = NULL; }
    ~PathStruct() { delete m_Node; }
};

#endif // PATHSTRUCT_H
