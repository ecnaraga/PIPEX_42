/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garance <garance@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 08:53:13 by garance           #+#    #+#             */
/*   Updated: 2023/09/21 09:56:19 by garance          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

char	**ft_research_path(char **env)
{
	int	i;

	i = -1;
	if (!env)
		return (NULL);
	while (env[++i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
			return (ft_split(env[i] + 5, ':'));
	}
	return (NULL);
}

static int	ft_find_good_path(char **path, char **good_path, char *cmd,
		int accss)
{
	char	*tmp;
	int		i;

	i = 0;
	while (path[i] && accss != 0)
	{
		*good_path = ft_strjoin(path[i], "/");
		if (!good_path)
			return (E_STRJOIN);
		tmp = *good_path;
		*good_path = ft_strjoin(*good_path, cmd);
		if (!*good_path)
			return (E_STRJOIN);
		free(tmp);
		accss = access(*good_path, F_OK | X_OK);
		if (accss == 0)
			return (E_OK);
		free(*good_path);
		*good_path = NULL;
		i++;
	}
	return (E_NO_CMD);
}

int	ft_access_cmd(char **path, char *cmd, char **good_path)
{
	int	accss;

	accss = access(cmd, F_OK | X_OK);
	if (accss == 0)
	{
		*good_path = ft_strdup(cmd);
		if (!*good_path)
			return (E_STRDUP);
		return (E_OK);
	}
	if (!path)
		return (E_NO_CMD);
	return (ft_find_good_path(path, good_path, cmd, accss));
}

void	redef_stdin(char **av, t_pipex p)
{
	int		fd_infile;
	char	*str;

	fd_infile = open(av[1], O_RDONLY);
	if (fd_infile == -1)
	{
		str = ft_strjoin("bash: ", av[1]);
		(perror(str), free(str), ft_exit(&p, p.fd_p[1], -1));
	}
	if (dup2(fd_infile, STDIN_FILENO) == -1)
		(perror("dup2"), ft_exit(&p, p.fd_p[1], fd_infile));
	close(fd_infile);
}

void	redef_stout(char **av, t_pipex p, int *fd_outfile)
{
	char	*str;

	*fd_outfile = open(av[4], O_CREAT | O_TRUNC | O_WRONLY, 0744);
	if (*fd_outfile == -1)
	{
		str = ft_strjoin("bash: ", av[4]);
		(perror(str), free(str), ft_exit(&p, -1, -1));
	}
}
