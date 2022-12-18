/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmiseiki <vmiseiki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 12:12:14 by vmiseiki          #+#    #+#             */
/*   Updated: 2021/08/06 14:10:41 by vmiseiki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include "get_next_line.h"

char	*ft_if_new_line(char *line, char *buffer, char *ptr)
{
	size_t	n;
	size_t	i;
	char	*temp1;

	temp1 = ft_substr(buffer, 0, ptr - buffer + 1);
	if (line == NULL)
		line = ft_strdup(temp1);
	else
		line = ft_strjoin(line, temp1);
	free(temp1);
	i = 0;
	n = ptr - buffer + 1;
	while (n < BUFFER_SIZE)
	{
		buffer[i] = buffer[n];
		n++;
		i++;
	}
	buffer[i] = '\0';
	return (line);
}

char	*get_line(int fd, char *line, char *buffer)
{
	size_t	size;

	line = NULL;
	if (fd < 0 || read(fd, buffer, 0) < 0 || BUFFER_SIZE < 1)
		return (NULL);
	if (!*buffer)
		buffer[read (fd, buffer, BUFFER_SIZE)] = '\0';
	if (ft_strlen(buffer) == 0)
		return (NULL);
	if (ft_strchr(buffer, '\n'))
		return (ft_if_new_line(line, buffer, ft_strchr(buffer, '\n')));
	line = ft_strdup(buffer);
	size = read (fd, buffer, BUFFER_SIZE);
	buffer[size] = '\0';
	while (!ft_strchr(buffer, '\n') && size == BUFFER_SIZE)
	{
		line = ft_strjoin(line, buffer);
		size = read (fd, buffer, BUFFER_SIZE);
		buffer[size] = '\0';
	}	
	if (ft_strchr(buffer, '\n'))
		return (ft_if_new_line(line, buffer, ft_strchr(buffer, '\n')));
	line = ft_strjoin(line, buffer);
	buffer[0] = '\0';
	return (line);
}

t_list	*new_list_el(int fd)
{
	t_list	*new;

	new = (t_list *)malloc(sizeof(t_list));
	if (!new)
		return (NULL);
	new -> fd = fd;
	new -> buffer[0] = '\0';
	new -> next = NULL;
	return (new);
}

void	ft_delete_list_node(t_list **lst, int fd)
{
	t_list	*temp;
	t_list	*pre_lst_elem;

	temp = *lst;
	if (temp -> fd == fd && temp != NULL)
	{
		*lst = (*lst)-> next;
		free (temp);
	}
	else
	{
		while (temp != NULL && temp -> fd != fd)
		{
			pre_lst_elem = temp;
			temp = temp -> next;
		}
		if (temp -> fd == fd)
		{
			pre_lst_elem -> next = temp -> next;
			free(temp);
		}
	}		
}

char	*get_next_line(int fd)
{
	static t_list	*head;
	t_list			*temp;
	char			*line;

	if (head == NULL)
		head = new_list_el(fd);
	temp = head;
	while (temp -> fd != fd)
	{
		if (temp -> next == NULL)
			temp -> next = new_list_el(fd);
		temp = temp -> next;
	}
	line = get_line(temp -> fd, NULL, temp -> buffer);
	if (temp -> buffer[0] == '\0')
		ft_delete_list_node(&head, fd);
	return (line);
}
