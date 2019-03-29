#include <xc.h>
#include "sumovore.h"
#include "motor_control.h"

void follow_simple_curves(void);
void follow_right_angle_turn(void);
void follow_acute_angle_turn(void);
void follow_stop(void);

unsigned char left_counter2(unsigned char left_value2);
unsigned char right_counter2(unsigned char right_value2);
unsigned char left_counter3(unsigned char left_value3);
unsigned char right_counter3(unsigned char right_value3);
unsigned char all_counter(unsigned char all_value);

void spin_left(void);
void turn_left(void);
void straight_fwd(void);
void turn_right(void);
void spin_right(void);

void straight_forward_fast(void);
void straight_forward_medium(void);
void straight_forward_slow(void);
void straight_stop(void);
void straight_reverse_slow(void);
void straight_reverse_medium(void);
void straight_reverse_fast(void);
void default_settings(void);

unsigned char left_value2 = 0, right_value2 = 0, left_value3 = 0, right_value3 = 0, all_value = 0;
unsigned char detect_left_value2 = 0, detect_right_value2 = 0, detect_left_value3 = 0, detect_right_value3 = 0, detect_all_value = 0;
unsigned char pick_counter = 0;

void motor_control(void)
{   
    switch(pick_counter)
    {
        default:
                follow_simple_curves();
                left_counter2(left_value2);
                right_counter2(right_value2);
                left_counter3(left_value3);
                right_counter3(right_value3);
                all_counter(all_value);
                
                if (left_value3 > 0)
                    pick_counter = 3;
                else if (right_value3 > 0)
                    pick_counter = 3;
                else if (all_value)
                    pick_counter = 5;
                else if (left_value2 > 0)
                    pick_counter = 2;
                else if (right_value2 > 0)
                    pick_counter = 2;
                
                break;
        case 3:
                follow_right_angle_turn();
                break;
        case 5: 
                follow_stop();
                break;
        case 2:
                follow_acute_angle_turn();
                break;
        
    }                    
}

unsigned char left_counter2(unsigned char left_value2)
{
    if (SeeLine.B == 0b01100u)
    {
        left_value2++;
    }
    return left_value2;
}

unsigned char right_counter2(unsigned char right_value2)
{
    if (SeeLine.B == 0b00110u)
    {
        right_value2++;
    }
    return right_value2;
}

unsigned char left_counter3(unsigned char left_value3)
{
    if (SeeLine.B == 0b11100u)
        left_value3++;
    
    return left_value3;
}

unsigned char right_counter3(unsigned char right_value3)
{
    if (SeeLine.B == 0b00111u)
        right_value3++;

   return right_value3;
}

unsigned char all_counter(unsigned char all_value)
{
    if (SeeLine.B == 0b11111u)
    {
        all_value++;
    }
    return all_value;
}

void follow_simple_curves(void)
{
     if ( SeeLine.b.Center ) straight_fwd();        //sensors bit 2, 0b00100u
     else if (SeeLine.b.Left) spin_left();          //sensors bit 4, 0b10000u
     else if (SeeLine.b.CntLeft) turn_left();       //sensors bit 3, 0b01000u
     else if (SeeLine.b.CntRight) turn_right();     //sensors bit 1, 0b00010u
     else if (SeeLine.b.Right) spin_right();        //sensors bit 0, 0b00001u
}

void follow_right_angle_turn(void)
{
    detect_right_value3 = right_counter3(right_value3);
    detect_left_value3 = left_counter3(left_value3);
    
    if (detect_right_value3 > detect_left_value3)
    {
        spin_right;
        right_counter3(0);
        left_counter3(0);
    }
    else if (detect_right_value3 < detect_left_value3)
    {
        spin_left;
        right_counter3(0);
        left_counter3(0);
    }
    else if (detect_right_value3 == detect_left_value3)
    {
        straight_fwd;  
        right_counter3(0);
        left_counter3(0);
    }

}

void follow_acute_angle_turn(void)
{
    detect_right_value2 = right_counter2(right_value2);
    detect_left_value2 = left_counter2(left_value2);
    
    if (detect_right_value2 > detect_left_value2)
    {
        spin_right;
        right_counter2(0);
        left_counter2(0);
    }
    else if (detect_right_value2 < detect_left_value2)
    {
        spin_left;
        right_counter2(0);
        left_counter2(0);
    }
    else if (detect_right_value2 == detect_left_value2)
    {
        straight_fwd;  
        right_counter2(0);
        left_counter2(0);
    }

}

void follow_stop(void)
{
    detect_all_value = all_counter(all_value);

    if (detect_all_value > 10)
    {
        straight_stop();
        all_counter(0);
        right_counter3(0);
        left_counter3(0);
    }
    
}


void spin_left(void)
{
  set_motor_speed(left, rev_fast, 0); 
  set_motor_speed(right, fast, 0); 
}

void turn_left(void)
{
  set_motor_speed(left, stop, 0); 
  set_motor_speed(right, fast, 0); 
}
void straight_fwd(void)
{
  set_motor_speed(left, fast, 0); 
  set_motor_speed(right, fast, 0); 
}
void spin_right(void)
{
  set_motor_speed(left, fast, 0); 
  set_motor_speed(right, rev_fast, 0); 
}
void turn_right(void)
{
  set_motor_speed(left, fast, 0); 
  set_motor_speed(right, stop, 0); 
}

//void spin_left(void)
//{
//  set_motor_speed(left, rev_fast, 0); //
//  set_motor_speed(right, fast, 0); //
//}
//
//void turn_left(void)
//{
//  set_motor_speed(left, stop, 0); //
//  set_motor_speed(right, fast, 0); //
//}
//void straight_fwd(void)
//{
//  set_motor_speed(left, fast, -100); //
//  set_motor_speed(right, fast, 0); //
//}
//void spin_right(void)
//{
//  set_motor_speed(left, fast, -100); //
//  set_motor_speed(right, rev_fast, 0); //
//}
//void turn_right(void)
//{
//  set_motor_speed(left, fast, -100); //
//  set_motor_speed(right, stop, 0); //
//}
//******

void straight_forward_fast(void)
{
    set_motor_speed(left, fast, -100); 
    set_motor_speed(right, fast, 0);
}

void straight_forward_medium(void)
{
    set_motor_speed(left, medium, -100); 
    set_motor_speed(right, medium, -120);
}

void straight_forward_slow(void)
{
    set_motor_speed(left, slow, -100); 
    set_motor_speed(right, slow, -117);
}

void straight_stop(void)
{
    set_motor_speed(left, stop, 0); 
    set_motor_speed(right, stop, 0);
}

void straight_reverse_slow(void)
{
    set_motor_speed(left, rev_slow, 100); 
    set_motor_speed(right, rev_slow, 130);
}

void straight_reverse_medium(void)
{
    set_motor_speed(left, rev_medium, 100); 
    set_motor_speed(right, rev_medium, 127);
}

void straight_reverse_fast(void)
{
    set_motor_speed(left, rev_fast, 0); 
    set_motor_speed(right, rev_fast, 0);
}

void default_settings(void)
{
    OpenTimer0(TIMER_INT_OFF & T0_SOURCE_INT & T0_16BIT & T0_PS_1_256);
    
    WriteTimer0(0);
    TMR0IF = 0;
    while(!TMR0IF)
    {
        straight_forward_fast();
    }
        
    WriteTimer0(0);
    TMR0IF = 0;
    while(!TMR0IF)
    {
        straight_forward_medium();
    }
    
    WriteTimer0(0);
    TMR0IF = 0;
    while(!TMR0IF)
    {
        straight_forward_slow();
    }
    
    WriteTimer0(0);
    TMR0IF = 0;
    while(!TMR0IF)
    {
        straight_stop();
    }
    

    WriteTimer0(0);
    TMR0IF = 0;
    while(!TMR0IF)
    {
        straight_reverse_slow();
    }
   
    WriteTimer0(0);
    TMR0IF = 0;
    while(!TMR0IF)
    {
        straight_reverse_medium();
    }
    
    WriteTimer0(0);
    TMR0IF = 0;
    while(!TMR0IF)
    {
        straight_reverse_fast();
    }
    
    WriteTimer0(0);
    TMR0IF = 0;
    while(!TMR0IF)
    {
        straight_stop();
    }
 
}
