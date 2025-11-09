#include "..\Inc\init.h"
#include "..\Inc\init2.h"
#include "stdio.h"

// Кнопка 6 - включение светодиодов
// Кнопка 7 - смена режима работы
int main(void)
{
    macros_init();
    
    uint32_t counter_LED1 = 0;
    uint32_t counter_LED2 = 0;
    uint32_t counter_LED3 = 0;
    uint32_t change_mode = 0;

    while(1)
    {
 
        if (pressed_button(IDRC7_))  
        {
            change_mode = !change_mode;
        }

        if(pressed_button(IDRC6_))
        {
            if(change_mode == 0)
            {
                if(counter_LED1 == 0)
                    counter_LED1 = 1;
                else if(counter_LED2 == 0)
                    counter_LED2 = 1;
                else if(counter_LED3 == 0)
                    counter_LED3 = 1;
            }
            else
            {
                if(counter_LED3 == 1)
                    counter_LED3 = 0;
                else if(counter_LED2 == 1)
                    counter_LED2 = 0;
                else if(counter_LED1 == 1)
                    counter_LED1 = 0;
            }
        }

        if(counter_LED1)
        {
            SET_BIT(BSRR_, BSRRB_0_ON);
        }
        else
        {
            SET_BIT(BSRR_, BSRRB_0_OFF);
        }

        if(counter_LED2)
        {
            SET_BIT(BSRR_, BSRRB_7_ON);
        }
        else
        {
            SET_BIT(BSRR_, BSRRB_7_OFF);
        }

        if(counter_LED3)
        {
            SET_BIT(BSRR_, BSRRB_14_ON);
        }
        else
        {
            SET_BIT(BSRR_, BSRRB_14_OFF);
        }
    }
}

// int main(void)
// {
//     RCC_init();
//     macros_init();
    
//     uint32_t counter_LED1 = 0;
//     uint32_t counter_LED2 = 0;
//     uint32_t counter_LED3 = 0;
//     uint32_t change_mode = 0;

//     while(1)
//     {
 
//         if (pressed_button(IDRC7_))  
//         {
//             change_mode++;
//             if(change_mode == 3)
//             {
//                 change_mode = 0;
//             }
//         }

//         if(pressed_button(IDRC6_))
//         {
//                 if(counter_LED1 == 1 && counter_LED2 == 1 && counter_LED3 == 1)
//                 {
//                     counter_LED1 = 0;
//                     counter_LED2 = 0;
//                     counter_LED3 = 0;
//                 }
//                 else
//                 {
//                 if(counter_LED1 == 0)
//                     counter_LED1 = 1;
//                 else if(counter_LED2 == 0)
//                     counter_LED2 = 1;
//                 else if(counter_LED3 == 0)
//                     counter_LED3 = 1;
//                 }
//         }

//         if((IDRC_ & IDRC6_) && (IDRC_ & IDRC7_))
//         {
//             glow_LED(counter_LED1, counter_LED2, counter_LED3, change_mode);
//         }
//     }
// }

// #include "..\Inc\init.h"
// #include "stdio.h"

// // Кнопка 6 - включение светодиодов
// // Кнопка 7 - смена режима работы

