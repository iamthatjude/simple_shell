#include "shell.h"

/**
 * buffer_input - buffers chained commands
 * @info: parameter struct
 * @buffer: address of buffer
 * @length: address of length var
 *
 * Return: bytes read
 */
ssize_t buffer_input(info_t *info, char **buffer, size_t *length)
{
	ssize_t bytesRead = 0;
	size_t lengthRemaining = 0;

	if (!*length) /* If nothing left in the buffer, fill it */
	{
		free(*buffer);
		*buffer = NULL;
		signal(SIGINT, sigintHandler);
#if USE_GETLINE
		bytesRead = getline(buffer, &lengthRemaining, stdin);
#else
		bytesRead = custom_getline(info, buffer, &lengthRemaining);
#endif
		if (bytesRead > 0)
		{
			if ((*buffer)[bytesRead - 1] == '\n')
			{
				(*buffer)[bytesRead - 1] = '\0'; /* Remove trailing newline */
				bytesRead--;
			}
			info->linecount_flag = 1;
			remove_comment(*buffer);
			build_history_list(info, *buffer, info->histcount++);
			/* if (_strchr(*buffer, ';')) is this a command chain? */
			{
				*length = bytesRead;
				info->cmd_buf = buffer;
			}
		}
	}
	return (bytesRead);
}

/**
 * get_input_line - gets a line minus the newline
 * @info: parameter struct
 *
 * Return: bytes read
 */
ssize_t get_input_line(info_t *info)
{
	static char *buffer; /* The ';' command chain buffer */
	static size_t position, nextPosition, length;
	ssize_t bytesRead = 0;
	char **argBuffer = &(info->arg), *ptr;

	_putchar(BUF_FLUSH);
	bytesRead = buffer_input(info, &buffer, &length);
	if (bytesRead == -1) /* EOF */
		return (-1);
	if (length) /* We have commands left in the chain buffer */
	{
		nextPosition = position; /* Init new iterator to current buffer position */
		ptr = buffer + position; /* Get pointer for return */

		check_chain(info, buffer, &nextPosition, position, length);
		while (nextPosition < length) /* Iterate to semicolon or end */
		{
			if (is_chain(info, buffer, &nextPosition))
				break;
			nextPosition++;
		}
		position = nextPosition + 1; /* Increment past nulled semicolon */
		if (position >= length) /* Reached end of buffer? */
		{
		position = length = 0; /* Reset position and length */
			info->cmd_buf_type = CMD_NORM;
		}

		*argBuffer = ptr; /* Pass back pointer to current command position */
		return (custom_strlen(ptr)); /* Return length of current command */
	}

	*argBuffer = buffer; /* Else not a chain, pass back from custom_getline() */
	return (bytesRead); /* Return length of buffer from custom_getline() */
}

/**
 * read_buffer - reads a buffer
 * @info: parameter struct
 * @buffer: buffer
 * @position: size
 *
 * Return: bytesRead
 */
ssize_t read_buffer(info_t *info, char *buffer, size_t *position)
{
	ssize_t bytesRead = 0;

	if (*position)
		return (0);
	bytesRead = read(info->readfd, buffer, READ_BUF_SIZE);
	if (bytesRead >= 0)
		*position = bytesRead;
	return (bytesRead);
}

/**
 * custom_getline - gets the next line of input from STDIN
 * @info: parameter struct
 * @ptr: address of pointer to buffer, preallocated or NULL
 * @length: size of preallocated ptr buffer if not NULL
 *
 * Return: size
 */
int custom_getline(info_t *info, char **ptr, size_t *length)
{
	static char buffer[READ_BUF_SIZE];
	static size_t position, bufLength;
	size_t k;
	ssize_t bytesRead = 0, size = 0;
	char *p = NULL, *newPtr = NULL, *c;

	p = *ptr;
	if (p && length)
		size = *length;
	if (position == bufLength)
		position = bufLength = 0;

	bytesRead = read_buffer(info, buffer, &bufLength);
	if (bytesRead == -1 || (bytesRead == 0 && bufLength == 0))
		return (-1);

	c = custom_strchr(buffer + position, '\n');
	k = c ? 1 + (unsigned int)(c - buffer) : bufLength;
	newPtr = custom_realloc
		(p, size, size ? size + k : k + 1);
	if (!newPtr) /* MALLOC FAILURE! */
		return (p ? (free(p), -1) : -1);

	if (size)
		custom_strncat(newPtr, buffer + position, k - position);
	else
		custom_strncpy(newPtr, buffer + position, k - position + 1);

	size += k - position;
	position = k;
	p = newPtr;

	if (length)
		*length = size;
	*ptr = p;
	return (size);
}

/**
 * sigintHandler - blocks ctrl-C
 * @signalNumber: the signal number
 *
 * Return: void
 */
void sigintHandler(__attribute__((unused)) int signalNum)
{
	_puts("\n");
	_puts("$ ");
	_putchar(BUF_FLUSH);
}
