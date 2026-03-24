#include "node_tree.h"

NodeItem *CreateTreeFromObjectNode(NodeItem * parent, ObjectNodeSTL *pNode)
{
	NodeItem *item = new NodeItem();

	item->appendRow(new QStandardItem("123"));

	return nullptr;
}

NodeItem *CreateTreeFromObjectNode(ObjectNodeSTL *pRootNode)
{
	NodeItem *item = new NodeItem();

	item->appendRow(new QStandardItem("123"));

	for(auto &node : pRootNode->m_vecChildren)
	{
		item->appendRow(CreateTreeFromObjectNode(item, node));
	}
	//for(auto& child : pRootNode->mV)

	return item;
}

NodeItem* TreeNodeVisitor::FromSTL(ObjectNodeSTL *pNodeRoot)
{
	NodeItem *item = new NodeItem();
	item->appendRow(new QStandardItem(pNodeRoot->m_strName));

	for(auto &node : pNodeRoot->m_vecChildren)
		item->appendRow(FromSTL(node));

	return item;
}

NodeItem *TreeNodeVisitor::FromSTL(ObjectNodeSTL *pNodeRoot, NodeItem* pParent)
{
	NodeItem *item = new NodeItem();

	for(auto &node : pNodeRoot->m_vecChildren)
		item->appendRow(FromSTL(node, item));

	pParent->appendRow(item);

	return nullptr;
}

NodeItem* TreeNodeVisitor::FromOBJ(ObjectNodeOBJ *pNodeRoot)
{
	NodeItem *item = new NodeItem();

	return item;
}

NodeItem *TreeNodeVisitor::FromOBJ(ObjectNodeOBJ *pNodeRoot, NodeItem *pParent)
{
	NodeItem *item = new NodeItem();

	return item;
}

NodeItem* TreeNodeVisitor::FromGLTF(ObjectNodeGLTF *pNodeRoot)
{
	NodeItem *item = new NodeItem();

	return item;

}

NodeItem *TreeNodeVisitor::FromGLTF(ObjectNodeGLTF *pNodeRoot, NodeItem *pParent)
{
	NodeItem *item = new NodeItem();

	return item;
}