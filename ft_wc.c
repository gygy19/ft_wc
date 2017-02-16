/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wc.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguyet <jguyet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/15 21:52:50 by jguyet            #+#    #+#             */
/*   Updated: 2016/05/16 00:31:43 by jguyet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct		s_flags
{
		int			flags[5];
		char		sflags[5];
		int			cflags;
		int			allflags[5];
}					t_flags;

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	ft_putstr(char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	write(1, s, i);
}

void	ft_putnbr(int nbr)
{
	if (nbr >= 10)
	{
		ft_putnbr(nbr / 10);
		ft_putchar((nbr % 10) + 48);
	}
	else
		ft_putchar(nbr + 48);
}

void	read_a_count(t_flags *fla, int fd)
{
	char	buffer[1024 + 1];
	short	ret;
	short	i;
	int		work;

	work = 0;
	while ((ret = read(fd, buffer, 1024)) > 0)
	{
		i = 0;
		buffer[ret] = '\0';
		while (buffer[i])
		{
			if (buffer[i] == ' ' || buffer[i] == '\t' || buffer[i] == '\n'\
				|| buffer[i] == '\r' || buffer[i] == '\f' || buffer[i] == '\v')
			{
				work = 0;
				fla->flags[0]++;
			}
			else if (work == 0)
			{
				fla->flags[2]++;
				fla->flags[0]++;
				work = 1;
			}
			else
				fla->flags[0]++;
			if (buffer[i] == '\n')
				fla->flags[1]++;
			i++;
		}
	}
}

void	print_space(void)
{
	ft_putstr("       ");
}

void	print_flags(t_flags *fla)
{
	if (fla->cflags & 512)
	{
		print_space();
		ft_putstr("\033[38;5;032m");
		if (fla->cflags == 512)
			ft_putstr("Line [");
		ft_putnbr(fla->flags[1]);
		if (fla->cflags == 512)
			ft_putstr("]");
	}
	if (fla->cflags & 256)
	{
		print_space();
		ft_putstr("\033[00m\033[33m");
		if (fla->cflags == 256)
			ft_putstr("Word [");
		ft_putnbr(fla->flags[2]);
		if (fla->cflags == 256)
			ft_putstr("]");
	}
	if (fla->cflags & 128 || fla->cflags & 1024)
	{
		print_space();
		ft_putstr("\033[00m\033[32m");
		if (fla->cflags == 128 || fla->cflags == 1024)
			ft_putstr("Caractere [");
		ft_putnbr(fla->flags[0]);
		if (fla->cflags == 128 || fla->cflags == 1024)
			ft_putstr("]");
	}
	ft_putstr("\033[00m");
}

void	print_Total(t_flags *fla)
{
	if (fla->cflags & 512)
	{
		print_space();
		ft_putstr("\033[38;5;032m");
		if (fla->cflags == 512)
			ft_putstr("Total Line [");
		ft_putnbr(fla->flags[1]);
		if (fla->cflags == 512)
			ft_putstr("]");
	}
	if (fla->cflags & 256)
	{
		print_space();
		ft_putstr("\033[00m\033[33m");
		if (fla->cflags == 256)
			ft_putstr("Total Word [");
		ft_putnbr(fla->flags[2]);
		if (fla->cflags == 256)
			ft_putstr("]");
	}
	if (fla->cflags & 128 || fla->cflags & 1024)
	{
		print_space();
		ft_putstr("\033[00m\033[32m");
		if (fla->cflags == 128 || fla->cflags == 1024)
			ft_putstr("Total Caractere [");
		ft_putnbr(fla->flags[0]);
		if (fla->cflags == 128 || fla->cflags == 1024)
			ft_putstr("]");
	}
	ft_putstr("\033[00m");
}

void	print_credit(int error)
{
	ft_putstr("ft_wc: illegal option -- ");
	ft_putchar(error);
	ft_putstr("\nusage: ft_wc [-clmw] [file ...]\n");
}

int		have_good_flags(t_flags *fla, char *s)
{
	int i;

	if (s[0] != '-')
		return ('-');
	i = 1;
	while (s[i])
	{
		if (s[i] != 'c' && s[i] != 'w' && s[i] != 'l' && s[i] != 'm')
			return (s[i]);
		if (s[i] == 'c' && !(fla->cflags & 128))
			fla->cflags += 128;
		else if (s[i] == 'w' && !(fla->cflags & 256))
			fla->cflags += 256;
		else if (s[i] == 'l' && !(fla->cflags & 512))
			fla->cflags += 512;
		else if (s[i] == 'm' && !(fla->cflags & 1024))
			fla->cflags += 1024;
		i++;
	}
	return (0);
}

int		main(int argc, char **argv)
{
	t_flags		*fla;
	int			error;
	int			fd;
	int			i;

	fd = 0;
	i = 2;
	if (!(fla = (t_flags *)malloc(sizeof(t_flags))))
		return (-1);
	fla->flags[0] = 0;
	fla->flags[1] = 0;
	fla->flags[2] = 0;
	fla->allflags[0] = 0;
	fla->allflags[1] = 0;
	fla->allflags[2] = 0;
	fla->cflags = 0;
	if (argc > 1)
	{
		if ((error = have_good_flags(fla, argv[1])) == 0)
		{
			if (argc > 2)
			{
				while (argv[i])
				{
					if ((fd = open(argv[i], O_RDWR)))
					{
						fla->flags[0] = 0;
						fla->flags[1] = 0;
						fla->flags[2] = 0;
						read_a_count(fla, fd);
						print_flags(fla);
						ft_putstr(" ");
						ft_putstr(argv[i]);
						ft_putstr("\n");
						fla->allflags[0] += fla->flags[0];
						fla->allflags[1] += fla->flags[1];
						fla->allflags[2] += fla->flags[2];
					}
					else
						ft_putstr("error fd\n");
					i++;
				}
				fla->flags[0] = fla->allflags[0];
				fla->flags[1] = fla->allflags[1];
				fla->flags[2] = fla->allflags[2];
				print_Total(fla);
				ft_putstr("\n");
			}
			else
			{
				read_a_count(fla, 0);
				print_flags(fla);
				ft_putstr("\n");
			}
		}
		else
			print_credit(error);
	}
	else
	{
		fla->cflags = 896;
		read_a_count(fla, 0);
		print_flags(fla);
		ft_putstr("\n");
	}
	free(fla);
	return (0);
}
