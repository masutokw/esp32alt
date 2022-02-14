#include "tb6612.h"
int32_t max_steps;
stepper focus_motor;


void init_stepper(stepper *motor)
{
    motor->max_steps=50000;
     motor->backslash=0;
     motor->step_size=0.1;
     motor->speed=10;
     motor->position=0; 
     motor->target=0;
     motor->backcounter=0;
     motor->resolution=0;
     motor->state=stop;
     motor->pcounter=0;
     motor->resolution;
     motor->temperature=25;
     motor->period=0;
     motor->periodtemp=0;
}

void move_to(stepper *motor,long int  target)
{
    motor->target = target;
    motor->state = slew;
    if ( motor->position == target){
        motor->state = stop;
        motor->resolution = 0;
        motor->target=motor->position;
    }
    else if ((motor->target) < motor->position)
        motor->resolution = -1;
    else if ((motor->target) > motor->position)
        motor->resolution = 1;

  
}
void do_step(stepper *motor)
{
  if ((motor->state == slew) && (motor->position == motor->target))
  {
    motor->state = sync;
    motor->resolution = 0;
    WA_O; WB_O
  }
  if (motor->state!=sync){
  if (( motor->backcounter <= 0) || ( motor->backcounter >= motor->backslash))
    motor->position += motor->resolution;
  else
    motor->backcounter += motor->resolution ;

  motor->pcounter = (motor->pcounter + motor->resolution);
  if (motor->pcounter < 0)
    motor->pcounter += 8;
  motor->pcounter %= 8;
  step_out(motor->pcounter);
  }

}

void step_out(uint8_t step)
{
  switch (step) {
    case 0: WA_P; WB_N; break;
    case 1: WA_P; WB_O; break;
    case 2: WA_P; WB_P; break;
    case 3: WA_O; WB_P; break;
    case 4: WA_N; WB_P; break;
    case 5: WA_N; WB_O; break;
    case 6: WA_N; WB_N; break;
    case 7: WA_O; WB_N; break;
    default: break;

  }
}