/****************************************************
@Author Vinicius Cesar 
@Contact vinians@terra.com.br
@Date 16/11/2008
@Version 1.0 
Pode ser compilado em qualquer C compativel com ANSI
Agradeço a Jack W. Crenshaw pelo seu excelente artigo
sobre criação de compiladores. Veja em 
http://compilers.iecc.com/crenshaw/
*****************************************************/
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
float term();
void init();
float expression();
float get_num();
void match(char c);
void skip_spaces();

void aborta(char *fmt, ...);
char isnum(char c);
void license(void);

char buff[1024];
char look = 0;
int look_pos = 0;

int main(int argc, char **argv)
{
	int i = 0;
	if (argc <= 1)
	{
		puts("\bUsage:\ncalc \"<expression>\" Operators:");
		puts("[+ - * /]... add/subtract/multiply/divide");
		puts("[^ ()]...... Exponenciation and Parentesis\n");
		puts("Copyright (C) 2011 - 2021 ViniciusCgp");
		puts("Email-me at: vinians2006@yahoo.com.br");
		puts("Digite calc --license pra ler a licensa");
		exit(0);
	}
	if (strcmp("--license", argv[1]) == 0)
	{
		license();
		exit(0);
	}

	for (i = 1; i < argc; i++)
	{
		strcat(buff, (char *)argv[i]);
		strcat(buff, " ");
	}
	init();
	printf("%5.2f\n", expression());
	return 0;
}
void license(void)
{
	puts("Este programa e distribuido livremente!");
	puts("Este programa e distribuido 'como esta' sem nenhuma garantia.");
}

void next_char()
{
	look = buff[look_pos];
	if (look != '\0')
		look_pos++;
}

void match(char c)
{
	if (look != c)
	{
		aborta("\bExpected '%c' found '%c'", c, look);
	}
	next_char();
}

void skip_spaces()
{
	while (look == 32 && look != '\0')
	{
		next_char();
	}
}

float expression()
{
	float total = 0;
	skip_spaces();
	if (look == '-')
		total = 0;
	else
		total = term();
	while (look != '\0' && look != ')')
	{
		switch (look)
		{
		case '+':
			match('+');
			skip_spaces();
			total += term();
			break;
		case '-':
			match('-');
			skip_spaces();
			total -= term();
			break;
		default:
			aborta("+ or - expected\n");
		}
	}
	return total;
}
float term()
{
	float num = 0;
	if (look == '(')
	{
		match('(');
		num = expression();
		match(')');
		skip_spaces();
	}
	else
		num = get_num();
	while ((look == '*') || (look == '/') || (look == '^'))
	{
		switch (look)
		{
		case '*':
			match('*');
			skip_spaces();
			num = num * get_num();
			break;
		case '/':
			match('/');
			skip_spaces();
			num = num / get_num();
			break;
		case '^':
			match('^');
			skip_spaces();
			num = (float)pow((double)num, (double)expression());
			break;
		case '(':
			match('(');
			num += expression();
			match(')');
		}
	}
	return num;
}

float get_num()
{
	float num = 0;
	char temp[12], i = 0;
	if (!isnum(look))
		aborta("Number expected");
	while (isnum(look))
	{
		temp[i++] = look;
		next_char();
	}
	temp[i] = '\0';
	num = (float)atof(&temp[0]);
	skip_spaces();
	return num;
}

void init()
{
	look = 0;
	look_pos = 0;
	next_char();
}

void aborta(char *fmt, ...)
{
	va_list argp;
	va_start(argp, fmt);
	vfprintf(stderr, fmt, argp);
	va_end(argp);
	fprintf(stderr, "\n");
	exit(1);
}
char isnum(char c)
{
	return (((c >= '0') && (c <= '9')) || c == '.');
}
