#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

int	err(char *str)
{
	while(*str)
		write(2, str++, 1);
	return (1);
}

int	ft_cd(char **argv, int i)
{
	if (i != 2)
		return err("error: cd: bad arguments\n");
	if (chdir(argv[1]) == -1)
		return err("error: cd: cannot change directory to "), err(argv[1]), err("\n");
	return (0);
}

int	ft_exec(char **argv, char **env, int i)
{
	int	status;
	int	pid;
	int fd[2];
	int	is_pipe;

	is_pipe = argv[i] && !strcmp(argv[i], "|");
	if (is_pipe && pipe(fd) == -1)
		return err("error: fatal\n");
	pid = fork();
	if (!pid)
	{
		argv[i] = 0;
		if (is_pipe && (dup2(fd[1], 1) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1))
			return err("error: fatal \n");
		execve(*argv, argv, env);
		return err("error: cannot execute "), err(*argv), err("\n");
	}
	waitpid(pid, &status, 0);
	if (is_pipe && (dup2(fd[0], 0) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1))
		return err("error: fatal\n");
	return WIFEXITED(status) && WEXITSTATUS(status);
}

int	main(int argc, char **argv, char **env)
{
	int	status;
	int i;

	i = 0;
	status = 0;
	if (argc > 1)
	{
		while(argv[i] && argv[++i])
		{
			argv += i;
			i = 0;
			while(argv[i] && strcmp(argv[i], "|") && strcmp(argv[i], ";"))
				i++;
			if (!strcmp(*argv, "cd"))
				status = ft_cd(argv, i);
			else if (i)
				status = ft_exec(argv, env, i);
		}
	}
	return (status);
}
