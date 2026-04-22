#pragma once

char* FullPath(char* file);
char* FullWritablePath(char* file);

void LoadProgress();
void SaveProgress();

#ifdef __EMSCRIPTEN__
void SyncPersistentStorage();
#endif