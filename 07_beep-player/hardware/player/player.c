#include <rthw.h>
#include <stdint.h>
#include <sys/types.h>
#include "led.h"
#include "player.h" 
#include "rtdef.h"
#include "rtthread.h"
#include "lcd.h"

int player_play(player_t player)
{
	rt_uint32_t level;

	if (player->status != PLAYER_RUNNING)
	{
		/* disable interrupt */
		level = rt_hw_interrupt_disable();

		/* set play status to playing */
		player->status = PLAYER_RUNNING;

		/* enable interrupt */
		rt_hw_interrupt_enable(level);

		/* release sem to play's thread start play */
		rt_sem_release(player->sem_play);
	}

	return 0;
}

int player_stop(player_t player)
{
	rt_uint32_t level;

	if (player->status == PLAYER_RUNNING)
	{
		/* disable interrupt */
		level = rt_hw_interrupt_disable();

		/* set play status to stop */
		player->status = PLAYER_STOP;

		/* enable interrupt */
		rt_hw_interrupt_enable(level);
	}

	return 0;
}

int player_last(player_t player)
{
	uint16_t len;
	rt_uint32_t level;

	/* disable interrupt */
	level = rt_hw_interrupt_disable();

	/* current playing song's number to decrease one */
	if (player->song_current > 1)
	{
		player->song_current --;
	}
	else
	{
		player->song_current = player->song_num;
	}

	/* update player current playing song's played time and total time */
	player->song_time_pass = 0;
	
	/* enable interrupt */
	rt_hw_interrupt_enable(level);

	player->decode->control(player->song_sheet[player->song_current - 1], DECODE_OPS_CMD_GET_LEN, len);

	/* disable interrupt */
	level = rt_hw_interrupt_disable();

	player->song_time_all = len;

	/* enable interrupt */
	rt_hw_interrupt_enable(level);

	if (player->status != PLAYER_RUNNING)
	{
		player_play(player);
	}

	return 0;
}
	
int player_next(player_t player)
{
	uint16_t len;
	rt_uint32_t level;

	/* disable interrupt */
	level = rt_hw_interrupt_disable();

	/* current playing song's number to add one */
	if (player->song_current < player->song_num)
	{
		player->song_current ++; 
	}
	else
	{
		player->song_current = 1;
	}

	/* update player current playing song's played time and total time */
	player->song_time_pass = 0;
	
	/* enable interrupt */
	rt_hw_interrupt_enable(level);

	player->decode->control(player->song_sheet[player->song_current - 1], DECODE_OPS_CMD_GET_LEN, &len);

	/* disable interrupt */
	level = rt_hw_interrupt_disable();

	player->song_time_all = len;

	/* enable interrupt */
	rt_hw_interrupt_enable(level);

	if (player->status != PLAYER_RUNNING)
	{
		player_play(player);
	}

	return 0;
}

int player_control(player_t player, int cmd, void *arg)
{
	rt_uint32_t level;

	switch (cmd)
	{
		case PLAYER_CMD_PLAY:
			player_play(player);
			break;
		case PLAYER_CMD_STOP:
			player_stop(player);
			break;
		case PLAYER_CMD_LAST:
			player_last(player);
			break;
		case PLAYER_CMD_NEXT:
			player_next(player);
			break;
		case PLAYER_CMD_SET_VOL:
			/* disable interrupt */
			level = rt_hw_interrupt_disable();
			player->volume = *(uint8_t *)arg;
			/* enable interrupt */
			rt_hw_interrupt_enable(level);
			player->audio->control(AUDIO_OPS_CMD_SET_VOL, &player->volume); 
			break;
		case PLAYER_CMD_GET_VOL:
			*(uint8_t *)arg = player->volume;
			break;
		case PLAYER_CMD_GET_STATUS:
			*(uint8_t *)arg = player->status;
			break;
	}
	return 0;
}

int player_add_song(player_t player, void *song)
{
	rt_uint32_t level;

	if (player->song_num == PLAYER_SONG_NUM_MAX)
	{
		return -1;
	}

	/* disable interrupt */
	level = rt_hw_interrupt_disable();

	player->song_sheet[player->song_num] = song;
	player->song_num ++;

	/* enable interrupt */
	rt_hw_interrupt_enable(level);

	return 0;
}

int player_show(player_t player)
{
	char name[PLAYER_SONG_NAME_LEN_MAX + 1];
	uint8_t i;
	uint16_t percent;
	uint32_t tmp_color;

	/*rt_kprintf("******************  Beep Player ********************\n");*/
	LCD_ShowString(0, 0, 239, 24, 24, "*** beep player ***");	

	/* print song list */
	for (i = 0; i < player->song_num; i++)
	{
		player->decode->control(player->song_sheet[i], DECODE_OPS_CMD_GET_NAME, name);
		/*rt_kprintf("%02d. %s\n", i + 1, name);*/
		/*LCD_ShowxNum(0, 30 + i * 32, i + 1, 2, 32, 0);*/
		/*LCD_ShowString(62, 30 + i * 32, 80, 32, 32, name);*/
	}

	/* print current play status */
	if (player->status == PLAYER_RUNNING)
	{
		/*rt_kprintf("--->  正在播放：");*/
		LCD_ShowString(0, 158, 224, 32, 32, "-----> playing");
	}
	else
	{
		/*rt_kprintf("---|| 暂停播放：");*/
		LCD_ShowString(0, 158, 224, 32, 32, "-----|| pause ");
	}

	/* print current playing song */
	player->decode->control(player->song_sheet[player->song_current - 1], DECODE_OPS_CMD_GET_NAME, name);
	/*rt_kprintf("%s", name);*/
	/*rt_kprintf("  ---\n");*/
	LCD_ShowString(0, 190, 100, 32, 32, name);
	LCD_ShowString(0, 222, 160, 32, 32, "-------------");

	/* print playing progress */
	percent = player->song_time_pass * 100 / player->song_time_all;

	/*rt_kprintf("播放进度：%02d%%  音量大小：%02d%%\n", percent, player->volume);*/
	LCD_ShowString(0, 254, 192, 32, 32, "playing:   %");
	LCD_ShowxNum(128, 254, percent, 3, 32, 0); 
	LCD_ShowString(0, 286, 128, 32, 32, "vol:   %");
	LCD_ShowxNum(64, 286, player->volume, 3, 32, 0);

	/*rt_kprintf("*********************************\n");*/
	/*LCD_ShowString(0, 318, 240, 32, 32, "**********");*/

	return 0;
}

static void player_entry(void *parameter)
{
	player_t player = (player_t)parameter;
	uint8_t buffer[PLAYER_BUFFER_SIZE], size;

	while (1)
	{
		if (player->status == PLAYER_RUNNING)
		{
			size = player->song_time_all - player->song_time_pass;
			if (size > PLAYER_BUFFER_SIZE)
			{
				size = PLAYER_BUFFER_SIZE;
			}
			size = player->decode->read(player->song_sheet[player->song_current - 1], player->song_time_pass, buffer, size);
			if (size > 0)
			{
				player->audio->write(buffer, size);
				player->song_time_pass += size;
				player_show(player);
			}

			/* current playing over change to next song */
			if (player->song_time_pass >= player->song_time_all)
			{
				player_next(player);
				player_show(player);
			}
		}
		else 
		{
			/* pause play to close audio */
			player->audio->close();

			/* waiting play sem */
			rt_sem_take(player->sem_play, RT_WAITING_FOREVER);

			/* start play to open audio */
			player->audio->open();
		}
	}
}

int player_start(player_t player)
{
	uint16_t len;
	static rt_uint8_t inited = 0;

	/* check player is running */
	if (inited == 1)
	{
		return -RT_ERROR;
	}

	if (player->song_num == 0)
	{
		return -1;
	}
	
	/* call interface initialization decoder */
	player->decode->init();

	player->status = PLAYER_STOP;
	player->volume = PLAYER_SOUND_SIZE_DEFAULT;
	player->song_current = 1;
	player->song_time_pass = 0;
	player->decode->control(player->song_sheet[player->song_current - 1], DECODE_OPS_CMD_GET_LEN, &len);
	player->song_time_all = len;

	/* call interface initialization audio device */
	player->audio->init();
	player->audio->control(AUDIO_OPS_CMD_SET_VOL, &player->volume);

	/* initialization dynamic sem */
	player->sem_play = rt_sem_create("sem_play", 0, RT_IPC_FLAG_FIFO);
	if (player->sem_play == RT_NULL)
	{
		return -RT_ERROR;
	}

	/* create dynamic thread */
	player->play_thread = rt_thread_create("player", 
										player_entry, player,
										512, 20, 20);
	if (player->play_thread != RT_NULL)
	{
		rt_thread_startup(player->play_thread);
	}
	else
	{
		rt_sem_delete(player->sem_play);
		return -RT_ERROR;
	}

	inited = 1;

	return 0;
}

	

