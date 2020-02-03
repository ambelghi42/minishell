/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ambelghi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/13 16:11:17 by ambelghi          #+#    #+#             */
/*   Updated: 2020/02/03 21:16:55 by ambelghi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_select.h"

void	cs_setter(int nb_elem, int max_col, t_cs_line *cs, struct winsize size)
{
	max_col = 0;
	cs = NULL;
	size.ws_row = size.ws_row;
	/*
	   if (cs && cs->screen.x < cs->max_len)
	   cs->max_len = cs->screen.x - 1;
	   if (cs && nb_elem >= size.ws_row - cs->min_row)
	   {
	   cs->max_row = cs->min_row - 1
	   + (nb_elem / max_col + (nb_elem % max_col == 0 ? 0 : 1));
	   cs->max_row = (cs->max_row > size.ws_row ? size.ws_row - 1
	   : cs->max_row);
	   cs->lines_col = nb_elem / max_col + (nb_elem % max_col == 0 ? 0 : 1);
	   }
	   else if (cs)
	   {
	   cs->max_row = cs->min_row - 1 + cs->nb_elem;
	   cs->lines_col = cs->nb_elem;
	   }
	   if (cs && cs->lines_col == 0)
	   cs->lines_col = 1;
	   if (cs)
	   cs->max_row = cs->col_command / cs->screen.x;
	 */
	nb_elem = 0;
}

void	cs_set(t_line_lst *list, int max_len)
{
	int				z;
	int				max_col;
	struct winsize	size;
	t_cs_line			*cs;

	if ((cs = cs_master(NULL, 0)))
	{
		list = NULL;
		ioctl(0, TIOCGWINSZ, &size);
		max_col = (max_len > 0 && max_len < size.ws_col
				? size.ws_col / (max_len + 1) : 1);
		cs->screen.x = size.ws_col;
		cs->screen.y = size.ws_row;
		cs->tty = ttyslot();
		z = ft_strlen(cs->input);
		cs->total_lines = z / cs->screen.x + (z % cs->screen.x > 0 ? 1 : 0);
		cs->max_scroll = cs->total_lines - (cs->screen.y - cs->min_row);
	}
}

int		get_total_lines(t_cs_line *cs)
{
	t_line_lst  *lst;
	int         i;
	int         row;

	i = 0;
	if ((lst = cs->list))
	{
		row = cs->min_row;
		row += cs->line_col / cs->screen.x;
		while ((lst = lst->next))
		{
			lst->len = ft_strlen(lst->str);
			if (lst->len / cs->screen.x > 0)
				row += lst->len / cs->screen.x + (lst->len % cs->screen.x > 0
						? 1 : 0);
			else
				row += 1;
		}
		i = row;
	}
	return (i);
}

int		get_row(t_cs_line *cs2)
{
	//	t_line_lst	*lst;
	int			i;
	//	int			row;
	int			z;
	int			oc;
	char		*ocu;
	t_cs_line	*cs;

	cs2 = NULL;
	i = 0;
	if ((cs = cs_master(NULL, 0)) && cs->input)
	{
		oc = (int)cs->input[cs->line_col];
		cs->input[cs->line_col] = '\0';
		ocu = cs->input;
		while ((ocu = ft_strchr(ocu, '\n')))
		{
			ocu++;
			i++;
		}
		z = ft_strlen(cs->input) / cs->screen.x + i;
		cs->input[cs->line_col] = (char)oc;
		return (cs->min_row + z);
	}
	return (cs ? cs->min_row : i);
}

void	move_cs(t_cs_line **cs)
{
	t_cs_line	*tmp;

	if (cs && (tmp = *cs) && tmp->input)
	{
		int cr = 0;
		int i3 = 0;
		int i = 0;
		while (tmp->input[0] && i < tmp->line_col)
		{
			i3++;
			if (tmp->input[i] == '\n' || i3 == tmp->screen.x)
			{
				i3 = 0;
				cr++;
			}
			i++;
		}
		//	tmp->col = i3;
		//	tmp->row = tmp->min_row + cr - tmp->scroll;
	//	tputs(tgoto(tgetstr("cm", NULL), tmp->min_col, tmp->min_row), 1, &my_putchar);
		char oc = tmp->input[tmp->end];
		if (tmp->line_col > 0)// && tmp->row != tmp->screen.y && tmp->col != tmp->screen.x)
		{
			oc = tmp->input[tmp->line_col];
			tmp->input[tmp->line_col] = '\0';
			//ft_putstr_fd(&tmp->input[tmp->start], tmp->tty);
	//		get_cs_line_position(&tmp->col, &tmp->row);
			tmp->input[tmp->line_col] = oc;
/*			ft_putstr_fd(ft_itoa(tmp->start), tmp->tty);
			ft_putstr_fd(" ", tmp->tty);
			ft_putstr_fd(ft_itoa(tmp->line_col), tmp->tty);
		*/}
/*		else
		{
			tmp->input[tmp->end] = '\0';
			ft_putstr_fd(&tmp->input[tmp->start], tmp->tty);
			tmp->input[tmp->end] = oc;
			if (tmp->line_col == 0)
				tputs(tgoto(tgetstr("cm", NULL), tmp->min_col, tmp->min_row), 1, &my_putchar);
		//	get_cs_line_position(&tmp->col, &tmp->row);
		}
	*/
	}
}

t_cs_line	*cs_master(t_line_lst *list, int init)
{
	static t_cs_line	cs = {0};

	if (init == 1)
	{
		get_cs_line_position(&cs.min_col, &cs.min_row);
		cs.col = 0;
		cs.row = cs.min_row;
		cs.list = list;
		cs.scroll = 0;
		cs.line_col = 0;
		cs.max_scroll = 0;
		cs.input = NULL;
		cs.list = line_lstnew(NULL, 0);
		//	line_lstaddb(cs.list, line_lstnew(NULL, 0));
	}
	return (&cs);
}
