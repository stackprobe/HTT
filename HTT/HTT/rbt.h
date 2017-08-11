typedef struct rbtNode_st // “à•”‚Å‚Ì‚ÝŽg—p
{
	void *Element;
	boolean Red;
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
void rbtReleaseTree(rbtTree_t *tree);

void rbtAddElement(rbtTree_t *tree, void *element);
int rbtHasElement(rbtTree_t *tree, void *element);
void *rbtGetElement(rbtTree_t *tree, void *element);
void rbtRemoveElement(rbtTree_t *tree, void *element);

void rbtSeekRootElement(rbtTree_t *tree);
void rbtSeekForEndElement(rbtTree_t *tree, int direct);
void rbtSeekForLeftEndElement(rbtTree_t *tree);
void rbtSeekForRightEndElement(rbtTree_t *tree);

void rbtSeekElement(rbtTree_t *tree, int direct);
void rbtSeekForLeftElement(rbtTree_t *tree);
void rbtSeekForRightElement(rbtTree_t *tree);
