#ifndef __PLAYER_H
#define __PLAYER_H

#include "rtdef.h"
#include <rtthread.h>
#include <stdint.h>

#define PLAYER_SOUND_SIZE_DEFAULT	3	/* 1 ~ 99 */
#define PLAYER_SONG_NUM_MAX			10
#define PLAYER_SONG_NAME_LEN_MAX	20
#define PLAYER_BUFFER_SIZE			20

enum PLAYER_STATUS
{
	PLAYER_RUNNING,		/* play running */
	PLAYER_STOP			/* stop play */
};
enum PLAYER_CMD
{
	PLAYER_CMD_PLAY,
	PLAYER_CMD_STOP,
	PLAYER_CMD_LAST,
	PLAYER_CMD_NEXT,
	PLAYER_CMD_SET_VOL,
	PLAYER_CMD_GET_VOL,
	PLAYER_CMD_GET_STATUS
};

enum AUDIO_OPS_CMD
{
	AUDIO_OPS_CMD_SET_VOL
};

enum DECODE_OPS_CMD
{
	DECODE_OPS_CMD_GET_NAME,
	DECODE_OPS_CMD_GET_LEN
};

struct	audio_ops 
{
	int (*init)(void);
	int (*open)(void);
	int (*close)(void);
	int (*control)(int cmd, void *arg);
	int (*write)(void *buffer, int size);
};

struct  decode_ops
{
	int (*init)(void);
	int (*control)(void *song, int cmd, int *arg);
	int (*read)(void *song, int index, void *buffer, int size);
};

struct player
{
	enum PLAYER_STATUS	status;				/* current play status */
	uint8_t				volume;				/* sound value */
	uint8_t				song_current;		/* current playing song */
	uint8_t				song_num;			/* song total */
	uint16_t			song_time_pass;		/* song's time already played */
	uint16_t			song_time_all;		/* song's time total */
	void *song_sheet[PLAYER_SONG_NUM_MAX];	/* song list */

	rt_sem_t			sem_play;			/* play status control sem */
	rt_thread_t			play_thread;		/* play thread */

	struct audio_ops	*audio;				
	struct decode_ops	*decode;
};
typedef struct player *player_t;

int player_add_song(player_t player, void *song);	/* add song to song's list */
int player_start(player_t player);					/* player init */
int player_control(player_t player, int cmd, void *arg);
int player_show(player_t player);					/* print song's list and current play progress */
		
#endif
