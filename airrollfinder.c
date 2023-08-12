#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "armordata.h"

void get_airrolls(int endurance)
{
	char str[100];
	sprintf(str, "%dend.txt", endurance);

	float max_weight = (float)endurance + 40.0f;

	float lb = max_weight * 0.25f;
	float ub = max_weight * 0.25000088f;

	FILE *fptr = fopen(str, "w");

	int results = 0;
	for (int h=0; h<70; h++)
	{
		for (int c=0; c<59; c++)
		{
			for (int g=0; g<56; g++)
			{
				for (int l=0; l<59; l++)
				{
					float raw_total = head[h] + chest[c] + gloves[g] + legs[l];
					if (raw_total > ub)
						continue;

					float fractional = raw_total - (int)raw_total;
					if (fractional <= 0.0f || fractional >= 0.1f)
						continue;

					int scaled_diff = ((int)(lb * 10) - (int)(raw_total * 10));
					if (scaled_diff % 5 != 0)
						continue;

					float balance = scaled_diff / 10.0f;

					// NOTE: using raw_total from earlier breaks this for some reason
					float total = balance
						+ head[h] + chest[c] + gloves[g] + legs[l];

					if (total > lb && total < ub)
					{
						fprintf(fptr, "%.1f + %.1f + %.1f + %.1f (+%.1f)\t", 
							head[h], chest[c], gloves[g], legs[l], balance);
						fprintf(fptr, "%s + %s + %s + %s\n", head_names[h], 
							chest_names[c], gloves_names[g], legs_names[l]);

						results += 1;
					}
				}
			}
		}
	}

	fclose(fptr);

	printf("%d results found. Generated file: %s\n", results, str);
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		printf("airoll-finder requires 1 argument (endurance)\n");
		return -1;
	}

	get_airrolls(strtol(argv[1], NULL, 10));
	return 0;
}