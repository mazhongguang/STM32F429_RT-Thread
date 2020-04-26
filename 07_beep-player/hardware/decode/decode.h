#ifndef __DECODE_H_
#define __DECODE_H_

#include "rtdef.h"
#include <rtthread.h>
#include <stdint.h>

#define SEMIBREVE_LEN	1900

/* 播放效果 */
#define SOUND_SIGNATURE	0	/* 调号：升0个半音 */
#define SOUND_OCTACHORD	1	/* 升降八度：升一个八度 */
#define SOUND_SPACE		4/5 /* 定义普通音符演奏的长度分率，每四分音符间隔 */

#define SONG_NAME_LENGTH_MAX	30
#define SONG_DATA_LENGTH_MAX	500

struct beep_song
{
	const uint8_t name[SONG_NAME_LENGTH_MAX];
	const uint8_t data[SONG_DATA_LENGTH_MAX];
};

struct beep_song_data
{
	rt_uint16_t freq;
	rt_uint16_t sound_len;
	rt_uint16_t nosound_len;
};

int beep_song_decode_init(void);
uint16_t beep_song_get_len(const struct beep_song *song);
int beep_song_get_name(const struct beep_song *song, char *name);
uint16_t beep_song_get_data(const struct beep_song *song, uint16_t index, struct beep_song_data *data);
							   
#endif
