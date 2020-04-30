// Fill out your copyright notice in the Description page of Project Settings.

#define MY_GAMEUSERSETTINGS_VERSION 8	// UE settings + 3
#include "AaronGameUserSettings.h"

float UAaronGameUserSettings::GetMasterSoundVolume()
{
	return MasterSoundVolume;
}

void UAaronGameUserSettings::SetMasterSoundVolume(float Volume)
{
	MasterSoundVolume = Volume;
}

float UAaronGameUserSettings::GetSFXSoundVolume()
{
	return SFXSoundVolume;
}

void UAaronGameUserSettings::SetSFXSoundVolume(float Volume)
{
	SFXSoundVolume = Volume;
}

float UAaronGameUserSettings::GetVoiceSoundVolume()
{
	return VoiceSoundVolume;
}

void UAaronGameUserSettings::SetVoiceSoundVolume(float Volume)
{
	VoiceSoundVolume = Volume;
}

float UAaronGameUserSettings::GetMusicSoundVolume()
{
	return MusicSoundVolume;
}

void UAaronGameUserSettings::SetMusicSoundVolume(float Volume)
{
	MusicSoundVolume = Volume;
}

float UAaronGameUserSettings::GetMouseSensivity()
{
	return MouseSensivity;
}

void UAaronGameUserSettings::SetMouseSensivity(float NewSensivity)
{
	MouseSensivity = NewSensivity;
}

bool UAaronGameUserSettings::GetIsToggleSprint()
{
	return IsToggleSprint;
}

void UAaronGameUserSettings::SetIsToggleSprint(bool IsToggle)
{
	IsToggleSprint = IsToggle;
}

int UAaronGameUserSettings::GetHoldingTimeWheel()
{
	return HoldingTimeWheel;
}

float UAaronGameUserSettings::GetHoldingTimeWheelSec()
{
	float time = HoldingTimeWheel;
	return (time / 1000.f);
}

void UAaronGameUserSettings::SetHoldingTimeWheel(int HoldingTime)
{
	HoldingTimeWheel = HoldingTime;
}

bool UAaronGameUserSettings::IsVersionValid()
{
	return (Version == MY_GAMEUSERSETTINGS_VERSION);
}

void UAaronGameUserSettings::UpdateVersion()
{
	Version = MY_GAMEUSERSETTINGS_VERSION;
}
