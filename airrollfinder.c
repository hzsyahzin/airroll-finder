#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include "armordata.h"


void get_airrolls(int endurance, bool fap, bool hr)
{
	float max_weight = (float)endurance + 40.0f;
	int results = 0;

	char str[1024];
	sprintf(str, "%dEND", endurance);

	// Necessary to have separate to preserve floating point error
	if (fap && hr)
	{
		strcat(str, "-FAP-HR");
		max_weight = max_weight * 1.5f * 1.2f;
	}
	else if (fap)
	{
		strcat(str, "-FAP");
		max_weight = max_weight * 1.2f;
	}
	else if (hr)
	{
		strcat(str, "-HR");
		max_weight = max_weight * 1.5f;
	}

	printf("Equip load: %.20f\n", max_weight);

	strcat(str, ".txt");
	FILE *fptr = fopen(str, "w");

	float target, boundary, compared_weight;
	for (int h=0; h<70; h++)
	{
		if (!strcmp("Mask of the Father", head_names[h]))
		{
			compared_weight = max_weight * 1.05f;
		}
		else
		{
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
	if (argc != 4)
	{
		printf("airoll-finder requires 3 arguments: [ENDURANCE] [FAP (y/n)] [HR (y/n)]\n");
		return -1;
	}

	bool fap = (!strcmp(argv[2], "y") || !strcmp(argv[2], "Y"));
	bool hr = (!strcmp(argv[3], "y") || !strcmp(argv[3], "Y"));

	get_airrolls(strtol(argv[1], NULL, 10), fap, hr);
	return 0;
}