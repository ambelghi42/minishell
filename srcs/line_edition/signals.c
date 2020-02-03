/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ambelghi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/13 14:27:55 by ambelghi          #+#    #+#             */
/*   Updated: 2020/02/03 23:48:03 by ambelghi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <term.h>
#include <termios.h>
#include <sys/ioctl.h>
#include "ft_select.h"
#include <unistd.h>
#include <stdlib.h>

void	size_handler(int sig)
{
	t_cs_line			*cs;
	//struct winsize	ws;

	if (sig == SIGWINCH && (cs = cs_master(NULL, 0)))
	{
		ft_clear(1);
		cs_set(NULL, 0);
		get_cs_line_position(&cs->min_col, &cs->min_row);
	/*	ioctl(cs->tty, TIOCGWINSZ, &ws);
		cs_set(cs->list, cs->max_len);
		ft_clear();
		if (scroll_nb(cs) > 0 && cs->scroll < cs->max_scroll)
			cs->scroll += scroll_nb(cs);
		else if (scroll_nb(cs) > 0)
			cs->scroll -= (cs->lines_col
					* (cs->col / (cs->max_len + 1))
					+ cs->scroll + 1) - cs->elem;
		if (cs->scroll > cs->max_scroll)
			cs->scroll = cs->max_scroll;
		if (cs->scroll < 0)
			cs->scroll = 0;
		print_col(cs->list, 1);
		*/
	int	cr = get_line(cs);
    cs->scroll = cr - (cs->screen.y - cs->min_row - 1);
	print_cmdline(cs);
//		move_cs(&cs);
	}
}

void	pause_handler(int sig)
{
	t_cs_line	*cs;

//	if (sig == SIGCONT && (cs = cs_master(NULL, 0)))
//		term_init(2, cs->av);
	if ((sig == SIGTSTP || sig == SIGTTIN || sig == SIGTTOU || sig == SIGSTOP)
		&& (cs = cs_master(NULL, 0)))
	{
//		term_init(0, cs->av);
		signal(SIGTSTP, SIG_DFL);
		signal(SIGTTIN, SIG_DFL);
		signal(SIGTTOU, SIG_DFL);
		signal(SIGSTOP, SIG_DFL);
		ioctl(cs->tty, TIOCSTI, "\x1A");
	}
}

void	sig_handler(int sig)
{
	if (sig >= 0)
	{
		term_init(0, NULL);
		exit(1);
	}
}
