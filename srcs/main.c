/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_started.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ambelghi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/15 13:21:45 by ambelghi          #+#    #+#             */
/*   Updated: 2020/01/10 19:45:52 by ambelghi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include "libft.h"
#include "minishell.h"
#include <signal.h>

void	msh_level(t_env_list *list)
{
	int i;

	i = ft_atoi(get_env(list, "MSHLVL")) + 1;
	if (get_env(list, "MSHLVL"))
		set_env(list, "MSHLVL", ft_itoa(i));
	else
		env_lstaddb(list, env_lstnew(ft_strdup("MSHLVL"), ft_itoa(i)));
}

int		read_me(t_env_list *list)
{
	int		i;
	char	*line;
	char	**params;

	i = 1;
	line = NULL;
	while (i)
	{
		ft_putstr_fd("minishell-1.0$ ", ttyslot());
		if ((i = get_next_line(ttyslot(), &line)) == -1)
			exit(EXIT_FAILURE);
		if ((params = get_params(line, list)))
		{
			minishell_launch(&list, params);
			free_tab(params);
		}
		if (get_interrupt(0, 0) == 1)
		{
			ft_putstr("\n");
			get_interrupt(1, 0);
		}
		ft_strdel(&line);
	}
	return (1);
}

int		main(int ac, char **av, char **envp)
{
	char		**env;
	t_env_list	*list;

	env = NULL;
	if (isatty(ttyslot()) && ((env = envp) || !env))
	{
		ac = 0;
		av = NULL;
		list = env_tolist(envp);
		msh_level(list);
		signal(SIGINT, handle_sigint);
		read_me(list);
		ft_putstr("exit\n");
		env_lstfree(&list);
		return (0);
	}
	return (0);
}
