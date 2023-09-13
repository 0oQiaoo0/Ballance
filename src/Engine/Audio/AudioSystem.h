#pragma once
#include <Windows.h>
#include <mmsystem.h>
#include <vector>
#include <string>
#include "../../src/Engine/core/ThreadPool.h"

enum AudioName {
    main_bgm,
    btn_clicked,
    roll_paper,
    roll_wood_wood,
    roll_wood_stone,
    roll_wood_metal,
    start_level,
    revive,
    extra_time,
    extra_life,
    checkpoint,
    terminal,
    transform,
    fall_down,
    game_bgm1,
    game_bgm2,
    game_bgm3,

};
static std::vector<std::string> audioFilePaths = {
    "../../src/Asset/Audio/music/Menu_atmo.wav",
    "../../src/Asset/Audio/effect/Menu_click.wav",
    "../../src/Asset/Audio/effect/Roll_Paper.wav",
    "../../src/Asset/Audio/effect/Roll_Wood_Wood.wav",
    "../../src/Asset/Audio/effect/Roll_Wood_Stone.wav",
    "../../src/Asset/Audio/effect/Roll_Wood_Metal.wav",
    "../../src/Asset/Audio/effect/Misc_StartLevel.wav",
    "../../src/Asset/Audio/effect/Extra_Start.wav",
    "../../src/Asset/Audio/effect/More_Seconds.wav",
    "../../src/Asset/Audio/effect/One_More_Life.wav",
    "../../src/Asset/Audio/effect/CheckPoint.wav",
    "../../src/Asset/Audio/effect/Terminal.wav",
    "../../src/Asset/Audio/effect/Transform.wav",
    "../../src/Asset/Audio/effect/Misc_Fall.wav",
    "../../src/Asset/Audio/music/Music_Theme_1_1.wav",
    "../../src/Asset/Audio/music/Music_Theme_1_2.wav",
    "../../src/Asset/Audio/music/Music_Theme_1_3.wav",

};


// “Ù∆µœµÕ≥
class AudioSystem {
public:
    ~AudioSystem();
    bool Init();
    void Tick(float deltaTime);
    static bool Play(AudioName audioName, bool repeat);
    static bool Stop(AudioName audioName);
};