#ifndef _MODE_H_
#define _MODE_H_

#define Rcheat(a)    ((int)(a))

struct dolnod
{
	struct dolnod	*dolnxt;
	int	doluse;
	unsigned char	**dolarg;
};

#endif