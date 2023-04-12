#include <string.h>
#include <stdio.h>
#include <stdlib.h>
/**
 * struct shash_node_s - Node of a sorted hash table
 *
 * @key: The key, string
 * The key is unique in the HashTable
 * @value: The value corresponding to a key
 * @next: A pointer to the next node of the List
 * @sprev: A pointer to the previous element of the sorted linked list
 * @snext: A pointer to the next element of the sorted linked list
 */
typedef struct shash_node_s
{
     char *key;
     char *value;
     struct shash_node_s *next;
     struct shash_node_s *sprev;
     struct shash_node_s *snext;
} shash_node_t;

/**
 * struct shash_table_s - Sorted hash table data structure
 *
 * @size: The size of the array
 * @array: An array of size @size
 * Each cell of this array is a pointer to the first node of a linked list,
 * because we want our HashTable to use a Chaining collision handling
 * @shead: A pointer to the first element of the sorted linked list
 * @stail: A pointer to the last element of the sorted linked list
 */
typedef struct shash_table_s
{
     unsigned long int size;
     shash_node_t **array;
     shash_node_t *shead;
     shash_node_t *stail;
} shash_table_t;
unsigned long int hash_djb2(const unsigned char *str)
{
    unsigned long int hash;
    int c;

    hash = 5381;
    while ((c = *str++))
    {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return (hash);
}
unsigned long int key_index(const unsigned char *key, unsigned long int size)
{
	if (key == NULL || size == 0)
		return (0);
	else
		return (hash_djb2(key) % size);
}
shash_table_t *shash_table_create(unsigned long int size)
{
	shash_table_t *ht;
	unsigned long int a;
	
	if (size == 0)
		return (NULL);
	ht = malloc(sizeof(shash_table_t));
	if (ht == NULL)
		return (NULL);
	ht->size == size;
	ht->array = malloc(sizeof(shash_node_t));
	if (ht->array == NULL)
		return (NULL);
	for (a = 0; a < ht->size; a++)
	{
		ht->array[a] = NULL;
	}
	ht->shead = ht->stail = NULL;
	return (ht);
}
int shash_table_set(shash_table_t *ht, const char *key, const char *value)
{	shash_node_t *temp, *new;
	unsigned long int index = key_index((const unsigned char *)key, ht->size);

	if (ht == NULL || key == NULL || *key == '\0' || value == NULL)
		return (0);
	temp = ht->shead;
	while (temp)
	{
		if (strcmp(temp->key, key) == 0)
		{
			free(temp->value);
			temp->value = strdup(value);
			return (1);
		}
		temp = temp->snext;
	}
	new = malloc(sizeof(shash_node_t));
	if (new == NULL)
		return (0);
	new->key = strdup(key);
	new->value = strdup(value);
	new->next = ht->array[index];
	ht->array[index] = new;
	new->snext = NULL;
	new->sprev = NULL;
	if (ht->shead == NULL)
	{
		ht->shead = new;
		ht->stail = new;
		return (1);
	}
	else if (strcmp(ht->shead->key, key) > 0)
	{
                new->snext = ht->shead;
                ht->shead->sprev = new;
		ht->shead = new;
		return (1);
	}
	else
	{
		temp = ht->stail;
		if (strcmp(temp->key, key) < 0)
		{
			temp->snext = new;
			new->sprev = temp;
			ht->stail = new;
			return (1);
		}
		else
		{
			while (temp->sprev != NULL && strcmp(temp->key, key) > 0)
				temp = temp->sprev;
			new->sprev = temp;
			new->snext = temp->snext;
			temp->snext->sprev = new;
			temp->snext = new;
			return (1);
		}
	}
	return (0);
}/**shash_node_t *new, *tmp;
	char *value_copy;
	unsigned long int index;

	if (ht == NULL || key == NULL || *key == '\0' || value == NULL)
		return (0);

	value_copy = strdup(value);
	if (value_copy == NULL)
		return (0);

	index = key_index((const unsigned char *)key, ht->size);
	tmp = ht->shead;
	while (tmp)
	{
		if (strcmp(tmp->key, key) == 0)
		{
			free(tmp->value);
			tmp->value = value_copy;
			return (1);
		}
		tmp = tmp->snext;
	}

	new = malloc(sizeof(shash_node_t));
	if (new == NULL)
	{
		free(value_copy);
		return (0);
	}
	new->key = strdup(key);
	if (new->key == NULL)
	{
		free(value_copy);
		free(new);
		return (0);
	}
	new->value = value_copy;
	new->next = ht->array[index];
	ht->array[index] = new;

	if (ht->shead == NULL)
	{
		new->sprev = NULL;
		new->snext = NULL;
		ht->shead = new;
		ht->stail = new;
	}
	else if (strcmp(ht->shead->key, key) > 0)
	{
		new->sprev = NULL;
		new->snext = ht->shead;
		ht->shead->sprev = new;
		ht->shead = new;
	}
	else
	{
		tmp = ht->shead;
		while (tmp->snext != NULL && strcmp(tmp->snext->key, key) < 0)
			tmp = tmp->snext;
		new->sprev = tmp;
		new->snext = tmp->snext;
		if (tmp->snext == NULL)
			ht->stail = new;
		else
			tmp->snext->sprev = new;
		tmp->snext = new;
	}

	return (1);
}*/
void shash_table_print(const shash_table_t *ht)
{
	shash_node_t *node;
	int k = 0;

	if (ht == NULL)
		return;

	node = ht->shead;
	printf("{");
	while (node)
	{
		if (k == 1)
			printf(", ");
		printf("'%s': '%s'", node->key, node->value);
		node = node->snext;
		k = 1;
	}
	printf("}\n");
}
char *shash_table_get(const shash_table_t *ht, const char *key)
{
}
void shash_table_print_rev(const shash_table_t *ht)
{
	shash_node_t *node;

	int k = 0;
	if (ht == NULL)
		return;

	node = ht->stail;
	printf("{");
	while (node)
	{
		if (k == 1)
			printf(", ");
		printf("'%s': '%s'", node->key, node->value);
		node = node->sprev;
		k = 1;
	}
	printf("}\n");
}
int main(void)
{
    shash_table_t *ht;

    ht = shash_table_create(1024);
    shash_table_set(ht, "y", "0");
    shash_table_print(ht);
    shash_table_set(ht, "j", "1");
    shash_table_print(ht);
    shash_table_set(ht, "c", "2");
    shash_table_print(ht);
    shash_table_set(ht, "b", "3");
    shash_table_print(ht);
    shash_table_set(ht, "z", "4");
    shash_table_print(ht);
    shash_table_set(ht, "n", "5");
    shash_table_print(ht);
    shash_table_set(ht, "a", "6");
    shash_table_print(ht);
    shash_table_set(ht, "m", "7");
    shash_table_print(ht);
    shash_table_print_rev(ht);
    /**shash_table_delete(ht);*/
    return (EXIT_SUCCESS);
}
