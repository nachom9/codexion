#include "codexion.h"

t_coder	*ft_lstnew(void *content, int id)
{
	t_coder	*node;

	node = malloc(sizeof(t_coder));
	if (!node)
		return (NULL);
	node->id = id;
	return (node);
}
