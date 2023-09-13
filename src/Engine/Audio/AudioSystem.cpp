#include "AudioSystem.h"

AudioSystem::~AudioSystem()
{
	for (auto path : audioFilePaths) {
		std::string cmd = "close " + path;
		mciSendString(LPCSTR(cmd.c_str()), 0, 0, 0);
	}
}

bool AudioSystem::Init()
{
	for (auto path : audioFilePaths) {
		std::string cmd = "open " + path;
		mciSendString(LPCSTR(cmd.c_str()), 0, 0, 0);
	}
	return true;
}

void AudioSystem::Tick(float deltaTime)
{

}

bool AudioSystem::Play(AudioName audioName, bool repeat)
{
	ThreadPool::AddTask([=]() {
		int index = static_cast<int>(audioName);
		std::string cmd = "play " + audioFilePaths[index];
		MCIERROR err;
		TCHAR buffer[256] = { 0 };
		err = mciSendString((LPCSTR)cmd.c_str(), 0, 0, 0);

		if (err) {
			mciGetErrorString(err, buffer, 256);
			std::cout << buffer << std::endl;
			return false;
		}
		});

	return true;
}

bool AudioSystem::Stop(AudioName audioName)
{
	ThreadPool::AddTask([=]() {
		int index = static_cast<int>(audioName);
		std::string cmd = "stop " + audioFilePaths[index];
		MCIERROR err;
		TCHAR buffer[256] = { 0 };
		err = mciSendString((LPCSTR)cmd.c_str(), 0, 0, 0);

		if (err) {
			mciGetErrorString(err, buffer, 256);
			std::cout << buffer << std::endl;
			return false;
		}
		});

	return true;
}
