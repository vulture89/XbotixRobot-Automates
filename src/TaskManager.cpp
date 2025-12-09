#include "TaskManager.h"
#include "Display.h"

TaskManager::TaskManager() 
  : currentState(TaskState::IDLE), 
    currentStatus(TaskStatus::NOT_STARTED) {
}

void TaskManager::begin() {
  if (currentState == TaskState::IDLE) {
    currentState = TaskState::TASK_0;
    currentStatus = TaskStatus::IN_PROGRESS;
  }
}

void TaskManager::completeCurrentTask() {
  currentStatus = TaskStatus::COMPLETED;
  goToNextTask();
}

void TaskManager::failCurrentTask() {
  currentStatus = TaskStatus::FAILED;
  
  displayMsg("Restarting task in 3s");
  delay(3000);

  currentStatus = TaskStatus::IN_PROGRESS;
}

void TaskManager::goToNextTask() {
  switch (currentState) {
    case TaskState::IDLE:
      currentState = TaskState::TASK_0;
      break;
    case TaskState::TASK_0:
      currentState = TaskState::TASK_1;
      break;
    case TaskState::TASK_1:
      currentState = TaskState::TASK_2;
      break;
    case TaskState::TASK_2:
      currentState = TaskState::TASK_3;
      break;
    case TaskState::TASK_3:
      currentState = TaskState::TASK_4;
      break;
    case TaskState::TASK_4:
      currentState = TaskState::TASK_5;
      break;
    case TaskState::TASK_5:
      currentState = TaskState::TASKS_COMPLETED;
      break;
    case TaskState::TASKS_COMPLETED:
      // Stay in completed state
      break;
  }
  currentStatus = TaskStatus::IN_PROGRESS;
}

TaskState TaskManager::getCurrentState() {
  return currentState;
}

TaskStatus TaskManager::getCurrentStatus() {
  return currentStatus;
}

bool TaskManager::isCompleted() {
  return currentState == TaskState::TASKS_COMPLETED;
}