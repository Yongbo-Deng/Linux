#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <getopt.h>
#include <linux/input.h>
#include <sys/ioctl.h>
#include <tslib.h>

int distance(struct ts_sample_mt *point1, struct ts_sample_mt *point2)
{
	int x = point1->x - point2->x;
	int y = point1->y - point2->y;
	int ret = x * x + y * y;
	return ret;
}

int main(int argc, char **argv)
{
	struct tsdev *ts;
	int ret;
	int i;
	int max_slots;
	struct input_absinfo slot;
	struct ts_sample_mt **samp_mt;
	struct ts_sample_mt **pre_samp_mt;
	int touch_count = 0;
	int point_pressed[20];

	ts = ts_setup(NULL, 0);
	if (!ts)
	{
		printf("ts_setup err\n");
		return -1;
	}

	if (ioctl(ts_fd(ts), EVIOCGABS(ABS_MT_SLOT), &slot) < 0) 
	{
		perror("ioctl EVIOGABS");
		ts_close(ts);
		return errno;
	}

	max_slots = slot.maximum + 1 - slot.minimum;

	samp_mt = malloc(sizeof(struct ts_sample_mt *));
	if (!samp_mt) {
		ts_close(ts);
		return -ENOMEM;
	}
	samp_mt[0] = calloc(max_slots, sizeof(struct ts_sample_mt));
	if (!samp_mt[0]) {
		free(samp_mt);
		ts_close(ts);
		return -ENOMEM;
	}

	pre_samp_mt = malloc(sizeof(struct ts_sample_mt *));
	if (!pre_samp_mt) {
		ts_close(ts);
		return -ENOMEM;
	}
	pre_samp_mt[0] = calloc(max_slots, sizeof(struct ts_sample_mt));
	if (!pre_samp_mt[0]) {
		free(pre_samp_mt);
		ts_close(ts);
		return -ENOMEM;
	}

	for (int i = 0; i < max_slots; i++)
	{
		pre_samp_mt[0][i].valid = 0;
	}

	while (1) 
	{
		ret = ts_read_mt(ts, samp_mt, max_slots, 1);
		if (ret < 0) 
		{
			printf("ts_read_mt err\n");
			ts_close(ts);
			return -1;
		}

		for (i = 0; i < max_slots; i++)
		{
			if (samp_mt[0][i].valid)
			{
				memcpy(&pre_samp_mt[0][i], &samp_mt[0][i], sizeof(struct ts_sample_mt));
			}
		}

		touch_count = 0;
		for (i = 0; i < max_slots; i++)
		{
			if (pre_samp_mt[0][i].valid && pre_samp_mt[0][i].tracking_id != -1)
			{
				point_pressed[touch_count++] = i;
			}
		}

		if (touch_count == 2)
		{
			printf("distance: %08d\n", distance(&pre_samp_mt[0][point_pressed[0]], &pre_samp_mt[0][point_pressed[1]]));
		}
	}
	return 0;
}

