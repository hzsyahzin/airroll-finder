#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include "armordata.h"


void get_airrolls(int endurance, bool fap)
{
	float max_weight = (float)endurance + 40.0f;
	int results = 0;

	char str[100];
	if (fap)
	{
		sprintf(str, "%dEND-FAP.txt", endurance);
		max_weight *= 1.2f;
	}
	else
		sprintf(str, "%dEND.txt", endurance);

	FILE *fptr = fopen(str, "w");

	float target, boundary, compared_weight;
	bool masked;

	for (int h=0; h<70; h++)
	{
		if (!strcmp("Mask of the Father", head_names[h]))
		{
			masked = true;
			compared_weight = max_weight * 1.05f;
		}
		else
		{
			masked = false;
			compared_weight = max_weight;
		}

		target = compared_weight * 0.25f;
		boundary = compared_weight * 0.25000088f;

		for (int c=0; c<59; c++)
		{
			for (int g=0; g<56; g++)
			{
				for (int l=0; l<59; l++)
				{
					float sum = head[h] + chest[c] + gloves[g] + legs[l];
					float balance = target - sum;

					if (balance < 0.0f)
						continue;

					float fractional = round((balance - floor(balance)) * 10) / 10;
					if (fractional != 0.0f && fractional != 0.5f)
						continue;

					balance = floor(balance) + fractional;

					sum = balance + head[h] + chest[c] + gloves[g] + legs[l];

					if (sum <= target || sum >= boundary)
						continue;

					fprintf(fptr, "%.1f + %.1f + %.1f + %.1f (+%.1f)\t", 
						head[h], chest[c], gloves[g], legs[l], balance);
					fprintf(fptr, "%s + %s + %s + %s\n", head_names[h], 
						chest_names[c], gloves_names[g], legs_names[l]);

					++results;
				}
			}
		}
	}

	fclose(fptr);

	printf("%d results found. Generated file: %s\n", results, str);
}

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		printf("airoll-finder requires 2 arguments: [ENDURANCE] [FAP (y/n)]\n");
		return -1;
	}

	bool fap = (!strcmp(argv[2], "y") || !strcmp(argv[2], "Y"));
	get_airrolls(strtol(argv[1], NULL, 10), fap);
	return 0;
}