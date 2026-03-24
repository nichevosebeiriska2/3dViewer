#ifndef NODE_TREE_H
#define  NODE_TREE

#include <QTreeView>
#include <QStandardItem>
#include <QStandardItemModel>

#include "common.h"

class NodeItem : public QStandardItem
{
};

NodeItem *CreateTreeFromObjectNode(ObjectNodeSTL *pRootNode);


class TreeNodeVisitor
{
public:
	TreeNodeVisitor() = default;

	NodeItem *FromSTL(ObjectNodeSTL *pNodeRoot);
	NodeItem *FromOBJ(ObjectNodeOBJ *pNodeRoot);
	NodeItem *FromGLTF(ObjectNodeGLTF *pNodeRoot);

protected:
	NodeItem *FromSTL(ObjectNodeSTL *pNode, NodeItem * pParent);
	NodeItem *FromOBJ(ObjectNodeOBJ *pNode, NodeItem *pParent);
	NodeItem *FromGLTF(ObjectNodeGLTF *pNode, NodeItem *pParent);
};

#endif // !NODE_TREE_H
