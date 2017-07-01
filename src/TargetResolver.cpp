#include "TargetResolver.hpp"
#include <regex>

#define MAX_GLOB_SIZE		0xF0

#define SKIP_NUMBER(s) while (*s && isdigit(*s)) s++
#define SKIP_GLOB(s) while (*s && (isdigit(*s) || *s == '-' || *s == '[' || *s == ']')) s++

char *		TargetResolver::_GlobTarget(const char *target)
{
	static char			buff[MAX_GLOB_SIZE];
	static const char *	oldTarget = NULL;
	static GlobMatch	globMatches[2];
	const char *		sTarget = target;
	int					globMatchIndex = 0;
	int					i = 0;

	//TODO: better globing with more than two matches x)
	if (oldTarget != target)
	{
		bzero(globMatches, sizeof(globMatches));
		while (*target)
		{
			if (*target == 'p')
				globMatchIndex = 1;
			if (*target == '[')
			{
				globMatches[globMatchIndex].value = globMatches[globMatchIndex].min = atoi(++target);
				SKIP_NUMBER(target);
				if (*target != '-')
					printf("parse error at: %s\n", target), exit(-1);
				globMatches[globMatchIndex].max = atoi(++target);
				SKIP_NUMBER(target);
				if (*target != ']')
					printf("parse error at: %s\n", target), exit(-1);
				target++;
				if (globMatches[globMatchIndex].max < globMatches[globMatchIndex].min)
					printf("min-max error at: %s\n", target), exit(-1);
			}
			else
				target++;
		}
		oldTarget = sTarget;
		target = sTarget;
	}
	globMatchIndex = 0;
//	printf("min0 = %i, max0 = %i, val0 = %i, min1 = %i, max1 = %i, val1 = %i\n", globMatches[0].min, globMatches[0].max, globMatches[0].value, globMatches[1].min, globMatches[1].max, globMatches[1].value);
	while (*target)
	{
		if (*target == 'p')
			globMatchIndex = 1;
		if (*target == '[')
		{
			SKIP_GLOB(target);
			i += sprintf(buff + i, "%i", globMatches[globMatchIndex].value);
			if (globMatchIndex == 1)
				globMatches[1].value++;
		}
		else
			buff[i++] = *target++;
	}

	if (globMatches[0].value == globMatches[0].max + 1)
		return NULL;

	if (globMatches[1].max == 0)
		globMatches[1].value = 1;
	if (globMatches[1].value == globMatches[1].max + 1)
	{
		globMatches[0].value++;
		globMatches[1].value = globMatches[1].min;
	}
	buff[i] = 0;
	return buff;
}

bool		TargetResolver::Resolve(const char *target, char *ipbuff, bool & error)
{
	std::regex	ipRegex("^(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])\\.(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])\\.(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])\\.(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])$");
	std::smatch	matches;
	if (!(target = _GlobTarget(target)))
		return false;

	if (std::regex_match(std::string(target), matches, ipRegex))
	{
		strcpy(ipbuff, target);
		return true;
	}

	error = true;
	//TODO: resolve host to find ip
	return false;
}
