typedef struct rbtNode_st // 内部でのみ使用
{
	void *Element;
	int Red;
	struct rbtNode_st *Parent;
	struct rbtNode_st *Children[2];
}
rbtNode_t;

typedef struct rbtTree_st
{
	int (*FuncComp)(void *, void *, void *);
	void (*FuncRelease)(void *, void *);
	void *ExtraData;
	rbtNode_t *Root;
	rbtNode_t *LastAccessNode;
}
rbtTree_t;

rbtTree_t *rbtCreateTree(int (*funcComp)(void *, void *, void *), void (*funcRelease)(void *, void *), void *extraData);

void rbtAddElement(rbtTree_t *tree, void *element);
int rbtHasElement(rbtTree_t *tree, void *element);
void *rbtGetElement(rbtTree_t *tree, void *element);
void rbtRemoveElement(rbtTree_t *tree, void *element);
