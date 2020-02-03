/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ambelghi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/23 14:45:32 by ambelghi          #+#    #+#             */
/*   Updated: 2020/02/03 23:37:27 by ambelghi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"
#include <sys/stat.h>
#include <sys/types.h>
#include "libft.h"
#include <sys/ioctl.h>

char	*replace_cr(t_cs_line *cs, char *input)
{
	char	*in;
	char	*spaces;
	char	*oc;
	char	oc2;
	int		len;

	if ((in = input))
	{
		while ((oc = ft_strchr(input, '\n')))
		{
			//oc++;
			oc2 = *oc;
			*oc = '\0';
			if ((len = ft_strlen(input)) > cs->screen.x)
				len = cs->screen.x - len % cs->screen.x;
			else
				len = cs->screen.x - len;
			spaces = ft_strdup(input);
			while (len-- > 0)
			{
				in = spaces;
				spaces = ft_strjoin(spaces, " ");
				ft_strdel(&in);
			}
			in = input;
			*oc = oc2;
			input = ft_strjoin(spaces, oc + 1);
			ft_strdel(&spaces);
			ft_strdel(&in);
			in = ft_strdup(input);
			//	ft_strdel(&oc);
		}
		//	input_master(input, 1);
	}
	return (in);
}

char	*input_master(char *in, int init)
{
	static char	*input = NULL;

	if (init == 1)
	{
		//ft_strdel(&input);
		input = in;
	}
	return (input);
}

char	*get_firstline(t_cs_line *cs)
{
	char		*oc;
	t_line_lst	*tmp;
	int			x;

	oc = NULL;
	if (cs && (tmp = cs->list))
	{
		x = 1;
		if (tmp->next)
			oc = tmp->next->str;
		while (cs->scroll > 0 && (tmp = tmp->next) && x <= cs->scroll)
		{
			x += tmp->nb_lines;
			if (x > cs->scroll)
			{
				x = cs->scroll - (x - tmp->nb_lines);
				return (&tmp->str[x * (cs->screen.x - 1) + 1]);
			}
		}
	}
	return (oc);
}

t_point	trim_input(t_cs_line *cs)
{
	t_point z;
	int		cr;
	int		line;
	int		i;
	int		ew;
	char	*tmp;
	int		i2;
	int		line2;

	z.x = 0;
	z.y = ft_strlen(cs->input);
	if (cs)
	{
		ew = 0;
		i = 0;
		line = 0;
		tmp = cs->input;
		cr = 0;
		i2 = 0;
		int i3 = 0;
		line2 = 0;
		while (tmp[i])
		{
			i3++;
			if (tmp[i] == '\n' || i3 == cs->screen.x
				|| (line == 0 && i3 + cs->min_col >= cs->screen.x))
			{
				i3 = 0;
				line++;
			}
			if ((cs->scroll > 0 && line > cs->scroll && ew == 0))
			{
				char *more = "[...]";
				if (cs->screen.x - cs->min_col < 5)
					more[cs->screen.x - cs->min_col] = '\0';
				ft_putstr_fd(more, cs->tty);
				ft_putstr_fd("\n", cs->tty);
				ew = 1;
				z.x = i + 1;
			}
			if (line - cs->scroll + cs->min_row + 1 > cs->screen.y)
			{
				z.y = i;//i;
				break ;
			}
			i++;
		}
		cs->start = z.x;
		cs->end = z.y;
	}
	return (z);
}

void	print_cmdline(t_cs_line *cs)
{
	//	t_line_lst	*tmp;
	int			i;
	int			row;
	int			z;
	int			x;
	int			start;
	int			end;
	//	char		*tmp2;
	//	int			row_start;
	char		oc;

	oc = 0;
	ft_clear(1);
	if (cs && cs->input && cs->line_col >= 0)
	{
		i = 1;
		tputs(tgoto(tgetstr("cm", NULL), cs->min_col, cs->min_row),
				1, &my_putchar);
		row = 0;
		z = 0;
		i = 0;
		x = 0;
		row = 0;
		t_point z = trim_input(cs);
		start = z.x;
		end = z.y;
		oc = cs->input[cs->line_col];
		cs->input[cs->line_col] = '\0';
		ft_putstr_fd(&cs->input[start], cs->tty);
		get_cs_line_position(&cs->col, &cs->row);
		cs->input[cs->line_col] = oc;
		oc = cs->input[end];
		cs->input[end] = '\0';
		ft_putstr_fd(&cs->input[cs->line_col], cs->tty);
		cs->input[end] = oc;
		tputs(tgoto(tgetstr("cm", NULL), cs->col, cs->row), 1, &my_putchar);
		//move_cs(&cs);
	}
}

void	line_master(t_cs_line *cs, char *input)
{
	char		*tmp;
	char		*insert;
	char		oc;
	int			i;
	int			cr;

	if (input && cs)// && (t = ft_strsplit(input, "\n")))
	{
		if (ft_strlen(cs->input) == cs->line_col)
		{
			tmp = cs->input;
			cs->input = ft_strjoin(cs->input, input);
			ft_strdel(&tmp);
		}
		else
		{
			oc = cs->input[cs->line_col];
			cs->input[cs->line_col] = '\0';
			tmp = cs->input;
			cs->input = ft_strjoin(tmp, input);
			tmp[cs->line_col] = oc;
			insert = cs->input;
			cs->input = ft_strjoin(insert, &tmp[cs->line_col]);
			ft_strdel(&insert);
			ft_strdel(&tmp);
		}
		cs->line_col += ft_strlen(input);
		cs->max_scroll = ft_strlen(cs->input) / cs->screen.x
			- (cs->screen.y - cs->min_row);
		i = 0;
		cr = get_line(cs);
		int	row_prompt = PROMPT_SIZE / cs->screen.x
						+ (PROMPT_SIZE % cs->screen.x > 0 ? 1 : 0);
		int	scroll_add = 0;
		if (cr + cs->min_row >= cs->screen.y && cs->min_row > row_prompt)
		{
			scroll_add = cr + cs->min_row - cs->screen.y + 1;
			while (cs->min_row - scroll_add < 1)
				scroll_add--;
			cs->min_row -= scroll_add;
			tputs(tgoto(tgetstr("SF", NULL), 0, scroll_add), 1, &my_putchar);
		}
		if (cr - cs->scroll + cs->min_row >= cs->screen.y)
			cs->scroll = cr - (cs->screen.y - cs->min_row - 1);
		if (cs->scroll < 0)
			cs->scroll = 0;
	}
}

int		get_cs_linecol(t_cs_line *cs)
{
	int	i;
	int	z;
	int	x;
	int	cr;

	i = 0;
	z = 0;
	cr = 0;
	x = cs->line_col;
	if (cs)
	{
		while (x > 0 && cr == 0)
		{
			z++;
			if (cs->input[--x] == '\n')// || z++ == cs->screen.x)
				cr++;
		}
		int w = z;
		z = 0;
		while (x > 0 && cr == 1)
		{
			z++;
			if (cs->input[--x] == '\n')// || z++ == cs->screen.x)
				cr++;
		}
		z--;
		if (w > 0 && w <= z)
			cs->line_col = x + w;
		else
			cs->line_col = x + z;
		cs->line_col = (cs->line_col < 0 ? 0 : cs->line_col);
		i = cs->line_col;
	}
	return (i);
}

int     get_cs_linecol2(t_cs_line *cs)
{
	int i;
	int z;
	int	w;
	int x;
	int cr;

	i = 0;
	z = 0;
	cr = 0;
	x = cs->line_col;
	if (cs)
	{
		w = 0;
		while (x > 0 && cr == 0)
		{
			w++;
			if (cs->input[--x] == '\n')
				cr++;
		}
		x = cs->line_col;
		w--;
		cr = 0;
		while (x < ft_strlen(cs->input) && cr == 0)
		{
			z++;
			if (cs->input[x++] == '\n')// || z++ == cs->screen.x)
				cr++;
		}
		z = 0;
		while (x < ft_strlen(cs->input) && cr == 1)
		{
			z++;
			if (cs->input[x++] == '\n')// || z++ == cs->screen.x)
				cr++;
		}
		//z--;
		if (w > 0 && w <= z)
			cs->line_col = (x - z) + w;
		else
			cs->line_col = x - 1;
		// if (cr && cs->row == cs->screen.y)
		//     cs->scroll += 1;
		i = cs->line_col;
	}
	return (i);
}

int		get_line(t_cs_line *cs)
{
	int i = 0;
	int cr = 0;
	int i3 = 0;
	int i4 = cs->line_col;
	int	cr2 = 0;

	while (cs->input[i4] && cr == 0)
	{
		i3++;
		if (cs->input[i4] == '\n' || i3 == cs->screen.x)
		{
			i3 = 0;
			cr++;
		}
		i4++;
	}
	i3 = 0;
	cr2 = cr;
	while (i < cs->line_col)
	{
		i3++;
		if (cs->input[i] == '\n' || i3 == cs->screen.x
			|| (cr2 == cr && i3 + cs->min_col >= cs->screen.x ))
		{
			i3 = 0;
			cr++;
		}
		i++;
	}
	return (cr);
}

void	arrow_up(t_cs_line *cs)
{
	//t_line_lst  *lst;
	int	scroll;
	int	cr;

	if (cs) //&& (lst = line_getlst(cs->list, cs->line + 1)))
	{
		scroll = 0;
		scroll = cs->scroll;
		get_cs_linecol(cs);
		cr = get_line(cs);
		if (cr + cs->min_row - cs->scroll  - 1 <= cs->min_row && cs->scroll)
			cs->scroll = cr - (cs->screen.y - cs->min_row - 1);
		if (cs->scroll < 0)
			cs->scroll = 0;
		print_cmdline(cs);
	}
}

void	arrow_down(t_cs_line *cs)
{
	int	scroll;
	int	cr;

	if (cs)// && (lst = line_getlst(cs->list, cs->line + 1)))
	{
		scroll = cs->scroll;
		get_cs_linecol2(cs);
		cr = get_line(cs);
		if (cr - cs->scroll + cs->min_row >= cs->screen.y)
			cs->scroll = cr - (cs->screen.y - cs->min_row - 1);
		if (cs->scroll < 0)
			cs->scroll = 0;
		print_cmdline(cs);
		//move_cs(&cs);	
	}
}

void	arrow_right(t_cs_line *cs)
{
	int	cr;

	if (cs)// && (lst = line_getlst(cs->list, cs->line + 1)))
	{
		if (cs->line_col < ft_strlen(cs->input))
			cs->line_col += 1;
		cr = get_line(cs);
		if (cr - cs->scroll + cs->min_row >= cs->screen.y)
			cs->scroll = cr - (cs->screen.y - cs->min_row - 1);
		if (cs->scroll < 0)
			cs->scroll = 0;
		print_cmdline(cs);
	}	
}

void	arrow_left(t_cs_line *cs)
{
	int	cr;

	if (cs)// && (lst = line_getlst(cs->list, cs->line + 1)))
	{	
		if (cs->line_col > 0)
			cs->line_col -= 1;
		cr = get_line(cs);
		if (cs->col == 0 && cs->row == cs->min_row && cs->scroll)
			cs->scroll -= 1;
		if (cs->scroll < 0)
			cs->scroll = 0;
		//	trim_input(cs);
		print_cmdline(cs);
	}
}

void	space_bar(t_cs_line *cs)
{
	t_line_lst	*tmp;

	if ((tmp = line_getlst(cs->list, cs->line)))
	{
		move_cs(&cs);
	}
}

void	home_key(t_cs_line *cs)
{
	int	i;
	int	cr;

	if (cs)
	{
		i = cs->line_col;
		while (i > 0)
		{
			if (cs->input[--i] == '\n')
				break ;
		}
		i++;
		cs->line_col = i;
		cr = get_line(cs);
        if (cr + cs->min_row - cs->scroll  - 1 <= cs->min_row && cs->scroll)
            cs->scroll = cr - (cs->screen.y - cs->min_row - 1);
        if (cs->scroll < 0)
            cs->scroll = 0;
		print_cmdline(cs);
		//move_cs(&cs);
	}
}

void	end_key(t_cs_line *cs)
{
	t_line_lst  *tmp;

	if (cs && (tmp = line_getlst(cs->list, cs->line + 1)))
	{
		cs->line_col = tmp->len;
		move_cs(&cs);
	}
}

void	maj_arrow_down(t_cs_line *cs)
{
	t_line_lst  *tmp;

	if (cs && (tmp = line_getlst(cs->list, cs->line + 1)))
	{
		if (cs->line < cs->nb_lines && tmp->next)
		{
			cs->line += 1;
			if (cs->line_col % cs->screen.x > tmp->next->len)
				cs->line_col = tmp->next->len;
			else
				cs->line_col = cs->line_col % cs->screen.x;
		}
		move_cs(&cs);
	}
}

void	maj_arrow_up(t_cs_line *cs)
{
	t_line_lst  *tmp;

	if (cs && (tmp = line_getlst(cs->list, cs->line + 1)))
	{
		if (cs->line > 0 && tmp->prev)
		{
			if (cs->line_col % cs->screen.x > tmp->prev->len)
				cs->line_col = tmp->prev->len;
			else
				cs->line_col = cs->line_col % cs->screen.x;
			cs->line -= 1;
		}
		move_cs(&cs);
	}
}
