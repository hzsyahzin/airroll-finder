/* Weight load addition: LH1 + RH1 + LH2 + RH2 + Head + Chest + Gloves + Legs (+ Hair) */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "armor.h"

void get_airrolls(int endurance, bool fap, bool hr)
{
	float max_equip = (float)endurance + 40.0f;
	int results = 0;
	char str[30];

	// Necessary to have separate to preserve floating point error
	if (fap && hr)
	{
		sprintf(str, "%dEND-FAP-HR.txt", endurance);
		max_equip = max_equip * 1.5f * 1.2f;
	}
	else if (fap)
	{
		sprintf(str, "%dEND-FAP.txt", endurance);
		max_equip = max_equip * 1.2f;
	}
	else if (hr)
	{
		sprintf(str, "%dEND-HR.txt", endurance);
		max_equip = max_equip * 1.5f;
	}
	else
	{
		sprintf(str, "%dEND.txt", endurance);
	}

	printf("Equip load: %.20f\n", max_equip);
	FILE *fptr = fopen(str, "w");

	float armor_weight, target, fractional, equip, lower_bound, upper_bound, compared_equip, total;
	for (int h = 0; h < NUM_HEAD; h++)
	{
		compared_equip = (!strcmp("Mask of the Father", head_names[h])) ? max_equip * 1.05f : max_equip;
		lower_bound = compared_equip * 0.25f;
		upper_bound = compared_equip * 0.25000088f; 

		for (int c = 0; c < NUM_CHEST; c++)
		{
			for (int g = 0; g < NUM_GLOVES; g++)
			{
				for (int l = 0; l < NUM_LEGS; l++)
				{
					armor_weight = head[h] + chest[c] + gloves[g] + legs[l];
					target = upper_bound - armor_weight;
					if (target <= 0.0f)
						continue;

					for (int t = 0; t < 3; t++)
					{
						equip = target - (t * 0.3f);
						fractional = round((equip - floor(equip)) * 10) / 10;
						if (fractional != 0.0f && fractional != 0.5f || equip <= 0.0f)
							continue;
						
						equip = floor(equip) + fractional;

						total = (t*0.3f) + equip + head[h] + chest[c] + gloves[g] + legs[l];
						if (total <= lower_bound || total >= upper_bound)
							continue;

						fprintf(fptr,
							"(%.1f + %d talisman) + %.1f + %.1f + %.1f + %.1f\t\t%s + %s + %s + %s\n", 
							equip, t, head[h], chest[c], gloves[g], legs[l],
							head_names[h], chest_names[c], gloves_names[g], legs_names[l]);

						++results;
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