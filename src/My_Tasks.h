#ifndef MY_TASKS__
#define MY_TASKS__

static void MainTask(void *pv);
void LedTaskQuizSW5(void *pv);
void MakeLedTask(void);
void ButtonTask(void *pv);
void DebounceButtTask(void *pv);
void MakeButtonsTask(void);
void MakeDebounceButtTask(void);

#endif