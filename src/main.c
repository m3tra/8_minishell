/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fporto <fporto@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/23 21:29:25 by fporto            #+#    #+#             */
/*   Updated: 2022/11/25 16:02:46 by fporto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_global	g_global;

// static char	*last_dir(void)
// {
// 	size_t	len;

// 	len = ft_strlen(g_global.cwd);
// 	while (len > 0 && g_global.cwd[len - 1] != '/')
// 		len--;
// 	return (ft_strdup(g_global.cwd + len));
// }

/*
*	Ctrl+C handler
*/
static void	sigint_action(int signal)
{
	rl_replace_line("", signal);
	printf("\n");
	rl_on_new_line();
	rl_redisplay();
}

static void	read_command(void)
{
	char	*tmp;
	printf(CLR_BLUE"[%s]\n"CLR_RST, g_global.cwd);
	tmp = ft_strdup(PROMPT);
	while ("drip")
	{
		g_global.input = readline(tmp);
		if (g_global.input && ft_strlen(g_global.input) != 0)
			break ;
		else if (g_global.input)
			free(g_global.input);
		if (!g_global.input || g_global.exit)
		{
			free(tmp);
			free_global(NULL);
		}
	}
	free(tmp);
	if (*g_global.input)
		add_history(g_global.input);
	free_arr(g_global.argv);
	g_global.argv = split_args(g_global.input);
}

// void	handle_io(void)
// {
// 	char	*fileIn;
// 	char	*fileOut;

// 	// Save STDIN/STDOUT
// 	int		originalFdIn = dup(STDIN_FILENO);
// 	int		originalFdOut = dup(STDOUT_FILENO);

// 	// Set the initial input
// 	int		fdIn;
// 	// If there is a specified input file ( < *.* ) open it
// 	// Else use default input
// 	if (fileIn)
// 		fdIn = open(fileIn, O_RDONLY);
// 	else
// 		fdIn = dup(originalFdIn);

// 	int		ret;
// 	int		fdOut;

// 	int		nSimpleCommands;
// 	t_simple_cmd	**simpleCmds;
// 	t_simple_cmd	*currSimpleCmd;

// 	nSimpleCommands = g_global.full_cmd.n_simple_cmds;
// 	simpleCmds = g_global.full_cmd.simpleCmds;
// 	for (int i = 0; i < nSimpleCommands; i++)
// 	{
// 		currSimpleCmd = simpleCmds[i];
// 		//redirect input
// 		dup2(fdIn, STDIN_FILENO);
// 		close(fdIn);
// 		//setup output
// 		if (i == nSimpleCommands - 1)
// 		{
// 			// Last simple command
// 			// If there is a specified output file ( > *.* ) open it
// 			// Else use default output
// 			if (fileOut)
// 				fdOut = open(fileOut, O_RDWR | O_CREAT, 0777);
// 			else
// 				fdOut = dup(originalFdOut);
// 		}
// 		else
// 		{
// 			// Not last simple command
// 			//create pipe
// 			int fdPipe[2];
// 			pipe(fdPipe);
// 			fdOut = fdPipe[1];
// 			fdIn = fdPipe[0];
// 		}// if/else
// 		// Redirect output
// 		dup2(fdOut, STDOUT_FILENO);
// 		close(fdOut);

// 		if (is_builtin(currSimpleCmd->args[0]))
// 			builtin(currSimpleCmd->args[0]);
// 		else
// 		{
// 			ret = not_builtin(currSimpleCmd->args[0]);
// 			if (!ret)
// 				free_global(CLR_RED"Command not found"CLR_RST);
// 		}
// 		// // Create child process
// 		// ret = fork();
// 		// if (ret == 0) {
// 		// 	execvp(scmd[i].args[0], scmd[i].args);
// 		// 	perror("execvp");
// 		// 	_exit(1);
// 		// }
// 	} //  for
// 	//restore in/out defaults
// 	dup2(originalFdIn, STDIN_FILENO);
// 	dup2(originalFdOut, STDOUT_FILENO);
// 	close(originalFdIn);
// 	close(originalFdOut);
// 	// Wait for last command
// 	if (ret && !g_global.background)
// 		waitpid(ret, NULL, NULL);
// }

int	main(int argc, char **argv, char **env)
{
	(void)argc;
	(void)argv;
	char			*cmd;

	t_full_cmd		*t_cmd;
	t_simple_cmd	*s_cmd;

	s_cmd = malloc(sizeof(t_simple_cmd));
	if (!s_cmd)
		return (1);
	s_cmd->args = malloc(sizeof(char *));
	if (!s_cmd->args)
		return (2);
	t_cmd = malloc (sizeof(t_full_cmd));
	if (!t_cmd)
		return (3);
	t_cmd->curr_simple_cmd = s_cmd;
	g_global.full_cmd = t_cmd;

	global_init(env);
	signal(SIGQUIT, SIG_IGN);		// Ctrl + \		//
	signal(SIGINT, sigint_action);	// Ctrl + C
	while ("swag")
	{
		read_command();
		cmd = g_global.argv[0];
		s_cmd->args[0] = cmd;
		if (!parse(g_global.input))
			continue ;
		if (!ft_strcmp(cmd, "cd")){
			cd();
			continue ;
		}
		else if (!ft_strcmp(cmd, "exit"))
			break ;
		if (!builtin(s_cmd))
			not_builtin(s_cmd);
		// printf("test\n");
		wait(NULL);
	}

	free(s_cmd->args);
	free(s_cmd);

	free_global(NULL);
}
