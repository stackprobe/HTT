#include "all.h"

static rbtNode_t *CreateNode(void *element, boolean red, rbtNode_t *p)
{
	rbtNode_t *node = (rbtNode_t *)memAlloc(sizeof(rbtNode_t));

	node->Element = element;
	node->Red = red;
	node->Parent = p;
	node->Children[0] = NULL;
	node->Children[1] = NULL;

	return node;
}

static rbtNode_t *LastParent;
static int LastDirect;
static rbtNode_t *GetNode(rbtTree_t *tree, void *element)
{
	rbtNode_t *node;

	if(tree->LastAccessNode && tree->FuncComp(tree->LastAccessNode->Element, element, tree->ExtraData) == 0)
		return tree->LastAccessNode;

	LastParent = NULL;
	LastDirect = 0;

	node = tree->Root;

	while(node)
	{
		int ret_comp = tree->FuncComp(node->Element, element, tree->ExtraData);

		if(!ret_comp)
			break;

		LastParent = node;
		LastDirect = 0 < ret_comp ? 0 : 1;

		node = node->Children[LastDirect];
	}
	tree->LastAccessNode = node;

	return node;
}

rbtTree_t *rbtCreateTree(int (*funcComp)(void *, void *, void *), void (*funcRelease)(void *, void *), void *extraData)
{
	rbtTree_t *tree = (rbtTree_t *)memAlloc(sizeof(rbtTree_t));

	errorCase(!funcComp);
	errorCase(!funcRelease);

	tree->FuncComp = funcComp;
	tree->FuncRelease = funcRelease;
	tree->ExtraData = extraData;
	tree->Root = NULL;
	tree->LastAccessNode = NULL;

	return tree;
}
void rbtReleaseTree(rbtTree_t *tree)
{
	autoList<rbtNode_t *> *nodes = new autoList<rbtNode_t *>();

	errorCase(!tree);
	nodes->AddElement(tree->Root);

	while(nodes->GetCount())
	{
		rbtNode_t *node = nodes->UnaddElement();

		if(node)
		{
			nodes->AddElement(node->Children[0]);
			nodes->AddElement(node->Children[1]);

			tree->FuncRelease(node->Element, tree->ExtraData);

			memFree(node);
		}
	}
	delete nodes;

	memFree(tree);
}

/*
	direct
		0: �� -> �E (���v���)
		1: �E -> �� (�����v���)
*/
static void Rotate(rbtTree_t *tree, rbtNode_t *node, int direct)
{
	rbtNode_t *p = node->Parent;
	rbtNode_t *c = node->Children[direct];
	rbtNode_t *cc;

	cc = c->Children[1 - direct];

	if(p)
	{
		p->Children[p->Children[0] == node ? 0 : 1] = c;
	}
	else
	{
		tree->Root = c;
	}

	c->Parent = p;
	c->Children[1 - direct] = node;

	node->Parent = c;
	node->Children[direct] = cc;

	if(cc)
	{
		cc->Parent = node;
	}
}

void rbtAddElement(rbtTree_t *tree, void *element)
{
	rbtNode_t *node;

	errorCase(!tree);
	errorCase(!element);
	errorCase(GetNode(tree, element)); // ? �����̗v�f�Əd������B

	if(LastParent)
	{
		node = CreateNode(element, 1, LastParent);

		LastParent->Children[LastDirect] = node;
		tree->LastAccessNode = node;

restart:
		if(LastParent->Red)
		{
			rbtNode_t *pp = LastParent->Parent;
			rbtNode_t *pc;

			pc = pp->Children[pp->Children[0] == LastParent ? 1 : 0];

			if(pc && pc->Red)
			{
				LastParent->Red = 0;
				pc->Red = 0;

				if(pp->Parent)
				{
					pp->Red = 1;
					LastParent = pp->Parent;
					LastDirect = LastParent->Children[0] == pp ? 0 : 1;
					node = pp;
					goto restart;
				}
			}
			else
			{
				if(pp->Children[LastDirect] == pc)
				{
					Rotate(tree, LastParent, LastDirect);
					LastParent = node;
					LastDirect ^= 1;
				}
				Rotate(tree, pp, LastDirect);
				LastParent->Red = 0;
				pp->Red = 1;
			}
		}
	}
	else
	{
		node = CreateNode(element, 0, NULL);

		tree->Root = node;
		tree->LastAccessNode = node;
	}
}
int rbtHasElement(rbtTree_t *tree, void *element) // ret: element ��������� (element == NULL �̂Ƃ� LastAccessNode ���L���Ȃ��) 0 �ȊO
{
	errorCase(!tree);

	if(!element)
		return (int)tree->LastAccessNode;

	return (int)GetNode(tree, element);
}
static rbtNode_t *GetNodeEx(rbtTree_t *tree, void *element)
{
	rbtNode_t *node = element ? GetNode(tree, element) : tree->LastAccessNode;

	errorCase(!node); // ���݂��� element ���� LastAccessNode ���L���łȂ���΂Ȃ�Ȃ��B

	return node;
}
void *rbtGetElement(rbtTree_t *tree, void *element) // element == NULL �̂Ƃ� LastAccessNode ��Ԃ��Belement ��������Ȃ���� error();
{
	errorCase(!tree);
	return GetNodeEx(tree, element)->Element;
}

static rbtNode_t *LastNode;
static int GetNearestLeaf(rbtTree_t *tree, rbtNode_t *node, int direct) // ret: direct ������ Leaf ��������� 0 �ȊO
{
	LastNode = node->Children[direct];

	if(LastNode)
	{
		for(; ; )
		{
			node = LastNode->Children[1 - direct];

			if(!node)
				break;

			LastNode = node;
		}
	}
	return (int)LastNode;
}
void rbtRemoveElement(rbtTree_t *tree, void *element) // element == NULL �̂Ƃ� LastAccessNode ���폜����Belement ��������Ȃ���� error();
{
	rbtNode_t *node;
	rbtNode_t *p;

	errorCase(!tree);
	node = GetNodeEx(tree, element);

	tree->LastAccessNode = NULL;
	tree->FuncRelease(node->Element, tree->ExtraData);

	while(GetNearestLeaf(tree, node, 0) || GetNearestLeaf(tree, node, 1))
	{
		node->Element = LastNode->Element;
		node = LastNode;
	}
	if(!node->Red)
	{
		rbtNode_t *n = node;
		rbtNode_t *pc;
		rbtNode_t *pcc;
		int pcDirect;

restart:
		p = n->Parent;

		if(p)
		{
			pcDirect = p->Children[0] == n ? 1 : 0;
pcReset:
			pc = p->Children[pcDirect];

			if(pc->Red)
			{
				Rotate(tree, p, pcDirect);
				p->Red = 1;
				pc->Red = 0;
				goto pcReset;
			}
			pcc = pc->Children[1 - pcDirect];

			if(pcc && pcc->Red)
			{
				Rotate(tree, pc, 1 - pcDirect);
				pc->Red = 1;
				pcc->Red = 0;
				goto pcReset;
			}
			pcc = pc->Children[pcDirect];

			if(pcc && pcc->Red)
			{
				Rotate(tree, p, pcDirect);
				pc->Red = p->Red;
				p->Red = 0;
				pcc->Red = 0;
			}
			else
			{
				pc->Red = 1;

				if(!p->Red)
				{
					n = p;
					goto restart;
				}
				p->Red = 0;
			}
		}
	}
	p = node->Parent;

	if(p)
	{
		p->Children[p->Children[0] == node ? 0 : 1] = NULL;
	}
	else
	{
		tree->Root = NULL;
	}
	memFree(node);
}

void rbtSeekRootElement(rbtTree_t *tree)
{
	errorCase(!tree);
	errorCase(!tree->Root); // �v�f����ȏ�Ȃ���΂Ȃ�Ȃ��B

	tree->LastAccessNode = tree->Root;
}
void rbtSeekForEndElement(rbtTree_t *tree, int direct) // direct == 0: Left, 1: Right
{
	rbtNode_t *node;
	rbtNode_t *tmpNode;

	errorCase(!tree);
	errorCase(direct != 0 && direct != 1);

	node = tree->Root;

	errorCase(!node); // �v�f����ȏ�Ȃ���΂Ȃ�Ȃ��B

	for(; ; )
	{
		tmpNode = node->Children[direct];

		if(!tmpNode)
		{
			break;
		}
		node = tmpNode;
	}
	tree->LastAccessNode = node;
}
void rbtSeekForLeftEndElement(rbtTree_t *tree)
{
	rbtSeekForEndElement(tree, 0);
}
void rbtSeekForRightEndElement(rbtTree_t *tree)
{
	rbtSeekForEndElement(tree, 1);
}

void rbtSeekElement(rbtTree_t *tree, int direct) // direct == 0: Left, 1: Right
{
	rbtNode_t *node;
	rbtNode_t *tmpNode;

	errorCase(!tree);
	errorCase(direct != 0 && direct != 1);

	node = tree->LastAccessNode;

	errorCase(!node); // LastAccessNode ���L���łȂ���΂Ȃ�Ȃ��B

	tmpNode = node->Children[direct];

	if(tmpNode)
	{
		for(; ; )
		{
			node = tmpNode;
			tmpNode = node->Children[1 - direct];

			if(!tmpNode)
			{
				break;
			}
		}
	}
	else
	{
		for(; ; )
		{
			tmpNode = node->Parent;

			if(!tmpNode || tmpNode->Children[1 - direct] == node)
			{
				break;
			}
			node = tmpNode;
		}
		node = tmpNode;
	}

	// node == NULL �̂Ƃ��A(���[�̃m�[�h���獶 || �E�[�̃m�[�h����E) �ɃV�[�N�����B-> �ʒu���̂܂�

	if(node)
	{
		tree->LastAccessNode = node;
	}
}
void rbtSeekForLeftElement(rbtTree_t *tree)
{
	rbtSeekElement(tree, 0);
}
void rbtSeekForRightElement(rbtTree_t *tree)
{
	rbtSeekElement(tree, 1);
}
