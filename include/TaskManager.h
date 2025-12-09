#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <Arduino.h>

enum class TaskState : uint8_t {
  IDLE,
  TASK_0,
  TASK_1,
  TASK_2,
  TASK_3,
  TASK_4,
  TASK_5,
  TASKS_COMPLETED
};

enum class TaskStatus : uint8_t {
  NOT_STARTED,
  IN_PROGRESS,
  COMPLETED,
  FAILED
};

class TaskManager {
private:
  TaskState currentState;
  TaskStatus currentStatus;

public:
  TaskManager();
  void begin();
  void completeCurrentTask();
  void failCurrentTask();
  void goToNextTask();
  TaskState getCurrentState();
  TaskStatus getCurrentStatus();
  bool isCompleted();
};

void taskProcessFlow();

#endif