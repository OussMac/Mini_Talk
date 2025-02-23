/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oimzilen <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 17:22:02 by oimzilen          #+#    #+#             */
/*   Updated: 2025/02/17 17:45:24 by oimzilen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

int	g_old_pid;

void	ft_decrypt(int signum, siginfo_t *info, void *context)
{
	static char	c;
	static int	i;

	(void)context;
	if (g_old_pid != 0 && info->si_pid != g_old_pid)
	{
		c = 0;
		i = 0;
	}
	g_old_pid = info->si_pid;
	c <<= 1;
	if (signum == SIGUSR2)
		c |= 1;
	i++;
	if (i == 8)
	{
		if (c == '\0')
			write(STDOUT_FILENO, "\n", 1);
		else
			write(STDOUT_FILENO, &c, 1);
		c = 0;
		i = 0;
	}
	usleep(37);
	kill(info->si_pid, SIGUSR1);
}

void	server_info(void)
{
	int	serv_pid;

	serv_pid = getpid();
	ft_putstr_fd("The Server Has Started, Server PID Is: [ ", STDOUT_FILENO);
	ft_putnbr_fd(serv_pid, STDOUT_FILENO);
	ft_putstr_fd(" ].\n", STDOUT_FILENO);
}

int	main(int argc, char *argv[])
{
	struct sigaction	sa;

	(void)argc;
	if (argc != 1)
	{
		ft_putstr_fd("Usage : ./server \n", STDOUT_FILENO);
		return (EXIT_FAILURE);
	}
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = ft_decrypt;
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, SIGUSR1);
	sigaddset(&sa.sa_mask, SIGUSR2);
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	server_info();
	while (1)
		pause();
	return (EXIT_SUCCESS);
}
