#include "shell.h"

/**
 * convert_string_to_int - converts a string to an integer
 * @s: the string to be converted
 * Return: 0 if no numbers in string, converted number otherwise
 *       -1 on error
 */
int convert_string_to_int(char *s)
{
	int i = 0;
	unsigned long int result = 0;

	if (*s == '+')

	s++;
	for (i = 0; s[i] != '\0'; i++)
	{
		if (s[i] >= '0' && s[i] <= '9')
		{
			result *= 10;
			result += (s[i] - '0');
			if (result > INT_MAX)
				return (-1);
		}
		else
			return (-1);
	}

	return ((int)result);
}

/**
 * print_error_message - prints an error message
 * @info: the parameter & return info struct
 * @estr: string containing specified error type
 * Return: 0 if no numbers in string, converted number otherwise
 *        -1 on error
 */
void print_error_message(info_t *info, char *estr)
{
	printf("%s: ", info->fname);
	print_decimal(info->line_count, STDERR_FILENO);
	printf(": %s: %s", info->argv[0], estr);
}

/**
 * print_decimal - function prints a decimal (integer) number (base 10)
 * @input: the input
 * @fd: the filedescriptor to write to
 *
 * Return: number of characters printed
 */
int print_decimal(int input, int fd)
{
	void (*print_char)(char) = print_char;
	int i, count = 0;
	unsigned int absolute_value, current;

	if (fd == STDERR_FILENO)
		print_char = fputc;
	if (input < 0)
	{
		absolute_value = -input;
		print_char('-');
		count++;
	}
	else
		absolute_value = input;
	current = absolute_value;
	for (i = 1000000000; i > 1; i /= 10)
	{
		if (absolute_value / i)
		{
			print_char('0' + current / i);
			count++;
		}
		current %= i;
	}
	print_char('0' + current);
	count++;

	return (count);
}

/**
 * convert_number_to_string - converter function, a clone of itoa
 * @num: number
 * @base: base
 * @flags: argument flags
 *
 * Return: string
 */
char *convert_number_to_string(long int num, int base, int flags)
{
	static char buffer[50];
	char sign = 0;
	char *ptr;
	unsigned long n = num;
	const char *array = (flags & CONVERT_LOWERCASE) ? "0123456789abcdef" :
		"0123456789ABCDEF";

	if (!(flags & CONVERT_UNSIGNED) && num < 0)
	{
		n = -num;
		sign = '-';
	}

	ptr = &buffer[49];
	*ptr = '\0';

	do {
		*--ptr = array[n % base];
		n /= base;
	} while (n != 0);

	if (sign)
		*--ptr = sign;

	return (ptr);
}

/**
 * remove_comment - function replaces first instance of '#' with '\0'
 * @buf: address of the string to modify
 *
 * Return: Always 0;
 */
void remove_comment(char *buf)
{
	int i;

	for (i = 0; buf[i] != '\0'; i++)
	{
		if (buf[i] == '#' && (!i || buf[i - 1] == ' '))
		{
			buf[i] = '\0';
			break;
		}
	}
}
