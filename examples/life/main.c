// \file
// This program waits for button presses on each of the buttons attached
// to a board and toggles the LED with the same index. For example, if the first
// button is pressed, the first LED is toggled. If the third button is pressed,
// the third LED is toggled.

#include <button.h>
#include <life.h>
#include <stdio.h>

// Callback for button presses.
//   btn_num: The index of the button associated with the callback
//   val: 1 if pressed, 0 if depressed
static void button_callback(int btn_num,
                            int val,
                            __attribute__((unused)) int arg2,
                            __attribute__((unused)) void *ud)
{
  if (btn_num == 0)
  {
    if (val == 1)
    {
      printf("Pressed!\n");
      int meaning_of_life;
      int life_err = life_meaning(&meaning_of_life);
      if (life_err < 0)
      {
        printf("Error getting the meaning of life: %d\n", life_err);
        return;
      }
      printf("The meaning of life is %d\n", meaning_of_life);
    }
  }
  else if (btn_num == 1)
  {
    if (val == 1)
    {
      printf("Pressed!\n");
      define_life(52);
    }
  }
}

int main(void)
{
  int err;

  err = button_subscribe(button_callback, NULL);
  if (err < 0)
    return err;

  // Enable interrupts on each button.
  int count;
  err = button_count(&count);
  if (err < 0)
    return err;

  for (int i = 0; i < count; i++)
  {
    button_enable_interrupt(i);
  }

  return 0;
}
