/*
 *
 *	FBL ENGINE
 *
 *	sound.c
 *
 *	Contains sound related functions like samples and music playback.
 *
 *	Hans Strömquist 2017 - 2022
 *
 */

#include "engine.h"


/* functions only used by sound.c */

int destroy_sound_sample(int tag, void *sound, void *dummy);
DLLIST *get_sound_item_at_id(int id);


DLLIST *fbl_sound_list = NULL;
unsigned int current_sound = 0;

/* for simplicity, only one song at a time */

Mix_Music *fbl_music = NULL;


/*
 * Create sound by adding an item on the list and loading a sample.
 */
int fbl_load_sound(const char *sample_file)
{

	FBL_SOUND *fbl_sound = NULL;

	char* new_path = engine_get_platform_asset_path(sample_file);

	fbl_sound = malloc(sizeof(FBL_SOUND));

	if (!fbl_sound)
	{
		fprintf(FBL_ERROR_OUT, "No memory for sound item!\n");
		return -1;
	}

	/* Load the sample */

	fbl_sound->sample = Mix_LoadWAV(new_path);
	if (fbl_sound->sample == NULL)
	{
		fprintf(FBL_ERROR_OUT, "Failed to load sound file! SDL_mixer Error: %s\n", Mix_GetError());
		return 1;
	}

	//fbl_sound->loop = loop;
	//fbl_sound->pan = pan;



	/* if it's the first sound created start a new list, otherwise append */

	if (current_sound == 0)
	{

		fbl_sound_list = DLCreate(current_sound, fbl_sound, sizeof(FBL_SOUND));

	}
	else
	{

		DLAppend(&fbl_sound_list, current_sound, fbl_sound, sizeof(FBL_SOUND));

	}

	free(fbl_sound);

	current_sound++;

	return current_sound - 1;


}


/*
 * Delete sound by removing the item on the list and destroying the sample
 */
void fbl_delete_sound(int id)
{

	FBL_SOUND *sound = NULL;
	DLLIST *item = get_sound_item_at_id(id);

	if (item != NULL)
	{

		sound = ((FBL_SOUND *)item->Object);

		/* security so that the first element can't be deleted, will crash! */

		if (id > 0)
		{
			Mix_FreeChunk(sound->sample);
			DLDelete(item);
		}
		else {
#ifdef FBL_DEBUG
			fprintf(FBL_ERROR_OUT, "Will not delete the first sound in list! (id: %d)\nUse destroy_all to get rid of it..\n", id);
#endif
		}

	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to delete sound %d, that does not exist!\n", id);
#endif

}


void fbl_play_sound(int id, int channel, int loops)
{

	FBL_SOUND *sound = NULL;
	DLLIST *item = get_sound_item_at_id(id);

	if (item != NULL)
	{

		sound = ((FBL_SOUND *)item->Object);

		sound->channel = Mix_PlayChannel(channel, sound->sample, loops);

		if (sound->channel == -1) {
			printf("Mix_PlayChannel error: %s\n", Mix_GetError());
		}

	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to play sound %d, that does not exist!\n", id);
#endif

}

void fbl_set_sound_volume(int id, int volume) {

	FBL_SOUND* sound = NULL;
	DLLIST* item = get_sound_item_at_id(id);

	if (item != NULL)
	{

		sound = ((FBL_SOUND*)item->Object);
		Mix_VolumeChunk(sound->sample, volume);	// MIX_MAX_VOLUME = 128

	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to set volume on sound %d, that does not exist!\n", id);
#endif

}

void fbl_pause_sound(int id)
{

	FBL_SOUND *sound = NULL;
	DLLIST *item = get_sound_item_at_id(id);

	if (item != NULL)
	{

		sound = ((FBL_SOUND *)item->Object);

		Mix_Pause(sound->channel);

	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to pause sound %d, that does not exist!\n", id);
#endif

}

void fbl_resume_sound(int id)
{

	FBL_SOUND *sound = NULL;
	DLLIST *item = get_sound_item_at_id(id);

	if (item != NULL)
	{

		sound = ((FBL_SOUND *)item->Object);

		Mix_Resume(sound->channel);

	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to resume sound %d, that does not exist!\n", id);
#endif

}

void fbl_destroy_all_sounds()
{

	DLWalk(fbl_sound_list, destroy_sound_sample, NULL);

	DLDestroy(&fbl_sound_list);

	current_sound = 0;

}



/* music related functions */


int fbl_load_music(const char *song_file)
{

	char* new_path = engine_get_platform_asset_path(song_file);

	fbl_music = Mix_LoadMUS(new_path);

	if (!fbl_music) {
		fprintf(FBL_ERROR_OUT, "Failed to load music file! SDL_mixer Error: %s\n", Mix_GetError());
		return 1;
	}

	return 0;

}


void fbl_destroy_music(void)
{

	if (fbl_music != NULL)
	{
		Mix_FreeMusic(fbl_music);
		fbl_music = NULL;
	}

}


void fbl_play_music(int volume, int loops)
{

	Mix_VolumeMusic(volume); // MIX_MAX_VOLUME = 128

	if (Mix_PlayMusic(fbl_music, loops) == -1) {
		fprintf(FBL_ERROR_OUT, "Failed to play music file! SDL_mixer Error: %s\n", Mix_GetError());
	}

}

void fbl_pause_music(void)
{

	Mix_PauseMusic();

}


void fbl_resume_music(void)
{

	Mix_ResumeMusic();

}


void fbl_stop_music(void)
{

	Mix_HaltMusic();

}


/* free the sound sample */

int destroy_sound_sample(int tag, void *sound, void *dummy)
{

	Mix_FreeChunk(((FBL_SOUND *)sound)->sample);

	return 0;

}


/*
* Return the correct item at the id.
* Check sprite-version for details.
*/
DLLIST *get_sound_item_at_id(int id)
{

	DLLIST *item;

	item = DLGetFirst(fbl_sound_list);


	while (item != NULL)
	{

		if (item->Tag == id)
			return item;

		item = item->Next;

	}

	return NULL;

}

/* end of sound.c */
