#include "Globals.h"
#include "Tasks.h"
#include "Display.h"
#include "MotorControls.h"

void taskProcessFlow() {
  if (taskManager.isCompleted()) {
    stop(NORMAL_BRAKE_TIME);
    displayMsg("All Done!");
    while(1);
  }
  
  switch(taskManager.getCurrentState()) {
    case TaskState::IDLE:
      taskManager.begin();
      break;

    case TaskState::TASK_0:
      executeTask0();
      break;

    case TaskState::TASK_1:
      executeTask1();
      break;
      
    case TaskState::TASK_2:
      executeTask2();
      break;
      
    case TaskState::TASK_3:
      executeTask3();
      break;
      
    case TaskState::TASK_4:
      executeTask4();
      break;
      
    case TaskState::TASK_5:
      executeTask5();
      break;
      
    case TaskState::TASKS_COMPLETED:
      stop(NORMAL_BRAKE_TIME);
      break;
  }
}