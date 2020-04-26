#include "decode.h"
#include "key.h"
#include "lcd.h"
#include "led.h"
#include "button.h"
#include "beep.h"
#include "rtdef.h"
#include "rtthread.h"
#include "drv_gpio.h"
#include <stdint.h>
#include "decode.h"
#include "player.h"
#include "song.h"
#include "stm32f4xx_ll_fmc.h"

struct player player;
struct audio_ops audio;
struct decode_ops decode;

uint8_t beep_volume = 3;

int decode_read(void *song, int index, void *buffer, int size)
{
	beep_song_get_data(song, index,buffer);
	return 1;
}

int audio_write(void *buffer, int size)
{
	struct beep_song_data *data = buffer;

	led_toggle();
	
	beep_on();
	beep_set(data->freq, beep_volume);
	rt_thread_mdelay(data->sound_len);
	beep_off();
	rt_thread_mdelay(data->nosound_len);

	return size;
}

int decode_control(void *song, int cmd, void *arg)
{
	if (cmd == DECODE_OPS_CMD_GET_NAME)
	{
		beep_song_get_name(song, arg);
	}
	else if (cmd == DECODE_OPS_CMD_GET_LEN)
	{
		*(uint16_t *)arg = beep_song_get_len(song);
	}
	return 0;
}

int audio_init(void)
{
	beep_init();
	led_init();
	return 0;
}

int audio_open(void)
{
	beep_on();
	led_on();
	return 0;
}

int audio_close(void)
{
	beep_off();
	led_off();
	return 0;
}


int audio_control(int cmd, void *arg)
{
	if (cmd == AUDIO_OPS_CMD_SET_VOL)
	{
		beep_volume = *(uint8_t *)arg;
	}
	return beep_volume;
}

int player_init(void)
{
	decode.init = beep_song_decode_init;
	decode.control = decode_control;
	decode.read = decode_read;

	audio.init = audio_init;
	audio.open = audio_open;
	audio.close = audio_close;
	audio.control = audio_control;
	audio.write = audio_write;

	player.decode = &decode;
	player.audio = &audio;

	player_add_song(&player, (void *)&song1);
	player_add_song(&player, (void *)&song2);
	player_add_song(&player, (void *)&song3);
	player_add_song(&player, (void *)&song4);
	player_add_song(&player, (void *)&song5);
	player_start(&player);

	player_control(&player, PLAYER_CMD_PLAY, RT_NULL);
	player_show(&player);

	return 0;
}

#define KEY0 PIN_KEY0
#define KEY1 PIN_KEY1
#define KEY2 PIN_KEY2
#define KEY_WKUP PIN_WAKE_UP

#define KEY_PRESS_VALUE 0
#define KEY_WKUP_PRESS_VALUE 1

int main(void)
{
	/* user app entry */
	player_init();
	key_init();

	return 0;
}
